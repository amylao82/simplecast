
#include <string>
#include "netsender.h"
#include "clientproc.h"

#include "config.h"


int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("Usage : %s [open  media_url | close]\n", argv[0]);
		return -1;
	}

	string str = argv[1];

	cast_config cfg("config.json");

	//TODO: 要实现往多个服务器发送数据,需要增加一步,查询可用服务器,并指定使用哪一个服务器.
	//shared_ptr<NetSender> pSender = createSender(NetSender::PROTOCOL_UDP, NetSender::TYPE_SERVER, "10.66.30.46", 8003);
	//shared_ptr<NetSender> pSender = createSender(NetSender::PROTOCOL_UDP, NetSender::TYPE_SERVER, "", 8003);
	//shared_ptr<clientproc> pProc = shared_ptr<clientproc>(new clientproc(pSender.get()));

	//NetSender* pSender = createSender(NetSender::PROTOCOL_UDP, NetSender::TYPE_SERVER, "", 8003);
	NetSender* pSender = createSender(NetSender::PROTOCOL_UDP, NetSender::TYPE_CLIENT, cfg.get_server(), cfg.get_port());
	if(pSender == NULL)
	{
	    printf("netsender create error!\n");
	    return 0;
	}

	clientproc* pProc = new clientproc(pSender);

	if(str == "open")
	{
		pProc->send_open_url(argv[2]);
	}
	else if(str == "close")
	{
		pProc->send_close();
	}
	else
	{
		printf("not know command\n");
	}


	delete pProc;
	delete pSender;

	return 0;
}

