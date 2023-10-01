
#pragma once

#include "netsender.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

class udpSender: public NetSender
{
	public:
		//继承来的对外接口
		virtual bool isConnect();
		//virtual int sendBuf(string str, struct sockaddr* addr);
		virtual int sendBuf(string str);
		virtual int sendBuf(string str, SOCKETINFO* socketinfo);
		//	virtual int string recvBuf();
		virtual bool disconnect();
		void setDataCB(DATAPROC dataproc, void* arg) { m_cbFunc = dataproc; m_cbArg = arg; }

	public:
		//非继承来的对外函数.
		udpSender(NETSENDER_TYPE type, int port);
		virtual ~udpSender();
		bool initServer();
		bool connectServer(string strSever);

	protected:
		//不对外函数
		//recevie thread.
		static void* threadReceive(void* obj);
		void* threadReceiveProc(); 
		bool createRecvThread();
		void stopRecvThread();

	protected:
		NETSENDER_TYPE m_type;
		int m_port;
		bool m_connectOK;

		int m_socket;

		DATAPROC m_cbFunc;
		void* m_cbArg;

		//接收数据线程.
		pthread_t m_pidRecv;
		bool m_bStopRecv;
		struct sockaddr m_svrSockAddr;
	private:
		udpSender(){};
};

