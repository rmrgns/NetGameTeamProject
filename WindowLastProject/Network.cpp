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
	hNetworkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

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
	char buf[256];

	// 데이터 받기
	retval = recv(sock, buf, retval, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return;
	}
	else if (retval == 0)
		return;

	if (sl == sendList::CheckLogin)
	{

		ResetEvent(hNetworkEvent);
	}
	else
	{
		return;
	}
}


void Network::SendCheckLoginAndMusicDownload(string id, string password)
{
	int retval;
	unsigned long len;
	
	//sendList sl = sendList::CheckLogin;	
	//len = sizeof(sl);
	 
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
	SetEvent(hNetworkEvent);
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

