#pragma once

#include "../server/NetGameServer/Common.h"
#include "../server/NetGameServer/packet.h"
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
	// Ŭ�󿡼� ������ sendList
	sendList processSendList;
	TitlePage* TitleTemp;
	EditStation* EditTemp;
	PlayStation* PlayTemp;

	int m_id;
	int m_prev_size = 0;
	bool m_iswork = true;
	int m_index = 1;

	const char* SERVERIP = (char*)"127.0.0.1";
	int retval;
	unsigned int len;
	int SERVERPORT = 9000;
	int BUFSIZE = 1024;

	string cmd = " ";

public:
	// �뷡 �̸��� ��Ʈ���� �̸��� �����ϴ� ����
	vector<PageMusicData> musicDataSet;
	int musicIndex = 0;
	int temp = 0;
	Network() {};
	~Network() {
		DeleteCriticalSection(&cs);
		closesocket(sock);
		WSACleanup();

	};

	bool Init();
	bool Connect();
	void SendUpdate();
	void Update();	// �������� ������ ������ ���۹޴� �Լ�

public:
	//������ Ŭ���� �߰��Լ�
	// ������ ���ɾ� ������ �Լ�
	void SendCommand(string cmd);

	void SendCheckLoginAndMusicDownload(string id, string password);
	void ProcessCheckLoginAndMusicDownload();
	void SendRequestPlayerScore();
	void ProcessRequestPlayerScore();
	void SendEnterEditStation(TitlePage* tp);
	void ProcessEnterEditStation();
	void SendLeaveEditStation(EditStation* es);
	void ProcessLeaveEditStation();

	// PlayStation 함수
	void SendEnterPlayStation(TitlePage* go);
	void ProcessEnterPlayStation();
	void SendLeavePlayStation(PlayStation* go);
	void ProcessLeavePlayStation();

	// Player�� ������ ������ ������Ʈ�ϴ� �Լ�
	void SendPlayerScore(unsigned int score);

	// �޼���
	void setCommand(string s) { cmd = s; }
};

