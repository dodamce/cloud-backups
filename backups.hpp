#pragma once
#include "./util/fileutil.hpp"
#include <unordered_map>
#include <pthread.h>
#include "./config/config.hpp"
#include "./util/json.hpp"
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
        // 将backupMap持久化存储
        bool Storage()
        {
            // 获取所有数据
            std::vector<BackupInfo> backups;
            this->GetAll(backups);
            // 添加到Json::Value中
            Json::Value root;
            for (size_t i = 0; i < backups.size(); i++)
            {
                Json::Value backup;
                backup["packflag"] = backups[i].packflag;
                backup["size"] = Json::Int64(backups[i].size);
                backup["mtime"] = Json::Int64(backups[i].mtime);
                backup["atime"] = Json::Int64(backups[i].atime);
                backup["real_path"] = backups[i].real_path;
                backup["pack_path"] = backups[i].pack_path;
                backup["url"] = backups[i].url;
                root.append(backup);
            }
            // 持久化 序列化+保存
            //  序列化
            std::string body;
            JsonUtil::serialize(root, body);
            // 保存文件
            FileUtil file(backupFile);
            file.setContent(body);
            return true;
        }
        // 加载配置信息，初始化backupMap
        bool InitLoad()
        {
            // 读取Json文件
            FileUtil file(backupFile);
            if (file.isExit() == false)
            {
                // 服务器文件信息不存在，无需初始化
                return true;
            }
            std::string body;
            file.getContent(body);
            // 反序列化
            Json::Value root;
            if (JsonUtil::unserialize(body, root) == true)
            {
                // 将反序列化的数据写到map上
                for (int i = 0; i < root.size(); i++)
                {
                    BackupInfo backupInfo;
                    backupInfo.packflag = root[i]["packflag"].asBool();
                    backupInfo.size = root[i]["size"].asInt64();
                    backupInfo.mtime = root[i]["mtime"].asInt64();
                    backupInfo.atime = root[i]["atime"].asInt64();
                    backupInfo.real_path = root[i]["real_path"].asString();
                    backupInfo.pack_path = root[i]["pack_path"].asString();
                    backupInfo.url = root[i]["url"].asString();
                    Insert(backupInfo);
                }
                return true;
            }
            return false;
        }
        DataMange()
        {
            backupFile = Config::GetInstance()->GetBackupFile();
            pthread_rwlock_init(&rwlock, nullptr);
            InitLoad();
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
            Storage();
            return true;
        }
        // 更新数据管理模块
        bool UpDate(const BackupInfo &backupInfo)
        {
            pthread_rwlock_wrlock(&rwlock);
            backupMap[backupInfo.url] = backupInfo;
            pthread_rwlock_unlock(&rwlock);
            Storage();
            return true;
        }
        // 通过url获取这个文件
        bool GetByUrl(const std::string &url, BackupInfo &backupInfo)
        {
            pthread_rwlock_wrlock(&rwlock);
            auto pos = backupMap.find(url);
            if (pos == backupMap.end())
            {
                LOG(WARNING, "url map not found you url is: " + url);
                pthread_rwlock_unlock(&rwlock);
                return false;
            }
            backupInfo = pos->second;
            pthread_rwlock_unlock(&rwlock);
            return true;
        }
        // 通过http uri 获取文件信息
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
            LOG(WARNING, "http uti not found you uri is: " + real_url);
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
