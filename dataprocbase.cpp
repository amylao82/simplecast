
#include "dataprocbase.h"


dataProcBase::dataProcBase(NetSender* pSender)
	:m_pSender(pSender)
{
	if(m_pSender == NULL)
		return;

	m_pSender->setDataCB(dataProcBase::dataRecvCB, this);
}

dataProcBase::~dataProcBase()
{
    cout << "~dataProcBase" << endl;
}

void dataProcBase::dataRecvCB(string strInfo, SOCKETINFO* sockinfo, void* obj)
{
	dataProcBase* p = (dataProcBase*)obj;
	
	p->dataRecvProc(strInfo, sockinfo);
}

void dataProcBase::dataRecvProc(string& strInfo, SOCKETINFO* sockinfo)
{
	UserProc(strInfo, sockinfo);
}

void dataProcBase::UserProc(string& strInfo, SOCKETINFO* sockinfo)
{
	//do something.
	cout << "receive info = " << strInfo << endl;
}

void dataProcBase::send_msg(string& str, SOCKETINFO* socketinfo)
{
	m_pSender->sendBuf(str, socketinfo);
}

void dataProcBase::send_msg(string& str)
{
	m_pSender->sendBuf(str);
}
