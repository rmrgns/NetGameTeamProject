#include "Network.h"
#include "Page.h"


Network* Network::m_pInst = NULL;


bool Network::Init()
{
	m_id = 0;
	WSADATA WSAData;

	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
		return false;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		err_quit("socket()");
		return false;
	}
	if (INVALID_SOCKET == sock)
	{
		err_display(WSAGetLastError());
		return false;
	}
	InitializeCriticalSection(&cs);
	hNetworkEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	return true;
}

bool Network::Connect()
{
	int retval;

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("connect()");
		return false;
	}
	return true;
}

void Network::Update()
{
	int retval = 0;
	int len;
	char buf[256];
	if (processSendList == sendList::CheckLogin)
	{
		// 데이터 받기
		retval = recv(sock, (char*)&len, sizeof(unsigned long), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvnamesize()");
			cout << "error1" << endl;
			LeaveCriticalSection(&cs);
			return;
		}
		else if (retval == 0)
		{
			cout << "error2" << endl;
			LeaveCriticalSection(&cs);
			return;
		}

		retval = recv(sock, buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvsendlist()");
			LeaveCriticalSection(&cs);
			return;
		}
		else if (retval == 0)
		{
			LeaveCriticalSection(&cs);
			return;
		}
		buf[retval] = '\0';
		cout << buf << endl;
		processSendList = sendList::None;
	}
}


void Network::SendCheckLoginAndMusicDownload(string id, string password)
{
	
	int retval;
	int len;
	string sl = "CheckLogin";
	len = sl.length();
	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload() Size");
	}

	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload()");
	}
	//EnterCriticalSection(&cs);
	//SetEvent(hNetworkEvent);
	//LeaveCriticalSection(&cs);
	processSendList = sendList::CheckLogin;
}

void Network::ProcessCheckLoginAndMusicDownload()
{
	
}

void Network::SendRequestPlayerScore()
{

}

void Network::ProcessRequestPlayerScore()
{

}

