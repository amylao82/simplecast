
#include "netsender.h"
#include "udpsender.h"

//shared_ptr<NetSender> createSender(NetSender::PROTOCOL_TYPE protocol, NetSender::NETSENDER_TYPE type, string connectServer, int port)
NetSender* createSender(NetSender::PROTOCOL_TYPE protocol, NetSender::NETSENDER_TYPE type, string connectServer, int port)
{
	switch(protocol)
	{
		case NetSender::PROTOCOL_UDP:
			{
				udpSender* pSender = new udpSender(type, port);
				if(type == NetSender::TYPE_SERVER)
					pSender->initServer();
				else
					pSender->connectServer(connectServer);

				return pSender;
			}
		case NetSender::PROTOCOL_TCP:
		case NetSender::PROTOCOL_WEBSOCKET:
		case NetSender::PROTOCOL_HTTP:
		default:
			//TODO: add new protocol,not support now.
			return NULL;
	}
}

