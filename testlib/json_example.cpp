#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>
// json序列化测试
//  int main(int argc, char const *argv[])
//  {
//      const char *name = "小米";
//      int age = 20;
//      float score[] = {77.5, 87.5, 90};
//      Json::Value value;
//      value["姓名"] = name;
//      value["年龄"] = age;
//      value["成绩"].append(score[0]);
//      value["成绩"].append(score[1]);
//      value["成绩"].append(score[2]);
//      Json::StreamWriterBuilder builder;
//      std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
//      std::stringstream ss;
//      writer->write(value, &ss);
//      std::cout << ss.str() << std::endl;
//      return 0;
//  }
using namespace std;

int main(int argc, char const *argv[])
{
    string str = R"({"姓名":"小米","年龄":19,"成绩":[77.5,87.5,90]})"; // c++11语法R"(str)"
    Json::Value value;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    string errs;
    bool ok = reader->parse(str.c_str(), str.c_str() + str.size(), &value, &errs);
    if (!ok)
    {
        std::cout << "parse error: " << errs << std::endl;
    }
    else
    {
        std::cout << "姓名: " << value["姓名"].asString() << std::endl;
        std::cout << "年龄: " << value["年龄"].asInt() << std::endl;
        std::cout << "成绩: " << value["成绩"][0].asFloat() << std::endl;
        std::cout << "成绩: " << value["成绩"][1].asFloat() << std::endl;
        std::cout << "成绩: " << value["成绩"][2].asFloat() << std::endl;
        for(int i=0;i<value["成绩"].size();i++){

            std::cout<<value["成绩"][i]<<" ";
        }
    }
    return 0;
}
