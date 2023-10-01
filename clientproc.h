
#pragma once

#include <algorithm>
#include <vector>

#include "dataprocbase.h"
#include "message.pb.h"
#include "netsender.h"

using namespace std;
using namespace com::simplecast::protocol;

class clientproc : public dataProcBase
{
	public:
		clientproc(NetSender* pSender);
		virtual ~clientproc();
		virtual void UserProc(string& strInfo, SOCKETINFO* sockinfo);

		void send_open_url(string strurl);
		void send_close();

	protected:
		static MediaType getMediaType(string strfile);
	protected:
		string m_strUrl;
};

