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
}

void Network::ProcessCheckLoginAndMusicDownload()
{
	
}

void Network::SendRequestPlayerScore()
{
	int retval;
	unsigned long len;

	string sl = "PlayerScore";
	len = sl.length();

	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendRequestPlayerScore() Size");
	}

	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendRequestPlayerScore()");
	}
}

void Network::ProcessRequestPlayerScore()
{
	
}


void Network::SendLeaveEditStation()
{
	int retval;
	unsigned long len;

	string sl = "LeaveEditStation";
	len = sl.length();

	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendLeaveEditStation() Size");
	}

	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendLeaveEditStation()");
	}
}

void Network::ProcessLeaveEditStation()
{
	int retval;
	unsigned long len;

	len = sizeof(bool);

	retval = recv(sock, (char*)&len, sizeof(unsigned long), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("ProcessLeaveEditStation()");
	}
}

void Network::SendEnterEditStation()
{
	int retval;
	unsigned long len;

	string sl = "EnterEditStation";
	len = sl.length();

	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendEnterEditStation() Size");
	}

	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendEnterEditStation()");
	}
}

void Network::ProcessEnterEditStation()
{

}