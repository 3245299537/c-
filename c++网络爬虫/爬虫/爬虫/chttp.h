#include <iostream>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") //�����
using namespace std;

class chttp
{
public:
	bool m_bhttps; //�ǲ���httpsЭ��
	string m_host; //����
	string m_object; //��Դ·��
	SOCKET m_socket; //�׽���
	string get_post; //get��Ϣ
public:
	chttp();
	~chttp();
	//��ʼ������
	bool Init();
	bool anasyleUrl(string url); //����url
	//���ӷ�����
	bool Connect();
	//������ҳԴ����
	bool gethttp(string& html);
	//����ͼƬ
	bool download(string url, string filename);
	bool close_socket();
};