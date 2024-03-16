// 项目错误日志打印
#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <time.h>
#define INFO 1
#define WARNING 2
#define ERROR 3
#define FATAL 4
#define LOG(level, message) Log(#level, message, __FILE__, __LINE__) // #将宏参数转化为字符串

// 时间戳转化为时间信息
static std::string convertTimeStamp2TimeStr(time_t timeStamp)
{
    struct tm *timeinfo = nullptr;
    char buffer[80];
    timeinfo = localtime(&timeStamp);
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    // printf("%s\n", buffer);
    return std::string(buffer);
}

// 日志级别+日志信息+时间戳+错误文件名称+错误行数
// 日志级别 INFO,WARNING,ERROR,FATAL
void Log(std::string level, std::string msg, std::string file_name, int line)
{

    std::cout << "[" << level << "]"
              << "[" << convertTimeStamp2TimeStr(time(nullptr)) << "]"
              << "[" << msg << "]"
              << "[" << file_name << "]"
              << "[" << line << "]" << std::endl;
}