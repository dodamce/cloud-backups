#include "../httplib/httplib.h"
#define SEVER_IP "116.204.70.147"
#define SEVER_PORT 8081
int main(int argc, char const *argv[])
{
    httplib::Client client(SEVER_IP, SEVER_PORT);
    // 区域文件结构体
    struct httplib::MultipartFormData item;
    item.name = "file"; // 和服务器 /multipart post方法注册函数方法中处理的文件名一致
    item.filename = "hello.txt";
    item.content = "hello world"; // 这个文件的内容，为了省事，正常情况这个文件内容需要io读取
    item.content_type = "text/plain";
    httplib::MultipartFormDataItems items; // 一次性可以上传多个文件数据，本质是MultipartFormData数组
    items.push_back(item);
    httplib::Result response = client.Post("/multipart", items); // 与服务器注册的uri一致 ,向服务器发送POST请求
    std::cout << "server response.status: " << response->status << std::endl;
    std::cout << "server response.body: " << response->body << std::endl;
    return 0;
}
