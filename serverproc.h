
#pragma once

#include <map>
#include <vector>
#include <thread>

#include "message.pb.h"
#include "netsender.h"
#include "dataprocbase.h"

using namespace std;
using namespace com::simplecast::protocol;

class serverproc: public dataProcBase
{
    public:
	serverproc(NetSender* pSender);
	virtual ~serverproc();

	virtual void UserProc(string& strInfo, SOCKETINFO* sockinfo);

    protected:
	void sendMsg(SimplecastMsg& msg, SOCKETINFO* sockinfo);

	void* thread_play_proc();

	static void* thread_play(void* arg);

    protected:
	//打开播放器.
	bool open_player();
	//关闭之前的播放器
	void close_player();
    protected:

	pthread_t m_pidPlay;

	string m_strUrl;
	MediaType m_media_type;
	bool m_b_playing;	//结束播放.
};

