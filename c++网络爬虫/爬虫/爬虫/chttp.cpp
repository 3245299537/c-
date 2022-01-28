#include "chttp.h"

chttp::chttp() //构造
{
	chttp::m_bhttps = false;
	m_socket = NULL;
}

chttp::~chttp() //析构
{
	cout << "chttp析构函数调用" << endl;
}

bool chttp::anasyleUrl(string url) //解析url
{
	//字符串转化为小写
	//tolower();
	// 大写
	//toupper();
	string str = url.substr(0, 8);
	if ("https://" == str)
	{
		m_bhttps = true;
	}
	if (str.find("http://") == string::npos)
	{
		m_bhttps = false;
	}
	else
	{
		return false;
	}

	//截取主机名
	int nPos = url.find('/', m_bhttps ? 8 : 7);
	if (nPos == string::npos)
	{
		m_host = url.substr(m_bhttps ? 8 : 7);
		m_object = "/";
	}
	else
	{
		m_host = url.substr(m_bhttps ? 8 : 7, nPos - (m_bhttps ? 8 : 7));
		m_object = url.substr(nPos);
	}

	if (m_host.empty()) //主机截不到
	{
		return false;
	}
	//url.substr(m_bhttps ? 8 : 7, nPos - (m_bhttps ? 8 : 7));

	if (m_object.empty())
	{
		return false;
	}
	return 0;
}

//初始化网络，加载windows套接字库
bool chttp::Init()
{
	WSADATA wd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd))
	{
		return false;
	}
	if (LOBYTE(wd.wVersion) != 2 || HIBYTE(wd.wVersion != 2))
	{
		return false;
	}

	//创建套接字
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
}
//连接服务器
bool chttp::Connect()
{
	//解析域名的ip地址
	HOSTENT* p = gethostbyname(m_host.c_str());
	if (p == NULL)
	{
		return false;
	}
	//连接服务器
	sockaddr_in sa; //地址族
	sa.sin_family = AF_INET;
	sa.sin_port = htons(80); //主机字节顺序转换网络字节顺序
	memcpy(&sa.sin_addr, p->h_addr, 4);

	if (SOCKET_ERROR == connect(m_socket, (sockaddr*)&sa, sizeof(sockaddr)))
	{
		return false;
	}
	return true;
}

//下载网页 //get请求
bool chttp::gethttp(string& html)
{
	get_post += "GET " + m_object + " HTTP/1.1\r\n";
	get_post += "Host: " + m_host + "\r\n";
	get_post += "User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Mobile Safari/537.36 Edg/97.0.1072.69\r\n";
	get_post += "Connection: Close\r\n";
	get_post += "\r\n";
	//发送get请求
	//套接字send
	if (SOCKET_ERROR == send(m_socket, get_post.c_str(), get_post.length(), 0))
	{
		return false;
	}
	//接受数据
	char ch = 0;
	while (recv(m_socket, &ch, sizeof(ch), 0)) //循环接收
	{
		if (ch > 0)
		{
			html = html + ch;
		}
		else
		{
			break;
		}
	}
	return true;
}

bool chttp::close_socket()
{
	closesocket(m_socket);
	m_host.empty();
	m_object.empty();
	m_socket = 0;
	return true;
}

//下载
bool chttp::download(string url, string filename)
{
	get_post += "GET " + m_object + " HTTP/1.1\r\n";
	get_post += "Host: " + m_host + "\r\n";
	get_post += "User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Mobile Safari/537.36 Edg/97.0.1072.69\r\n";
	get_post += "Connection: Close\r\n";
	get_post += "\r\n";
	//发送get请求
	//套接字send
	if (SOCKET_ERROR == send(m_socket, get_post.c_str(), get_post.length(), 0))
	{
		return false;
	}
	//打开文件
	FILE* fp = fopen(filename.c_str(), "wb");
	if (fp == NULL)
	{
		return false;
	}


	//接受数据
	char ch = 0;
	//接收头信息
	while (recv(m_socket, &ch, 1, 0))
	{
		if (ch == '\r')
		{
			recv(m_socket, &ch, 1, 0);
			if (ch == '\n')
			{
				recv(m_socket, &ch, 1, 0);
				if (ch == '\r')
				{
					recv(m_socket, &ch, 1, 0);
					{
						if (ch == '\n')
						{
							break;
						}
					}
				}
			}
		}
	}

	//过滤后的数据
	while (recv(m_socket, &ch, sizeof(ch), 0)) //循环接收
	{
		fwrite(&ch, 1, 1, fp);
	}

	//关闭文件
	fclose(fp);
	return true;
}