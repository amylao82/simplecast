
#pragma once

//#include <map>
//#include "message.pb.h"
#include "netsender.h"
#include <iostream>

using namespace std;
//using namespace fairy;

class dataProcBase
{
	public:
		dataProcBase(NetSender* pSender);
		~dataProcBase();

		static void dataRecvCB(string strInfo, SOCKETINFO* sockinfo, void* obj);

		void dataRecvProc(string& strInfo, SOCKETINFO* sockinfo);

		virtual void UserProc(string& strInfo, SOCKETINFO* sockinfo);

		void send_msg(string& str, SOCKETINFO* socketinfo);
		void send_msg(string& str);

	protected:
		NetSender* m_pSender;
	private:
		dataProcBase() {};
};

