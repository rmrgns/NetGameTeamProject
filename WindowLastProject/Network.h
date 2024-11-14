#pragma once
#include <thread>
#include "../server/NetGameServer/Common.h"
#include "main.h"

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

	std::thread worker;

	int m_id;
	int m_prev_size = 0;
	bool m_iswork = true;
	const char* SERVERIP = (char*)"127.0.0.1";

	int SERVERPORT = 9000;
	int BUFSIZE = 1024;
public:
	Network() {};
	~Network() {
		closesocket(sock);
		WSACleanup();

	};

	bool Init();
	bool Connect();


public:
	//서버와 클라간의 중계함수

	void SendCheckLoginAndMusicDownload(string id, string password);
	void ProcessCheckLoginAndMusicDownload();
	void SendRequestPlayerScore();
	void ProcessRequestPlayerScore();


};

