#include <iostream>
#include <string>
#include <vector>
#include <queue> //线性容器
#include <fstream>
#include <regex>
#include "chttp.h"
using namespace std;

//预处理器包含
//_CRT_SECURE_NO_WARNINGS
//_WINSOCK_DEPRECATED_NO_WARNINGS


class spider
{
public:
	spider()
	{
		menu();
		cout << "构造函数调用" << endl << endl;
		cin_url();
		start_catch(url);
	}

	void menu() //简介
	{
		cout << "\t\t\t欢迎使用灏清v1.0简单爬虫" << endl;
	}

	void cin_url() //获取url
	{
		cout << "请输入需要抓取的url地址：";
		cin >> url;
	}

	bool start_catch(string url) //开始抓取
	{
		queue<string> q; //URL队列
		q.push(url);

		while (!q.empty()) //队列是否为空
		{
			string currentUrl = q.front(); //取出当前url
			q.pop(); //删除
			//解析url
			chttp http;
			http.Init(); //初始化套接字
			http.anasyleUrl(currentUrl);
			cout << http.m_host << endl << http.m_object << endl;
			if (false == http.Connect())
			{
				cout << "连接服务器失败" << GetLastError << endl;
			}
			else
			{
				cout << "连接服务器成功" << endl;
			}
			string html;
			http.gethttp(html);
			http.close_socket();
			//编码转换

			char* pszbuf = new char[html.length() + 1];
			wchar_t* pszwidebuf = new wchar_t[(html.length() + 1) * 2];
			MultiByteToWideChar(CP_UTF8, 0, html.c_str(), html.length(), pszwidebuf, (html.length() + 1) * 2);
			WideCharToMultiByte(CP_ACP, 0, pszwidebuf, wcslen(pszwidebuf), pszbuf, html.length() + 1, NULL, NULL);
			cout << html << endl;
			delete[] pszbuf;
			delete[] pszwidebuf;

			vector<string> vecImage;
			//解析网页内容
			smatch mat; //正则表达式
			regex rex("http://[^\\s'\"<>()]+");
			string::const_iterator start = html.begin();
			string::const_iterator end = html.end();
			//匹配到了图片连接
			while (regex_search(start, end, mat, rex))
			{
				string per(mat[0].first, mat[0].second);
				cout << per << endl;
				if (per.find(".jpg") != string::npos || per.find(".png") != string::npos || per.find(".jpeg") != string::npos)
				{
					vecImage.push_back(per);
				}
				else
				{
					if (per.find("http://www.w3.org") != string::npos)
						q.push(per);
				}
				start = mat[0].second;
			}
			//下载图片
			for (int i = 0; i <= vecImage.size(); i++)
			{
				chttp http_download;
				string Filename = "D:\\img\\" + vecImage[i].substr(vecImage[i].find_last_of('/') + 1);
				http_download.download(vecImage[i], Filename);
			}
		}
		return 0;
	}

	~spider()
	{
		cout << "析构函数调用" << endl;
	}
public:
	string url;
};

int main()
{
	spider Spider;
}