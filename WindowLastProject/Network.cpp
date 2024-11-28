#define _CRT_SECURE_NO_WARNINGS // ??? C ??? ?? ?? ?? ???
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ??? ??? API ?? ?? ?? ???

#include "Network.h"



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

void Network::SendUpdate()
{
	//if(cmd == 'S')
}

void Network::Update()
{

	char buf[256];
	if (processSendList == sendList::CheckLogin)
	{
		ProcessCheckLoginAndMusicDownload();
	}
	else if (processSendList == sendList::EnterPlayStation)
	{
		ProcessEnterPlayStation();
	}
	else
	{
		return;
	}
	processSendList = sendList::None;
}


void Network::SendCommand(string cmd)
{
	string sl = cmd;
	len = sl.length();

	ThrottlePackets();
	retval = send(sock, (char*)&len, sizeof(unsigned int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCommand() Size");
	}

	ThrottlePackets();
	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCommand()");
	}
}

void Network::SendCheckLoginAndMusicDownload(string id, string password)
{
	string sl = "CheckLogin";
	SendCommand(sl);
	processSendList = sendList::CheckLogin;
}

void Network::ProcessCheckLoginAndMusicDownload()
{
	char buf[BUFSIZ];
	char name[BUFSIZ + 1];

	unsigned int size = 0;
	

	/*retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload()");
	}*/
	//for (int i{}; i < 12; i++)

	/*retval = send(sock, (char*)&temp, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload()");
	}*/
	//len = 0;
	// ??? ?̸? ???Ʈ ũ?? ?ޱ?(??? ???)
	for (int i{}; i < 11; i++)
	{
		retval = recv(sock, (char*)&len, sizeof(unsigned int), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvnamesize()");
			return;
		}
		cout << len << endl;

		// ??? ?̸? ???Ʈ ?ޱ?(???? ???)

		retval = recv(sock, buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvname()");
			return;
		}

		buf[retval] = '\0';
		strcpy(name, buf);
		FILE* recvFile = fopen(name, "wb"); // ???۹?? ??? ???(?????? ????)
		if (recvFile == NULL) {
			printf("??? ???? ?Ұ?\n");
			return;
		}
		cout << name << endl;
		// ??? ????? ũ?? ?ޱ?(??? ???)
		retval = recv(sock, (char*)&len, sizeof(unsigned long), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvfilesize()");
			return;
		}
		cout << len << endl;

		// ??? ????? ?ޱ?(???? ???)      
		unsigned long totalBytesReceived = 0;

		while (totalBytesReceived < len) {
			retval = recv(sock, buf, BUFSIZ, MSG_WAITALL);

			if (retval == SOCKET_ERROR) {
				err_display("recvfile()");
				return;
			}

			fwrite(buf, 1, retval, recvFile);
			totalBytesReceived += retval;
		}
		cout << "successNT" << endl;
		fclose(recvFile);  // ??? ?ݱ?
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

}

void Network::SendEnterPlayStation(TitlePage* go)
{
	string sl = "EnterPlayStation";
	SendCommand(sl);
	tp = go;
	processSendList = sendList::EnterPlayStation;
}

void Network::ProcessEnterPlayStation()
{
	unsigned char check;

	retval = recv(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("ProcessEnterPlayStation");
	}

	if (check == 'p')
	{
		// PlayerStation ???
		//tp->setSelectCommand(check);
		tp->Select(check);
		cout << check << endl;
	}
}

void Network::SendPlayerScore(unsigned int score)
{
	string sl = "SendPlayerScore";
	SendCommand(sl);
	PlayerScorePacket p;
	p.index = m_index;
	p.score = score;

	ThrottlePackets();
	retval = send(sock, (char*)&p, sizeof(PlayerScorePacket), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendPlayerScore() score");
	}

	processSendList = sendList::PlayerScore;

}
