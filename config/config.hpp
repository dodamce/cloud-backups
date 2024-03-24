#pragma once
#include "../util/json.hpp"
#include "../util/fileutil.hpp"
#include <mutex>
#define CONFIG_FILE "./cloud_backups.conf"
namespace CloudBackups
{
    class Config
    {
    private:
        Config(const Config &config) = delete;
        Config()
        {
            // 读取配置文件
            CloudBackups::FileUtil file(CONFIG_FILE);
            std::string body;
            if (file.getContent(body) == false)
            {
                LOG(FATAL, "read config error!");
                exit(-1);
            }
            Json::Value root;
            if (CloudBackups::JsonUtil::unserialize(body, root) == false)
            {
                LOG(FATAL, "unserialize config error!");
                exit(-1);
            }
            // 解析配置
            hot_time = root["hot_time"].asInt();
            sever_port = root["server_port"].asInt();
            server_ip = root["server_ip"].asString();
            download_prefix = root["download_prefix"].asString();
            packfile_suffix = root["packfile_suffix"].asString();
            packfile_dir = root["packfile_dir"].asString();
            back_dir = root["back_dir"].asString();
            backup_file = root["backup_file"].asString();
        }
        static Config *instance;
        static std::mutex lock;
        int hot_time;
        int sever_port;
        std::string server_ip;
        std::string download_prefix;
        std::string packfile_suffix;
        std::string packfile_dir;
        std::string back_dir;
        std::string backup_file;

    public:
        static Config *GetInstance()
        {
            if (!instance)
            {
                lock.lock();
                if (!instance)
                {
                    instance = new Config();
                }
                lock.unlock();
            }
            return instance;
        }
        int GetHotTime() { return hot_time; }
        int GetServerPort() { return sever_port; }
        std::string GetServerIp() { return server_ip; }
        std::string GetDownloadPrefix() { return download_prefix; }
        std::string GetPackfileSuffix() { return packfile_suffix; }
        std::string GetPackfileDir() { return packfile_dir; }
        std::string GetBackDir() { return back_dir; }
        std::string GetBackupFile() { return backup_file; }
    };
    Config *Config::instance = nullptr;
    std::mutex Config::lock;
}