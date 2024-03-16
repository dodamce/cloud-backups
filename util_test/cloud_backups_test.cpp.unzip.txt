#include "util/fileutil.hpp"

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
}

int main(int argc, char const *argv[])
{
    FileUtilTest(argv[1]);
    return 0;
}
