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
		// 파일 이름 텍스트 크기 받기(고정 길이)
		retval = recv(sock, (char*)&len, sizeof(unsigned long), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvnamesize()");
			return;
		}
		else if (retval == 0)
			return;

		// 파일 이름 텍스트 받기(가변 길이)

		retval = recv(sock, buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvname()");
			return;
		}
		else if (retval == 0)
			return;

		buf[retval] = '\0';
		strcpy(name, buf);
		FILE* recvFile = fopen(buf, "wb"); // 전송받을 파일 선택(없으면 생성)
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
		else if (retval == 0)
			return;

		// 파일 데이터 받기(가변 길이)      
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
	int retval;
	unsigned long len;

	string sl = "EnterPlayStation";
	len = sl.length();

	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendEnterPlayStation() Size");
	}

	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendEnterPlayStation()");
	}
	tp = go;
	processSendList = sendList::EnterPlayStation;
}

void Network::ProcessEnterPlayStation()
{
	int retval;
	unsigned char check;

	retval = recv(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("ProcessEnterPlayStation");
	}

	if (check == 'p')
	{
		// PlayerStation 입장
		tp->setSelectCommand(check);
		tp->NextPage();
		//cout << check << endl;
	}
}

//void Network::SendPlayerScore(unsigned int score)
//{
//	int retval;
//
//	retval = send(sock, (char*)&score, sizeof(unsigned int), 0);
//	if (retval == SOCKET_ERROR) {
//		err_display("SendPlayerScore()");
//	}
//
//	
//}

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
