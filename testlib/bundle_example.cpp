#include <iostream>
#include <string>
#include <fstream>
#include "../bundle/bundle.h"
using namespace std;
int main(int argc, char const *argv[])
{
    cout << "argv[1]是原始文件名称" << endl;
    cout << "argv[2]是压缩后的文件名称" << endl;
    if (argc < 3)
    {
        cout << "参数错误" << endl;
        return 1;
    }
    string filename = argv[1];
    string outname = argv[2];
    ifstream file;
    file.open(filename, std::ios::binary); // 二进制打开
    file.seekg(0, std::ios::end);          // 跳转到文件末尾，方便获取文件大小
    size_t f_size = file.tellg();
    file.seekg(0, std::ios::beg); // 跳转到文件开头
    string body;
    body.resize(f_size);
    file.read(&body[0], f_size); // 文件内容读入body &body[0]为字符串首地址
    file.close();
    string packed = bundle::pack(bundle::LZIP, body); // 压缩后的数据，压缩格式为LZIP
    ofstream outfile;
    outfile.open(outname, std::ios::binary);
    outfile.write(packed.c_str(), packed.size());
    outfile.close();
    cout << "压缩完成" << endl;
    return 0;
}
