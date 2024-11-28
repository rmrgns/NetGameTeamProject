#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include "Network.h"
#include "Page.h"
#include <cstring>


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

	return true;
}

bool Network::Connect()
{

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

	char buf[256];
	if (processSendList == sendList::CheckLogin)
	{
		ProcessCheckLoginAndMusicDownload();
	}
	else if (processSendList == sendList::EnterEditStation)
	{
		ProcessEnterEditStation();
	}
	else if (processSendList == sendList::LeaveEditStation)
	{
		ProcessLeaveEditStation();
	}
	else
	{
		return;
	}
	processSendList = sendList::None;
}


void Network::SendCheckLoginAndMusicDownload(string id, string password)
{

	string sl = "CheckLogin";
	len = sl.length();
	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload() Size");
		return;
	}

	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload()");
		return;
	}

	processSendList = sendList::CheckLogin;
}

void Network::ProcessCheckLoginAndMusicDownload()
{
	char buf[1024];
	char name[1024 + 1];

	unsigned int size = 0;
	
	/*retval = recv(sock, (char*)&size, sizeof(unsigned int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload() Size");
		return;
	}*/

	/*retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload()");
	}*/
	for (int i{}; i < 12; i++)
	{
		// ���� �̸� �ؽ�Ʈ ũ�� �ޱ�(���� ����)
		retval = recv(sock, (char*)&len, sizeof(unsigned long), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvnamesize()");
			return;
		}
		else if (retval == 0)
			return;

		// ���� �̸� �ؽ�Ʈ �ޱ�(���� ����)

		retval = recv(sock, buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvname()");
			return;
		}
		else if (retval == 0)
			return;

		buf[retval] = '\0';
		strcpy(name, buf);
		FILE* recvFile = fopen(buf, "wb"); // ���۹��� ���� ����(������ ����)
		if (recvFile == NULL) {
			printf("���� ���� �Ұ�\n");
			return;
		}
		cout << name << endl;
		// ���� ������ ũ�� �ޱ�(���� ����)
		retval = recv(sock, (char*)&len, sizeof(unsigned long), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvfilesize()");
			return;
		}
		else if (retval == 0)
			return;

		// ���� ������ �ޱ�(���� ����)      
		unsigned long totalBytesReceived = 0;

		while (totalBytesReceived < len) {
			retval = recv(sock, buf, BUFSIZE, MSG_WAITALL);

			if (retval == SOCKET_ERROR) {
				err_display("recvfile()");
				return;
			}

			fwrite(buf, 1, retval, recvFile);
			totalBytesReceived += retval;
		}

		fclose(recvFile);  // ���� �ݱ�
	}
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


void Network::SendLeaveEditStation(EditStation* es)
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
	EditTemp = es;
	processSendList = sendList::LeaveEditStation;
}

void Network::ProcessLeaveEditStation()
{
	int retval;
	bool check = FALSE;

	retval = recv(sock, (char*)&check, sizeof(bool), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recvnamesize()");
		return;
	}
	else if (retval == 0)
		return;


	if (check == TRUE) {
		EditTemp->LeaveEditStation();
	}
}

void Network::SendEnterEditStation(TitlePage* tp)
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
	TitleTemp = tp;
	processSendList = sendList::EnterEditStation;
}

void Network::ProcessEnterEditStation()
{
	int retval;
	bool check;

	retval = recv(sock, (char*)&check, sizeof(bool), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recvnamesize()");
		return;
	}
	else if (retval == 0)
		return;


	if (check == TRUE) {
		TitleTemp->EnterEditStation();
	}
}