#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include "log.hpp"
#include "../bundle/bundle.h"
namespace CloudBackups
{
    class FileUtil
    {
    private:
        std::string _filepath; // 文件名称 uri格式
        struct stat st;        // 文件属性

    public:
        FileUtil(const std::string &filepath)
        {
            _filepath = filepath;
            if (stat(_filepath.c_str(), &st) < 0)
            {
                LOG(WARNING, "get file stat failed! maybe this file not exits");
            }
        }
        int64_t filesize() { return st.st_size; }         // 获取文件大小，失败返回-1
        time_t last_modify_time() { return st.st_mtime; } // 获取文件最后修改时间
        time_t last_visit_time() { return st.st_atime; }  // 获取文件最后访问时间
        std::string filename()                            // 文件名称
        {
            size_t pos = _filepath.find_last_of("/");
            if (pos == std::string::npos)
            {
                return _filepath;
            }
            return _filepath.substr(pos + 1);
        }
        bool getPoslen(std::string &body, size_t pos, size_t len) // 从文件中读取len个字节，从pos位置开始读取，读取内容放到body中，为了实现断点续传
        {
            size_t size = this->filesize(); // 文件大小
            if (pos >= size)
            {
                LOG(ERROR, "pos is out of range!");
                return false;
            }
            if (pos + len > size)
            {
                LOG(ERROR, "pos + len is out of range!");
                return false;
            }
            std::ifstream ifs;
            ifs.open(_filepath.c_str(), std::ios::binary);
            if (!ifs.is_open())
            {
                LOG(ERROR, "open file failed!");
                return false;
            }
            ifs.seekg(pos, std::ios::beg);
            body.resize(len);
            ifs.read(&body[0], len);
            if (!ifs.good())
            {
                // 上次读取出错
                LOG(ERROR, "read file failed!");
                ifs.close();
                return false;
            }
            ifs.close();
            return true;
        }
        bool getContent(std::string &body) // 获取整体的文件数据
        {
            size_t size = this->filesize();
            return getPoslen(body, 0, size);
        }
        bool setContent(const std::string &body) // 设置文件内容
        {
            std::ofstream ofs;
            ofs.open(_filepath.c_str(), std::ios::binary);
            if (!ofs.is_open())
            {
                LOG(ERROR, "open file failed!");
                return false;
            }
            ofs.write(body.c_str(), body.size());
            if (!ofs.good())
            {
                // 上次写入出错
                LOG(ERROR, "write file failed!");
                ofs.close();
                return false;
            }
            ofs.close();
            return true;
        }
        bool zip(const std::string &packname) // 文件压缩功能,传入压缩后名称
        {
            // 读取源文件所有内容
            std::string body;
            if (this->getContent(body) == false)
            {
                // 获取源文件数据失败
                LOG(ERROR, "get file content failed!");
                return false;
            }
            // 对数据进行压缩
            std::string packed = bundle::pack(bundle::LZIP, body);
            // 保存压缩后的数据
            FileUtil file(packname);
            if (file.setContent(packed) == false)
            { // 保存压缩后的数据失败
                LOG(ERROR, "save zip file content failed!");
                return false;
            }
            return true;
        }
        bool unzip(const std::string &filename) // 文件解压缩功能,传入解压缩文件名称
        {
            // 读取当前压缩的数据
            std::string body;
            if (this->getContent(body) == false)
            {
                // 获取源文件数据失败
                LOG(ERROR, "get zip file content failed!");
                return false;
            }
            // 对压缩的数据进行解压
            std::string unpacked = bundle::unpack(body);
            // 保存解压数据
            FileUtil file(filename);
            if (file.setContent(unpacked) == false)
            { // 保存解压数据失败
                LOG(ERROR, "save unzip file content failed!");
                return false;
            }
            return true;
        }
    };
}