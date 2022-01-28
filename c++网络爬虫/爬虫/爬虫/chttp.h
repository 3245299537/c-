#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") //网络库
using namespace std;

class chttp
{
public:
	bool m_bhttps; //是不是https协议
	string m_host; //域名
	string m_object; //资源路径
	SOCKET m_socket; //套接字
	string get_post; //get信息
public:
	chttp();
	~chttp();
	//初始化网络
	bool Init();
	bool anasyleUrl(string url); //解析url
	//连接服务器
	bool Connect();
	//接受网页源代码
	bool gethttp(string& html);
	//下载图片
	bool download(string url, string filename);
	bool close_socket();
};