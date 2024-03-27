#pragma once
#include "backups.hpp"
#include <unistd.h>
extern CloudBackups::DataMange *dataMange;
namespace CloudBackups
{
    class HotMange
    {
    private:
        std::string back_dir;
        std::string pack_dir;
        std::string pack_suffix;
        int hot_time;
        // 热点文件返回true
        bool HotJudge(const std::string &filename)
        {
            FileUtil file(filename);
            if (file.isExit() == false)
            {
                LOG(FATAL, "error!");
                return false;
            }
            time_t last = file.last_visit_time();
            time_t now = time(nullptr);
            if (now - last > hot_time)
            {
                return true;
            }
            return false;
        }

    public:
        HotMange()
        {
            Config *config = Config::GetInstance();
            back_dir = config->GetBackDir();
            pack_dir = config->GetPackfileDir();
            pack_suffix = config->GetPackfileSuffix();
            hot_time = config->GetHotTime();
            // 目录不存在则创建
            FileUtil pack(pack_dir);
            if (pack.isExit() == false)
            {
                pack.mkdir();
            }
            FileUtil back(back_dir);
            if (back.isExit() == false)
            {
                back.mkdir();
            }
        }
        bool RunModule()
        {
            while (true)
            {
                // 遍历备份目录
                FileUtil file(back_dir);
                std::vector<std::string> array;
                file.ls(array);
                // 判断文件是否是热点文件
                for (auto &item : array)
                {
                    // 热点文件不处理
                    if (HotJudge(item) == true)
                    {
                        continue;
                    }
                    // 对非热点文件进行压缩
                    FileUtil compress(item);
                    // 获取文件备份信息
                    BackupInfo backupInfo;
                    if (dataMange->GetByRealPath(item, backupInfo) == false)
                    {
                        // 文件存在，无备份信息，漏记录
                        backupInfo.InitBackupInfo(item);
                        dataMange->Insert(backupInfo);
                    }
                    compress.zip(backupInfo.pack_path);
                    //  删除源文件
                    compress.removeFile();
                    //  修改备份信息
                    backupInfo.packflag = true;
                    dataMange->UpDate(backupInfo);
                }
                usleep(1000); // 避免空目录循环遍历，消耗资源
            }
            return true;
        }
    };
}