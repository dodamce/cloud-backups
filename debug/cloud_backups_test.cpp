// #include "util/fileutil.hpp"
#include <vector>
#include "../util/json.hpp"
#include "../config/config.hpp"
#include "backups.hpp"
#include "hot.hpp"
#include "server.hpp"
void FileUtilTest(const std::string &filepath) // 文件工具类测试
{
    CloudBackups::FileUtil file(filepath);
    // 测试文件基本信息
    LOG(INFO, "filename: " + file.filename());
    LOG(INFO, "filesize: " + std::to_string(file.filesize()));
    LOG(INFO, "last modify time: " + std::to_string(file.last_modify_time()));
    LOG(INFO, "last visit time: " + std::to_string(file.last_visit_time()));
    // 测试读文件
    std::string body;
    file.getContent(body);
    LOG(INFO, "\n\t-------------file content: ---------------\n\t" + body);
    // 测试写文件
    CloudBackups::FileUtil outfile("./util_test/hello.txt"); // 没有文件会创建
    outfile.setContent(body);
    // 测试压缩，解压功能
    // 压缩
    CloudBackups::FileUtil zipfile(filepath); // 传入要压缩的文件
    std::string zipname = filepath + ".lz";
    zipfile.zip(zipname); // 传入压缩后的文件
    // 解压缩
    CloudBackups::FileUtil unzipfile(zipname); // 传入要解压的文件
    std::string unzipname = filepath + ".unzip.txt";
    unzipfile.unzip(unzipname); // 传入解压完后的文件
    // 测试创建文件夹
    CloudBackups::FileUtil fileCreate("./util_test/test_dir");
    fileCreate.mkdir();
    std::vector<std::string> buff;
    fileCreate.ls(buff);
    for (auto &it : buff)
    {
        LOG(INFO, it);
    }
}
void JsonUtilTest()
{
    std::string name = "小米";
    int age = 10;
    float score[] = {65.5, 67.9};
    Json::Value root;
    root["name"] = name;
    root["age"] = age;
    root["score"].append(score[0]);
    root["score"].append(score[1]);
    std::string buff;
    CloudBackups::JsonUtil::serialize(root, buff);
    std::cout << buff << std::endl;
    Json::Value val;
    CloudBackups::JsonUtil::unserialize(buff, val);
    std::cout << "name se: " << val["name"].asString() << std::endl;
    std::cout << "age se: " << val["age"].asInt() << std::endl;
    for (int i = 0; i < val["socre"].size(); i++)
    {
        std::cout << "score se: " << val["score"][i].asDouble() << std::endl;
    }
}

void ConfigTest()
{
    CloudBackups::Config *conf = CloudBackups::Config::GetInstance();
    std::cout << conf->GetHotTime() << std::endl;
    std::cout << conf->GetServerIp() << std::endl;
    std::cout << conf->GetServerPort() << std::endl;
    std::cout << conf->GetBackDir() << std::endl;
    std::cout << conf->GetBackupFile() << std::endl;
    std::cout << conf->GetDownloadPrefix() << std::endl;
    std::cout << conf->GetPackfileDir() << std::endl;
    std::cout << conf->GetPackfileSuffix() << std::endl;
}

void BackupInfoUnitTest()
{
    //     CloudBackups::BackupInfo backup("./build.sh");
    //     std::cout << backup.packflag << std::endl;
    //     std::cout << backup.mtime << std::endl;
    //     std::cout << backup.atime << std::endl;
    //     std::cout << backup.size << std::endl;
    //     std::cout << backup.pack_path << std::endl;
    //     std::cout << backup.real_path << std::endl;
    //     std::cout << backup.url << std::endl;
    //     std::cout << "DataMange Test Begin" << std::endl;
    //     CloudBackups::DataMange dataMange;
    //     dataMange.Insert(backup);
    //     CloudBackups::BackupInfo info;
    //     dataMange.GetByUrl("./download/build.sh", info);
    //     std::cout << info.packflag << std::endl;
    //     std::cout << info.mtime << std::endl;
    //     std::cout << info.atime << std::endl;
    //     std::cout << info.size << std::endl;
    //     std::cout << info.pack_path << std::endl;
    //     std::cout << info.real_path << std::endl;
    //     std::cout << info.url << std::endl;

    //     info.packflag = true;
    //     dataMange.UpDate(info);
    //     std::cout << "===============" << std::endl;
    //     std::vector<CloudBackups::BackupInfo> buff;
    //     dataMange.GetAll(buff);
    //     for (auto &it : buff)
    //     {
    //         std::cout << it.packflag << std::endl;
    //         std::cout << it.mtime << std::endl;
    //         std::cout << it.atime << std::endl;
    //         std::cout << it.size << std::endl;
    //         std::cout << it.pack_path << std::endl;
    //         std::cout << it.real_path << std::endl;
    //         std::cout << it.url << std::endl;
    //     }
    //     std::cout << "DataMange Test End" << std::endl;
    // 测试初始化Load函数
    CloudBackups::DataMange dataMange;
    std::vector<CloudBackups::BackupInfo> buff;
    dataMange.GetAll(buff);
    for (auto &it : buff)
    {
        std::cout << it.packflag << std::endl;
        std::cout << it.mtime << std::endl;
        std::cout << it.atime << std::endl;
        std::cout << it.size << std::endl;
        std::cout << it.pack_path << std::endl;
        std::cout << it.real_path << std::endl;
        std::cout << it.url << std::endl;
    }
}
CloudBackups::DataMange *dataMange;
void HotUnitTest()
{
    dataMange = new CloudBackups::DataMange();
    CloudBackups::HotMange hot;
    hot.RunModule();
}
void ServerUtilTest()
{
    CloudBackups::Server server;
    dataMange = new CloudBackups::DataMange();
    server.RunMoudle();
}
int main(int argc, char const *argv[])
{
    // FileUtilTest(argv[1]);
    // JsonUtilTest();
    // ConfigTest();
    // BackupInfoUnitTest();
    // HotUnitTest();
    ServerUtilTest();
    return 0;
}
