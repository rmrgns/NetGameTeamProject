#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

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
	if(cmd == 'S')
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
	// 파일 이름 텍스트 크기 받기(고정 길이)
	for (int i{}; i < 11; i++)
	{
		retval = recv(sock, (char*)&len, sizeof(unsigned int), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvnamesize()");
			return;
		}
		cout << len << endl;

		// 파일 이름 텍스트 받기(가변 길이)

		retval = recv(sock, buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvname()");
			return;
		}

		buf[retval] = '\0';
		strcpy(name, buf);
		FILE* recvFile = fopen(name, "wb"); // 전송받을 파일 선택(없으면 생성)
		if (recvFile == NULL) {
			printf("파일 열기 불가\n");
			return;
		}
		cout << name << endl;
		// 파일 데이터 크기 받기(고정 길이)
		retval = recv(sock, (char*)&len, sizeof(unsigned long), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvfilesize()");
			return;
		}
		cout << len << endl;

		// 파일 데이터 받기(가변 길이)      
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
		fclose(recvFile);  // 파일 닫기
	}
}

void Network::SendRequestPlayerScore()
{

}

void Network::ProcessRequestPlayerScore()
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
		// PlayerStation 입장
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

//void Network::SendEnterEditStation()
//{
//	int retval;
//	unsigned long len;
//
//	string sl = "EnterEditStation";
//	len = sl.length();
//
//	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
//	if (retval == SOCKET_ERROR) {
//		err_display("SendEnterEditStation() Size");
//	}
//
//	retval = send(sock, sl.c_str(), len, 0);
//	if (retval == SOCKET_ERROR) {
//		err_display("SendEnterEditStation()");
//	}
//}
//
//void Network::ProcessEnterEditStation()
//{
//
//}
