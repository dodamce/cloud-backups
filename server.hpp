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
            LOG(INFO, "upload begin");
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
            LOG(INFO, "list show begin");
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
        // 下载文件
        static void Download(const httplib::Request &request, httplib::Response &response)
        {
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