
#include "clientproc.h"

typedef struct _suffix_type_item {
	string suffix;
	MediaType type;
} suffix_type_item;

const vector<suffix_type_item> suffix_type = {
	{"mkv", TYPE_VIDEO },
	{"mp4", TYPE_VIDEO },
	{"mp3", TYPE_MUSIC },
	{"aac", TYPE_MUSIC },
	{"jpg", TYPE_IMAGE},
	{"jpeg", TYPE_IMAGE},
	{"png", TYPE_IMAGE},
};


clientproc::clientproc(NetSender* pSender)
	:dataProcBase(pSender)
	 ,m_strUrl("")
{
}

clientproc::~clientproc()
{
}

void clientproc::UserProc(string& strInfo, SOCKETINFO* sockinfo)
{
	//TODO: 如果要实现扫描网内可用的服务器,需要在这里接收数据
}

void clientproc::send_open_url(string strurl)
{
	printf("send_open_url(%s)\n", strurl.c_str());
	string strMsg;
	msgSetUrl msgurl;
	string strSetUrl;
	//msgurl.set_url("http://10.66.30.46/video/DrunkenMaster2.mkv");
	msgurl.set_url(strurl);
	msgurl.set_type(clientproc::getMediaType(strurl));
	msgurl.SerializeToString(&strSetUrl);

	SimplecastMsg msg;
	msg.set_type(RPC_TYPE_SET_URL);
	msg.set_request(strSetUrl);

	msg.SerializeToString(&strMsg);

	send_msg(strMsg);
}

void clientproc::send_close()
{
	string strMsg;
	SimplecastMsg msg;
	msg.set_type(RPC_TYPE_CLOSE);
	msg.SerializeToString(&strMsg);

	send_msg(strMsg);
}

MediaType clientproc::getMediaType(string strfile)
{
	int pos = strfile.rfind(".");
	//没找到后缀,默认认为是VIDEO格式.
	if(pos == string::npos)
		return TYPE_VIDEO;

	string strsuffix = strfile.substr(pos+1, strfile.length());

	transform(strsuffix.begin(),strsuffix.end(),strsuffix.begin(),::tolower);

	vector<suffix_type_item>::const_iterator it;
	for(it = suffix_type.begin(); it != suffix_type.end(); ++it)
	{
		if( (*it).suffix == strsuffix)
		{
			return (*it).type;
		}
	}

	return TYPE_VIDEO;
}

