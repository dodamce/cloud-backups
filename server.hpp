#pragma once
#include "backups.hpp"
#include "./httplib/httplib.h"
#include "./config/config.hpp"
extern CloudBackups::DataMange *dataMange;
namespace CloudBackups
{
    class Server
    {
    private:
        int port;
        std::string ip;
        std::string download_prefix;
        httplib::Server server;
        // 上传文件
        static void Upload(const httplib::Request &request, httplib::Response &response)
        {
            // LOG(INFO, "upload begin");
            // POST请求，文件数据在http正文中，分区存储
            bool ret = request.has_file("file"); // 判断有无上传文件字段
            if (ret == false)
            {
                LOG(ERROR, "request error!");
                response.status = 400;
                return;
            }
            // 获取数据
            const auto &file = request.get_file_value("file");
            std::string backdir = Config::GetInstance()->GetBackDir();
            // 保存文件
            std::string filepath = backdir + FileUtil(file.filename).filename(); // 实际路径+文件名
            FileUtil stream(filepath);
            stream.setContent(file.content);
            // 更新文件信息Json文件
            BackupInfo info(filepath);
            dataMange->Insert(info);
            LOG(INFO, "upload success");
        }
        // 展示页面
        static void ListShow(const httplib::Request &request, httplib::Response &response)
        {
            // LOG(INFO, "list show begin");
            // 获取所有文件信息
            std::vector<BackupInfo> array;
            dataMange->GetAll(array);
            // 根据所有文件信息构建http响应
            std::stringstream ss;
            ss << R"(<!DOCTYPE html><html lang="cn"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>download list</title></head><body>)";
            ss << R"(<h1 align="center">Download List</h1>)";
            for (auto &info : array)
            {
                std::string filename = FileUtil(info.real_path).filename();
                ss << R"(<tr><td><a href=")" << info.url << R"(">)" << filename << "</a></td>";
                ss << R"(<td align="right"> )" << convertTimeStamp2TimeStr(info.mtime) << "  </td>";
                ss << R"(<td align="right">)" << info.size / 1024 << "Kb</td></tr>";
                ss << "<br>";
            }
            ss << "</body></html>";
            response.body = ss.str();
            response.set_header("Content-Type", "text/html");
            response.status = 200;
            LOG(INFO, "list show end");
        }
        // ETag为设计者自行指定 ETags：文件名称-文件大小-最后修改时间 构成
        static std::string GetETag(BackupInfo info)
        {
            std::string etag = FileUtil(info.real_path).filename();
            etag += "-";
            etag += std::to_string(info.size);
            etag += "-";
            etag += std::to_string(info.mtime);
            return etag;
        }
        // 下载文件
        static void Download(const httplib::Request &request, httplib::Response &response)
        {
            // 1. 获取客户端请求资源的路径 request.path
            // 2. 根据路径获取文件备份信息
            BackupInfo info;
            if (dataMange->GetByUrl(request.path, info) == false)
            {
                LOG(WARNING, "file /download not found");
                response.status = 404;
                return;
            }
            // 3. 判断文件是否被压缩,被压缩的话需要先解压缩，删除压缩包，修改备份信息
            if (info.packflag == true)
            {
                // 被压缩,解压到backdir目录浏览
                FileUtil tool(info.pack_path);
                tool.unzip(info.real_path);
                // 删除压缩包
                tool.removeFile();
                info.packflag = false;
                // 修改配置文件
                dataMange->UpDate(info);
            }
            //  4. 读取文件数据放入body中
            FileUtil tool(info.real_path);
            tool.getContent(response.body);
            // 判断断点续传
            bool retrans = false; // 标记断点续传
            std::string befetag;
            if (request.has_header("If-Range"))
            {
                // 断点续传 服务端在下载时响应ETag字段搭配使用判断文件是否被修改
                befetag = request.get_header_value("If-Range");
                if (befetag == GetETag(info))
                {
                    // 文件没修改过
                    retrans = true;
                }
            }
            // 没有If-Range字段或者If-Range字段与ETag不匹配，重新下载
            if (retrans == false)
            {
                // 正常下载
                //  5. 设置响应头部字段ETag Accept-Range字段
                response.set_header("ETag", GetETag(info));
                response.set_header("Accept-Ranges", "bytes");
                response.set_header("Content-Type", "application/octet-stream");
                response.status = 200;
            }
            else
            {
                // 断点续传，了解区间范围
                response.set_header("ETag", GetETag(info));
                response.set_header("Accept-Ranges", "bytes");
                response.status = 206;//cpp-httplib会自动根据请求Range字段对response.body进行切片返回，封装实现
            }
            LOG(INFO, "download success");
        }

    public:
        Server()
        {
            Config *config = Config::GetInstance();
            port = config->GetServerPort();
            ip = config->GetServerIp();
            download_prefix = config->GetDownloadPrefix();
            LOG(INFO, "init server success");
        }
        bool RunMoudle()
        {
            LOG(INFO, "server running");
            // 搭建Http服务器
            server.Post("/upload", Upload); // 文件上传
            server.Get("/list", ListShow);  // 展示页面
            server.Get("/", ListShow);      // 网页根目录也是展示页面
            std::string download_url = download_prefix + "(.*)";
            // LOG(INFO, "DEBUG:" + download_url);
            server.Get(download_url, Download); // 下载文件,正则表达式捕捉要下载的文件
            if (server.listen(ip, port) == false)
            {
                LOG(FATAL, "server listen failed! ip=" + ip);
                return false;
            }
            return true;
        }
    };
}