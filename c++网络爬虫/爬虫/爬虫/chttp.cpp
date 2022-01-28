#include "chttp.h"

chttp::chttp() //����
{
	chttp::m_bhttps = false;
	m_socket = NULL;
}

chttp::~chttp() //����
{
	cout << "chttp������������" << endl;
}

bool chttp::anasyleUrl(string url) //����url
{
	//�ַ���ת��ΪСд
	//tolower();
	// ��д
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

	//��ȡ������
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

	if (m_host.empty()) //�����ز���
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

//��ʼ�����磬����windows�׽��ֿ�
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

	//�����׽���
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
}
//���ӷ�����
bool chttp::Connect()
{
	//����������ip��ַ
	HOSTENT* p = gethostbyname(m_host.c_str());
	if (p == NULL)
	{
		return false;
	}
	//���ӷ�����
	sockaddr_in sa; //��ַ��
	sa.sin_family = AF_INET;
	sa.sin_port = htons(80); //�����ֽ�˳��ת�������ֽ�˳��
	memcpy(&sa.sin_addr, p->h_addr, 4);

	if (SOCKET_ERROR == connect(m_socket, (sockaddr*)&sa, sizeof(sockaddr)))
	{
		return false;
	}
	return true;
}

//������ҳ //get����
bool chttp::gethttp(string& html)
{
	get_post += "GET " + m_object + " HTTP/1.1\r\n";
	get_post += "Host: " + m_host + "\r\n";
	get_post += "User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Mobile Safari/537.36 Edg/97.0.1072.69\r\n";
	get_post += "Connection: Close\r\n";
	get_post += "\r\n";
	//����get����
	//�׽���send
	if (SOCKET_ERROR == send(m_socket, get_post.c_str(), get_post.length(), 0))
	{
		return false;
	}
	//��������
	char ch = 0;
	while (recv(m_socket, &ch, sizeof(ch), 0)) //ѭ������
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

//����
bool chttp::download(string url, string filename)
{
	get_post += "GET " + m_object + " HTTP/1.1\r\n";
	get_post += "Host: " + m_host + "\r\n";
	get_post += "User-Agent: Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Mobile Safari/537.36 Edg/97.0.1072.69\r\n";
	get_post += "Connection: Close\r\n";
	get_post += "\r\n";
	//����get����
	//�׽���send
	if (SOCKET_ERROR == send(m_socket, get_post.c_str(), get_post.length(), 0))
	{
		return false;
	}
	//���ļ�
	FILE* fp = fopen(filename.c_str(), "wb");
	if (fp == NULL)
	{
		return false;
	}


	//��������
	char ch = 0;
	//����ͷ��Ϣ
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

	//���˺������
	while (recv(m_socket, &ch, sizeof(ch), 0)) //ѭ������
	{
		fwrite(&ch, 1, 1, fp);
	}

	//�ر��ļ�
	fclose(fp);
	return true;
}