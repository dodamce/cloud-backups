# cloud-backups Linux(CentOS)/Windows-C++ 云备份项目
##  1. 项目介绍

此项目完成的是自动将指定文件夹中的文件上传到备份服务器上，并且能够随时通过浏览器进行查看并且下载，其中下载过程支持断点续传，服务器也会对文件进行热点管理，将非热点文件进行压缩处理，节省存储空间

## 2. 实现目标

这个项目实现两端程序，其中包括部署在用户机的客户端程序（Windows环境），上传所需要的备份文件，运行在服务器上的服务端程序，实现备份文件的存储，两端合作实现自动云备份功能。

## 3. 服务器负责的功能

- 针对客户端上传的文件进行备份存储
- 能够对文件进行热点文件的存储和管理，节省存储空间
- 支持客户端浏览器下载文件，并且支持断点续传
- 支持客户端访问浏览器查看文件列表

	##  服务器功能模块划分
	- 数据模块管理：负责服务器上的备份文件信息管理，以便随时获取数据
	- 网络通信模块：搭建网络通信服务器，实现与客户端通信
	- 业务处理模块：针对客户端的各个请求进行对应的业务处理并响应结果，上传，下载，断点续传
	- 热点管理模块：对长时间不访问的文件进行压缩存储。

## 4. 客户端负责的功能

- 指定目录的文件检测，获取文件夹里面的文件
- 判断这个文件是否需要备份，服务器备份过的文件则不需要进行备份，已经备份的文件如果修改也需要重新备份
- 若这个文件被用户打开，则不进行备份。需要每隔一段时间检测更新备份。
- 将需要备份的文件上传备份文件

	## 客户端功能模块划分
	- 数据管理模块：管理备份的文件信息
	- 文件检测模块：监控指定文件夹，获取这个文件夹下所有的文件信息（通过获取到的备份文件信息）
	- 文件备份模块：上传需要备份的文件数据，将数据传递给服务器。

## 5. 环境搭建

### gcc升级7.3版本 CentOS系统
		
	gcc -v 
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/32e002b4f8c1472ebc09f008f3d4868e.png)
我这里版本为4.8.5，所以需要升级gcc

```bash
sudo yum install centos-release-scl-rh centos-release-scl
sudo yum install devtoolset-7-gcc devtoolset-7-gcc-c++
echo "source /opt/rh/devtoolset-7/enable" >> ~/.bashrc
```
一行一行输入，下载一路y即可

gcc -v检查版本：
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/1ab881117840406886e36187f65c7ce5.png)
检查配置文件是否写入，否则下次使用版本又退回4.8.5了（注意是追加重定向，不要把系统配置文件清空了）
 cat ~/.bashrc
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/3826d9fc5abc431aa5f5921d94617379.png)


###  jsoncpp库安装

```bash
sudo yum install epel-release -- 可能由于CentOS系统版本不同可能这条命令会失败，失败了继续执行下面的命令即可
sudo yum install jsoncpp-devel
```
查看安装的库是否成功

```bash
ls /usr/include/
```

看路径下是否有jsoncpp（版本不同可能没有，因为路径不同，但是是和json相关文件夹）


![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/e796684d91c14e919c18a328e3cfcd9a.png)

```bash
ls /usr/include/jsoncpp/json
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/8deba1ba190944dca92c701404509ce5.png)
检查json相关库
```bash
ls /lib64/libjson*
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/47c5a4a2c7764342a3cf3e135e9efa4e.png)
## bundle数据压缩库安装

```bash
sudo yum install git
ping www.github.com 检查github网站是否能访问
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/41f9831da23447ed9b5cbb654ee89a4e.png)

```bash
git clone https://github.com/r-lyeh-archived/bundle.git
```
失败可能由于网络问题，重复试试

实在不行，去Github上下载后上传到服务器上也可以
[bundle Github仓库](https://github.com/r-lyeh-archived/bundle)
[bundle Gitee镜像仓库](https://gitee.com/chooosky/bundle?_from=gitee_search)

### cpp-httplib库安装
[cpp-httplib Github仓库](https://github.com/yhirose/cpp-httplib)
[cpp-httplib Gitee镜像仓库](https://gitee.com/minhanghuang/cpp-httplib?_from=gitee_search)

也可以使用git命令克隆下载，我这里是下载zip文件上传到服务器上安装的


![在这里插入图片描述](https://img-blog.csdnimg.cn/direct/c8a7048395d746878cb9c64eefb19ffb.png)
安装完毕
