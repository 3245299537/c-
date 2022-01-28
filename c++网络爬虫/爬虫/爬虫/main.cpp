#include <iostream>
#include <string>
#include <vector>
#include <queue> //��������
#include <fstream>
#include <regex>
#include "chttp.h"
using namespace std;

//Ԥ����������
//_CRT_SECURE_NO_WARNINGS
//_WINSOCK_DEPRECATED_NO_WARNINGS


class spider
{
public:
	spider()
	{
		menu();
		cout << "���캯������" << endl << endl;
		cin_url();
		start_catch(url);
	}

	void menu() //���
	{
		cout << "\t\t\t��ӭʹ�����v1.0������" << endl;
	}

	void cin_url() //��ȡurl
	{
		cout << "��������Ҫץȡ��url��ַ��";
		cin >> url;
	}

	bool start_catch(string url) //��ʼץȡ
	{
		queue<string> q; //URL����
		q.push(url);

		while (!q.empty()) //�����Ƿ�Ϊ��
		{
			string currentUrl = q.front(); //ȡ����ǰurl
			q.pop(); //ɾ��
			//����url
			chttp http;
			http.Init(); //��ʼ���׽���
			http.anasyleUrl(currentUrl);
			cout << http.m_host << endl << http.m_object << endl;
			if (false == http.Connect())
			{
				cout << "���ӷ�����ʧ��" << GetLastError << endl;
			}
			else
			{
				cout << "���ӷ������ɹ�" << endl;
			}
			string html;
			http.gethttp(html);
			http.close_socket();
			//����ת��

			char* pszbuf = new char[html.length() + 1];
			wchar_t* pszwidebuf = new wchar_t[(html.length() + 1) * 2];
			MultiByteToWideChar(CP_UTF8, 0, html.c_str(), html.length(), pszwidebuf, (html.length() + 1) * 2);
			WideCharToMultiByte(CP_ACP, 0, pszwidebuf, wcslen(pszwidebuf), pszbuf, html.length() + 1, NULL, NULL);
			cout << html << endl;
			delete[] pszbuf;
			delete[] pszwidebuf;

			vector<string> vecImage;
			//������ҳ����
			smatch mat; //������ʽ
			regex rex("http://[^\\s'\"<>()]+");
			string::const_iterator start = html.begin();
			string::const_iterator end = html.end();
			//ƥ�䵽��ͼƬ����
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
			//����ͼƬ
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
		cout << "������������" << endl;
	}
public:
	string url;
};

int main()
{
	spider Spider;
}