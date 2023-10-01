#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <csignal>
#include <thread>

#include "message.pb.h"
#include "serverproc.h"
#include "config.h"

using namespace std;

int keepRunning = 1;

void sig_handler( int sig )
{
	if ( sig == SIGINT)
	{
		keepRunning = 0;
	}
}

int main(int argc, char* argv[])
{
	signal( SIGINT, sig_handler);

	cast_config cfg("config.json");

	//shared_ptr<NetSender> pSender = createSender(NetSender::PROTOCOL_UDP, NetSender::TYPE_SERVER, "", 8003);
	//shared_ptr<serverproc> pProc = shared_ptr<serverproc>(new serverproc(pSender.get()));

	NetSender* pSender = createSender(NetSender::PROTOCOL_UDP, NetSender::TYPE_SERVER, "", cfg.get_port());
	serverproc* pProc = new serverproc(pSender);


	while(keepRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	delete pProc;
	delete pSender;

	return 1;
}

