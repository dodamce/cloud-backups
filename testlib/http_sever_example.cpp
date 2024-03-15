#include "../httplib/httplib.h"

void Hello(const httplib::Request &req, httplib::Response &rsp)
{
    rsp.set_content("Hello World!", "text/plain"); // 设置响应正文
    rsp.status = 200;
}
void Numbers(const httplib::Request &req, httplib::Response &rsp)
{
    auto num = req.matches[1];          // 0保存的是整体path 之后的下标中保存的是正则表达式捕捉的数据，就是uri /number/[i]的i
    rsp.set_content(num, "text/plain"); // 设置响应正文
    rsp.status = 200;
}
void Mutipart(const httplib::Request &req, httplib::Response &rsp)
{
    auto ret = req.has_file("file");
    if (ret == false)
    {
        std::cout << "不是文件上传\n"; // 通常 Mutipart用于上传数据
        rsp.status = 404;
    }
    else
    {
        const auto &file = req.get_file_value("file");
        rsp.body.clear();
        rsp.body = file.filename;
        rsp.body += "\n";
        rsp.body += file.content;
        rsp.set_header("Content-Type", "text/plain");
        rsp.status = 200;
    }
}
int main(int argc, char const *argv[])
{
    httplib::Server svr;                   // 实例化Sever来搭建服务器
    svr.Get("/hi", Hello);                 // 注册一个针对/hi的GET请求映射函数,处理方法为Hello的回调函数
    svr.Get(R"(/numbers/(\d+))", Numbers); // 正则表达式/numbers/1 /numbers/2 .... 都会响应这个方法
    svr.Post("/multipart", Mutipart);
    svr.listen("0.0.0.0", 8081); // 匹配服务器所有网卡
    return 0;
}