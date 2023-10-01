
#include "udpsender.h"

#define SERVER_PORT 8003 
#define BUFF_LEN 1024

udpSender::udpSender(NETSENDER_TYPE type, int port)
	:m_type(type), m_port(port), m_connectOK(false), m_socket(-1), m_cbFunc(NULL), m_cbArg(NULL), m_bStopRecv(true)
{
}

udpSender::~udpSender()
{
	disconnect();
}

bool udpSender::isConnect()
{
	return m_connectOK;
}

//对于客户端,只能往服务器上发.
int udpSender::sendBuf(string str)
{
	sendto(m_socket, str.c_str(), str.length(), 0, &m_svrSockAddr, sizeof(m_svrSockAddr));
	return 1;
}

//对于服务端,需要指定往哪一个客户端发送数据.
int udpSender::sendBuf(string str, SOCKETINFO* socketinfo)
{
	sendto(m_socket, str.c_str(), str.length(), 0, &socketinfo->udp.socket, sizeof(*socketinfo));
	return 1;
}

bool udpSender::initServer()
{
	bool bRet;
	struct sockaddr_in* ser_addr = (struct sockaddr_in*)&m_svrSockAddr;

	m_socket = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
	if(m_socket < 0)
	{
		printf("create socket fail!\n");
		return false;
	}

	memset(ser_addr, 0, sizeof(&ser_addr));
	ser_addr->sin_family = AF_INET;
	ser_addr->sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
	ser_addr->sin_port = htons(m_port);  //端口号，需要网络序转换

	bRet = bind(m_socket, &m_svrSockAddr, sizeof(m_svrSockAddr));
	if(bRet < 0)
	{
		printf("socket bind fail!\n");
		return false;
	}


	bRet = createRecvThread();

	if(bRet == false)
	{
		printf("createRecvThread fail!\n");
		return false;
	}

	m_connectOK = true;

	return true;
}

bool udpSender::connectServer(string strServer)
{
	bool bRet;
	struct sockaddr_in* ser_addr = (struct sockaddr_in*)&m_svrSockAddr;

	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_socket < 0)
	{
		printf("create socket fail!\n");
		return false;
	}

	//如果传进来的是一个域名,需要通过域名获取一个host.
	struct hostent *host = gethostbyname(strServer.c_str());

	memset(ser_addr, 0, sizeof(struct sockaddr_in));
	ser_addr->sin_family = AF_INET;

	if(host != NULL)
		ser_addr->sin_addr.s_addr = inet_addr(inet_ntoa( *(struct in_addr*)host->h_addr_list[0] ));
	else
		ser_addr->sin_addr.s_addr = inet_addr(strServer.c_str());

	ser_addr->sin_port = htons(m_port);  //注意网络序转换

	//==============================
	bRet = createRecvThread();

	if(bRet == false)
	{
		printf("createRecvThread fail!\n");
		return false;
	}

	m_connectOK = true;

	return true;
}

bool udpSender::createRecvThread()
{
	m_bStopRecv = false;
	pthread_create(&m_pidRecv, NULL, &threadReceive, this);
	return true;
}

void udpSender::stopRecvThread()
{
	void* res;
	m_bStopRecv = true;
	pthread_join(m_pidRecv, &res);
}

void* udpSender::threadReceive(void* obj)
{
	udpSender* p = (udpSender*)obj;
	return p->threadReceiveProc();
}


void* udpSender::threadReceiveProc()
{
    char buf[BUFF_LEN];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in client_addr;  //client_addr用于记录发送方的地址信息

	while(!m_bStopRecv)
    {
        memset(buf, 0, BUFF_LEN);
        len = sizeof(client_addr);
		//这个要想办法在退出时结束一下.暂时先不考虑.
        count = recvfrom(m_socket, buf, BUFF_LEN, 0, (struct sockaddr*)&client_addr, &len);  //recvfrom是拥塞函数，没有数据就一直拥塞
        if(count == -1)
        {
            printf("recieve data fail!\n");
            return NULL;
        }

//		cout << "s_addr = " << client_addr.sin_addr.s_addr << endl;
//		cout << " port = " << client_addr.sin_port << endl;
//		cout << " recv count = " << count << "buf = " << buf << endl;

		//通过回调向上一级传递.
		if(m_cbFunc != NULL)
		{
			string str(buf, count);
			m_cbFunc(str, (SOCKETINFO*)&client_addr, m_cbArg);
			//m_cbFunc(buf, (SOCKETINFO*)&client_addr, m_cbArg);
		}
	}

	return NULL;
}

bool udpSender::disconnect()
{
	cout << "udpSender disconnect " << endl;
	if(m_socket >= 0)
	{
		close(m_socket);
		m_socket = -1;
	}
	cout << "udpSender disconnect end" << endl;

	return true;
}

