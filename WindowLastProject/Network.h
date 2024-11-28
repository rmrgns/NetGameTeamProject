#pragma once
#include <thread>
#include "../server/NetGameServer/Common.h"
#include "Page.h"
#include "Game.h"
#include "HeapDebug.h"
#include <cstring>

static CRITICAL_SECTION cs;

static HANDLE hNetworkLoopThread;

class Network
{
private:
	static Network* m_pInst;
public:
	static Network* GetInst()
	{
		if (!m_pInst)
			m_pInst = new Network;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete m_pInst;
			m_pInst = NULL;
		}
	}

private:
	SOCKET sock;
	HANDLE hd;
	// 클라에서 전달한 sendList
	sendList processSendList;
	TitlePage* TitleTemp;
	EditStation* EditTemp;

	std::thread worker;

	int m_id;
	int m_prev_size = 0;
	bool m_iswork = true;
	const char* SERVERIP = (char*)"127.0.0.1";
	int retval;
	int len;
	int SERVERPORT = 9000;
	int BUFSIZE = 1024;
public:
	Network() {};
	~Network() {
		DeleteCriticalSection(&cs);
		closesocket(sock);
		WSACleanup();

	};

	bool Init();
	bool Connect();
	void Update();	// 서버에서 보내는 데이터 전송받는 함수

public:
	//서버와 클라간의 중계함수

	void SendCheckLoginAndMusicDownload(string id, string password);
	void ProcessCheckLoginAndMusicDownload();
	void SendRequestPlayerScore();
	void ProcessRequestPlayerScore();
	void SendEnterEditStation(TitlePage* tp);
	void ProcessEnterEditStation();
	void SendLeaveEditStation(EditStation* es);
	void ProcessLeaveEditStation();
	// 메서드

};

