
#include <sys/wait.h>
//#include <stdio.h>

#include "serverproc.h"
#include "message.pb.h"

//2022.08.25.使用创建子线程,再在子线程里fork子进程,调用外部播放器的方法,实现播放再监听网络二不误.
//

serverproc::serverproc(NetSender* pSender)
    :dataProcBase(pSender)
    ,m_pidPlay(-1)
     ,m_b_playing(false)
{
}

serverproc::~serverproc()
{
    printf("~serverproc\n");
}

void serverproc::UserProc(string& strInfo, SOCKETINFO* sockinfo)
{
    SimplecastMsg msg;
    int i;

    printf("test\n");
    printf("\n\nserverproc::UserProc: %s, len = %d\n", strInfo.c_str(), strInfo.length());

    bool bRet;

    bRet = msg.ParseFromString(strInfo);


    switch(msg.type())
    {
	case RPC_TYPE_SET_URL:
	    {
		printf("m_b_playing = %d\n", m_b_playing);
		//当前正在播放状态,先结束.
		if(m_b_playing)
		{
		    close_player();
		}

		msgSetUrl msgurl;
		msgurl.ParseFromString(msg.request());
		m_strUrl = msgurl.url();
		m_media_type = msgurl.type();

		printf("url = %s\n", m_strUrl.c_str());

		open_player();

	    }
	    break;
	case RPC_TYPE_CLOSE:

	    printf("receive quit message\n\n");
	    printf("===================\n\n");

	    close_player();

	    break;
    }
    printf("=========end switch\n");
}

//void* serverproc::thread_play_proc()
//{
//    printf("thread_play_proc==type = %d\n", m_media_type);
//    //创建子进程.
//    if(0 == fork())
//    {
//	string strcmd;
//	switch(m_media_type)
//	{
//	    case TYPE_IMAGE:
//		strcmd = "./show.sh " + m_strUrl;
//		break;
//	    case TYPE_MUSIC:
//		strcmd = "./playaudio.sh " + m_strUrl;
//		break;
//	    case TYPE_VIDEO:
//	    default:
//		strcmd = "./play.sh " + m_strUrl;
//		break;
//	}
//
//	system(strcmd.c_str());
//
//	printf("end play subfork\n");
//    }
//    else
//    {
//	wait(NULL);
//	//线程结束,等待下一次传过来的Url
//	m_strUrl = "";
//	m_pidPlay = -1;
//	m_b_playing = false;
//	printf("end play main\n");
//    }
//    return NULL;
//}

//使用一种更简单的方法,不用fork
void* serverproc::thread_play_proc()
{
    printf("thread_play_proc==type = %d\n", m_media_type);
    //创建子进程.
//    if(0 == fork())
//    {
	string strcmd;
	switch(m_media_type)
	{
	    case TYPE_IMAGE:
		strcmd = "./show.sh " + m_strUrl;
		break;
	    case TYPE_MUSIC:
		strcmd = "./playaudio.sh " + m_strUrl;
		break;
	    case TYPE_VIDEO:
	    default:
		strcmd = "./play.sh " + m_strUrl;
		break;
	}

	system(strcmd.c_str());

	printf("end play subfork\n");
	m_strUrl = "";
	m_b_playing = false;
	m_pidPlay = -1;
//    }
//    else
//    {
//	wait(NULL);
//	//线程结束,等待下一次传过来的Url
//	m_strUrl = "";
//	m_pidPlay = -1;
//	m_b_playing = false;
//	printf("end play main\n");
//    }
    return NULL;
}

void* serverproc::thread_play(void* arg)
{
    serverproc* p = (serverproc*)arg;
    return p->thread_play_proc();
}

void serverproc::sendMsg(SimplecastMsg& msg, SOCKETINFO* sockinfo)
{
    string strSend;
    msg.SerializeToString(&strSend);
    //发出回应包.
    send_msg(strSend, sockinfo);
    //	m_pSender->sendBuf(strSend, sockinfo);
}


bool serverproc::open_player()
{
    if(m_pidPlay == -1)
    {
	pthread_create(&m_pidPlay, NULL, &thread_play, this);
	m_b_playing = true;
    }

    return true;
}

void serverproc::close_player()
{
    if(m_strUrl.length() != 0)
    {
	string strcmd =  "./close.sh " + m_strUrl;
	printf("%s\n,", strcmd.c_str());
	system(strcmd.c_str());
	m_strUrl = "";
	//wait(NULL);
    }
    if(m_pidPlay != -1)
    {
	pthread_join(m_pidPlay, NULL);
	m_pidPlay = -1;
    }
}

