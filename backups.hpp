#pragma once
#include "./util/fileutil.hpp"
#include <unordered_map>
#include <pthread.h>
#include "./config/config.hpp"
namespace CloudBackups
{
    struct BackupInfo
    {
        bool packflag;         // 压缩标记
        size_t size;           // 文件大小
        time_t mtime;          // 最后一次修改时间
        time_t atime;          // 最后一次访问时间
        std::string real_path; // 实际文件存储位置
        std::string pack_path; // 文件压缩存储位置
        std::string url;
        // 打开文件初始化BackInfo
        BackupInfo(const std::string &real_path)
        {
            this->packflag = false;
            FileUtil file(real_path);
            this->size = file.filesize();
            this->mtime = file.last_modify_time();
            this->atime = file.last_visit_time();
            this->real_path = real_path;
            // 获取配置文件的压缩文件路径
            Config *config = Config::GetInstance();
            std::string packdir = config->GetPackfileDir();   // 压缩文件根目录
            std::string suffix = config->GetPackfileSuffix(); // 压缩文件后缀
            std::string zipname = file.filename() + suffix;
            this->pack_path = packdir + "/" + zipname; // 压缩文件路径
            std::string download = config->GetDownloadPrefix();
            this->url = download + "/" + file.filename(); // 下载请求路径
        }
    };
}
