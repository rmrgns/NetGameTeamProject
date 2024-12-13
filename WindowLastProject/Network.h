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
	TitlePage* TitleTemp = nullptr;
	EditStation* EditTemp = nullptr;
	PlayStation* PlayTemp = nullptr;
	LobbyInfo* LobbyTemp = nullptr;

	int m_id;
	int m_prev_size = 0;
	bool m_iswork = true;
	unsigned short m_index = 0;
	unsigned int m_score = 0;

	unsigned short index2 = 0;
	unsigned int score2 = 0;


	//const char* SERVERIP = (char*)"127.0.0.1";
	//const char* SERVERIP = (char*)"192.168.0.2";
	const char* SERVERIP = (char*)"10.20.11.22";
	int retval;
	unsigned long len;
	int SERVERPORT = 9000;

	string cmd = "None";
	
public:
	//char temp = 'z';
	// �뷡 �̸��� ��Ʈ���� �̸��� �����ϴ� ����
	vector<PageMusicData> musicDataSet;
	int musicIndex = 0;
	int temp = 0;
	Network() {};
	~Network() {
		//DeleteCriticalSection(&cs);
		closesocket(sock);
		WSACleanup();

	};

	bool Init();
	bool Connect();
	void SendUpdate();
	void Update();	// �������� ������ ������ ���۹޴� �Լ�

	void SetPlayStation(PlayStation* ps) { PlayTemp = ps; }

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

	// PlayStation ?�수
	void SendEnterPlayStation(TitlePage* go);
	void ProcessEnterPlayStation();
	void SendLeavePlayStation(PlayStation* go);
	void ProcessLeavePlayStation();

	// PlayerScore
	void SendPlayerScore(unsigned int score);

	// Lobby func
	void SendEnterLobbyAndInfo(TitlePage* go);
	void ProcessEnterLobbyAndInfo();

	void SendReadyStatus();

	void setCommand(string s) { cmd = s; }

	//EditStation func
	void SendUploadMusic(string music, string pattern);
	void SendFile(string filename);
};

