#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>
// 封装json API
namespace CloudBackups
{
    class JsonUtil
    {
    public:
        // 序列化
        static bool serialize(const Json::Value &root, std::string &str)
        {
            Json::StreamWriterBuilder builder;
            std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
            std::stringstream ss;
            writer->write(root, &ss);
            str = ss.str();
            return true;
        }
        static bool unserialize(std::string &str, Json::Value &write)
        {
            Json::CharReaderBuilder builder;
            std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
            std::string errs;
            if (!reader->parse(str.c_str(), str.c_str() + str.size(), &write, &errs))
            {
                std::cerr << "parse json error: " << errs << std::endl;
                return false;
            }
            return true;
        }
    };
}