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
        BackupInfo() {}
        BackupInfo(const std::string &real_path)
        {
            this->packflag = false;
            FileUtil file(real_path);
            if (file.isExit()) // 文件存在时才可以获取下面的信息
            {
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
            else
            {
                LOG(FATAL, "file not found");
            }
        }
    };
    class DataMange
    {
    private:
        pthread_rwlock_t rwlock;                               // 读写锁，读共享，写互斥
        std::unordered_map<std::string, BackupInfo> backupMap; // 文件请求路径和对应信息的哈希表
        std::string backupFile;                                // 数据持久化信息文件，文件格式为json
    public:
        DataMange()
        {
            backupFile = Config::GetInstance()->GetBackupFile();
            pthread_rwlock_init(&rwlock, nullptr);
        }
        ~DataMange()
        {
            pthread_rwlock_destroy(&rwlock);
        }
        // 数据管理模块插入信息
        bool Insert(const BackupInfo &backupInfo)
        {
            pthread_rwlock_wrlock(&rwlock);
            backupMap[backupInfo.url] = backupInfo;
            pthread_rwlock_unlock(&rwlock);
            return true;
        }
        // 更新数据管理模块
        bool UpDate(const BackupInfo &backupInfo)
        {
            pthread_rwlock_wrlock(&rwlock);
            backupMap[backupInfo.url] = backupInfo;
            pthread_rwlock_unlock(&rwlock);
            return true;
        }
        // 通过url获取这个文件
        bool GetByUrl(const std::string &url, BackupInfo &backupInfo)
        {
            pthread_rwlock_wrlock(&rwlock);
            auto pos = backupMap.find(url);
            if (pos == backupMap.end())
            {
                pthread_rwlock_unlock(&rwlock);
                return false;
            }
            backupInfo = pos->second;
            pthread_rwlock_unlock(&rwlock);
            return true;
        }
        // 通过http url 获取文件信息
        bool GetByRealPath(const std::string &real_url, BackupInfo &backupInfo)
        {
            pthread_rwlock_wrlock(&rwlock);
            auto pos = backupMap.begin();
            while (pos != backupMap.end())
            {
                if (pos->second.real_path == real_url)
                {
                    backupInfo = pos->second;
                    pthread_rwlock_unlock(&rwlock);
                    return true;
                }
                pos++;
            }
            pthread_rwlock_unlock(&rwlock);
            return false;
        }
        // 获取请求映射下所有文件信息
        bool GetAll(std::vector<BackupInfo> &backups)
        {
            pthread_rwlock_wrlock(&rwlock);
            backups.clear();
            for (auto &backup : backupMap)
            {
                backups.push_back(backup.second);
            }
            pthread_rwlock_unlock(&rwlock);
            return true;
        }
    };
}
