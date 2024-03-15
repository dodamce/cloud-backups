#include <iostream>
#include <fstream>
#include <string>
#include "../bundle/bundle.h"
using namespace std;
int main(int argc, char const *argv[])
{
    cout << "argv[1]是压缩包名称" << endl;
    cout << "argv[2]是解压缩后的文件名称" << endl;
    if (argc < 3)
    {
        cout << "参数错误" << endl;
        return 1;
    }
    string infile = argv[1];
    string outfile = argv[2];
    ifstream inf;
    inf.open(infile, std::ios::binary);
    inf.seekg(0, std::ios::end);
    int size = inf.tellg();
    inf.seekg(0, std::ios::beg);
    string body;
    body.resize(size);
    inf.read(&body[0], size);
    inf.close();
    string unpacked;
    unpacked = bundle::unpack(body); // 解压
    ofstream outf;
    outf.open(outfile, std::ios::binary);
    outf.write(unpacked.c_str(), unpacked.size());
    outf.close();
    cout << "解压缩成功" << endl;
    return 0;
}
