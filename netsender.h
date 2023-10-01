
#pragma once

#include <string>
#include <memory>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

//定义一个返回的结构体,里面包含接收到数据的相关信息.
//如果是udp,里面是struct sockaddr
//如果是TCP,里面是socket.
//如果是WEBSOCKET,里面是websocket相关的接口信息.
typedef union {
	struct _udp {
		struct sockaddr socket;
	} udp;
	struct _tcp {
		int socket;
	} tcp;
} SOCKETINFO;

//data callback function .
typedef void (*DATAPROC)(string, SOCKETINFO*, void*);

class NetSender {
    public:
	typedef enum {
		PROTOCOL_UDP = 0,
		PROTOCOL_TCP = 1,
		PROTOCOL_WEBSOCKET = 2,
		PROTOCOL_HTTP = 3,
	} PROTOCOL_TYPE;

	typedef enum {
	    TYPE_SERVER = 0,
	    TYPE_CLIENT = 1,
	} NETSENDER_TYPE;

	//二个函数二选一,不用二个都调用.可以根据类构造时填进来的类型来调用.
//	virtual bool createServer() = 0;
//	virtual bool connect(string server) = 0;

	virtual bool isConnect() = 0;
	//virtual int sendBuf(string str, struct sockaddr* addr) = 0;
	virtual int sendBuf(string str) = 0;
	virtual int sendBuf(string str,SOCKETINFO* socketinfo) = 0;
//	virtual string recvBuf() = 0;
	virtual void setDataCB(DATAPROC dataproc, void* arg) = 0;
	virtual bool disconnect() = 0;
	virtual ~NetSender() {};	//需要一个虚函数的析构函数,才能实现从基类的指针链式调用释放子类.
};

//指定创建的是服务器端还是客户端, 如果不客户端,后面的参数是要连接的服务器.
//shared_ptr<NetSender> createSender(NetSender::PROTOCOL_TYPE protocol, NetSender::NETSENDER_TYPE type, string connectServer, int port);
NetSender* createSender(NetSender::PROTOCOL_TYPE protocol, NetSender::NETSENDER_TYPE type, string connectServer, int port);
