#define _CRT_SECURE_NO_WARNINGS // ??? C ??? ?? ?? ?? ???
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ??? ??? API ?? ?? ?? ???
#define BUFSIZE 4096

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
	//cout << cmd << endl;
	if (cmd == "SendPlayerScore")
	{
		string sl = "SendPlayerScore";
		SendCommand(sl);
		PlayerScorePacket p;
		p.index = m_index;
		p.score = m_score;

		ThrottlePackets();
		retval = send(sock, (char*)&p, sizeof(PlayerScorePacket), 0);
		if (retval == SOCKET_ERROR) {
			err_display("SendPlayerScore() score");
		}
		PlayerScorePacket p2;
		ThrottlePackets();
		retval = recv(sock, (char*)&p2, sizeof(PlayerScorePacket), 0);
		if (retval == SOCKET_ERROR) {
			err_display("SendPlayerScore() score");
		}

		if (m_index == p2.index)
		{
			m_score = p2.score;
		}
		else
		{
			index2 = p2.index;
			score2 = p2.score;
		}
		PlayerScorePacket p3;
		ThrottlePackets();
		retval = recv(sock, (char*)&p3, sizeof(PlayerScorePacket), 0);
		if (retval == SOCKET_ERROR) {
			err_display("SendPlayerScore() score");
		}

		if (m_index == p3.index)
		{
			m_score = p3.score;
		}
		else
		{
			index2 = p3.index;
			score2 = p3.score;
		}
		cout << "index: " << m_index << ", score: " << m_score << endl;

		PlayTemp->SetScores(m_index, m_score, index2, score2);
		//cout << p.score << ": " <<m_score << endl;
		//processSendList = sendList::PlayerScore;
	}
}

void Network::Update()
{
	/*if (temp == 'z')
	{
		string a ="a";
		string b ="b";
		SendCheckLoginAndMusicDownload(a,b);
	}*/
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
	else if (processSendList == sendList::EnterPlayStation)
	{
		ProcessEnterPlayStation();
	}
	else if (processSendList == sendList::LeavePlayStation)
	{
		ProcessLeavePlayStation();
	}
	else if (processSendList == sendList::EnterLobby)
	{
		ProcessEnterLobbyAndInfo();
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
	//temp = 'x';
	string sl = "CheckLogin";
	SendCommand(sl);
	
	processSendList = sendList::CheckLogin;
}

void Network::ProcessCheckLoginAndMusicDownload()
{
	char buf[BUFSIZE + 1] = {0};
	char name[BUFSIZE + 1];

	unsigned int size = 0;
	/*string sl;

	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload()");
	}*/
	//for (int i{}; i < 12; i++)

	/*retval = send(sock, (char*)&temp, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload()");
	}*/
	//len = 0;
	// ??? ?見? ???트 크?? ?檳?(??? ???)
	for (int i{}; i < 11; i++)
	{
		// file name size recv
		ThrottlePackets();
		retval = recv(sock, (char*)&len, sizeof(unsigned long), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvnamesize()");
			return;
		}
		cout << len << endl;

		// file name recv
		ThrottlePackets();
		retval = recv(sock, buf, len, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvname()");
			return;
		}

		buf[retval] = '\0';
		strcpy(name, buf);
		FILE* recvFile = fopen(name, "wb"); // ???膀?? ??? ???(?????? ????)
		if (recvFile == NULL) {
			printf("file open error\n");
			return;
		}
		cout << name << endl;

		// file size recv
		ThrottlePackets();
		retval = recv(sock, (char*)&len, sizeof(unsigned long), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recvfilesize()");
			return;
		}
		cout << len << endl;

		// file rev    
		unsigned long totalBytesReceived = 0;

		while (totalBytesReceived < len) {
			ThrottlePackets();
			retval = recv(sock, buf, BUFSIZE, MSG_WAITALL);

			if (retval == SOCKET_ERROR) {
				err_display("recvfile()");
				return;
			}

			fwrite(buf, 1, retval, recvFile);
			totalBytesReceived += retval;

			//printf("\033[%d;1H", 1);  // ?대씪?댁뼵??ID???곕씪 ?ㅻⅨ 以꾨줈 ?대룞
			//printf("[?대씪?댁뼵??%d] 吏꾪뻾?? %d%% / ?꾩껜 ?곗씠???ш린: %ld, ?꾩옱 諛쏆? ?ш린: %ld\n",
			//	1,
			//	(int)(((float)totalBytesReceived / (float)len) * 100.f),
			//	len, totalBytesReceived);
		}
		cout << "successNT" << endl;
		fclose(recvFile);  // ??? ?膚?
	}

}

void Network::SendRequestPlayerScore()
{
	int retval;
	unsigned long len;

	string sl = "PlayerScore";
	len = sl.length();

	ThrottlePackets();
	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendRequestPlayerScore() Size");
	}

	ThrottlePackets();
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
void Network::SendEnterPlayStation(TitlePage* go)
{
	string sl = "EnterPlayStation";
	SendCommand(sl);

	char isReady = 'p';

	ThrottlePackets();
	retval = send(sock, (char*)&isReady, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("EnterPlayStation");
	}

	ThrottlePackets();
	retval = recv(sock, (char*)&m_index, sizeof(unsigned short), 0);
	if (retval == SOCKET_ERROR) {
		err_display("EnterPlayStation");
	}

	cout << "index: " << m_index << endl;

	TitleTemp = go;
	processSendList = sendList::EnterPlayStation;
}

void Network::ProcessEnterPlayStation()
{
	unsigned char check;

	ThrottlePackets();
	retval = recv(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("ProcessEnterPlayStation");
	}

	if (check == 'p')
	{
		// PlayerStation ???
		//tp->setSelectCommand(check);
		TitleTemp->Select(check);
		cmd = "EnterPlayStation";
		//cout << check << endl;
	}
}

void Network::SendLeavePlayStation(PlayStation* go)
{
	string sl = "LeavePlayStation";
	SendCommand(sl);
	PlayTemp = go;
	processSendList = sendList::LeavePlayStation;
}

void Network::ProcessLeavePlayStation()
{
	unsigned char check;

	ThrottlePackets();
	retval = recv(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("ProcessLeavePlayStation");
	}
	cout << check << endl;
	if (check == '4')
	{
		PlayTemp->LeavePlayStation();
		cmd = "None";
	}
}

void Network::SendPlayerScore(unsigned int score)
{
	m_score = score;
	if(cmd == "EnterPlayStation")
		cmd = "SendPlayerScore";
	
}

void Network::SendEnterLobbyAndInfo(TitlePage* go)
{
	string sl = "EnterLobbyAndInfo";
	SendCommand(sl);
	TitleTemp = go;

	string id = TitleTemp->getUserID();
	len = id.length();
	// send file name size
	ThrottlePackets();
	retval = send(sock, (char*)&len, sizeof(unsigned int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvIDSIzeAtEnterLobby");
		return;
	}
	cout << len << endl;


	// send file name
	ThrottlePackets();
	retval = send(sock, id.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvIDAtEnterLobby");
		return;
	}
	processSendList = sendList::EnterLobby;
}

void Network::ProcessEnterLobbyAndInfo()
{
	unsigned char check;

	ThrottlePackets();
	retval = recv(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("ProcessEnterLobbyAndInfo");
	}

	cout << check << endl;

	if (check == '5')
	{
		TitleTemp->Select('l');
		cout << "success" << endl;
		cmd = "None";
	}
}

void Network::SendUploadMusic(string music, string pattern)
{
	cout << music << endl;
	SendFile(music);
	string ptName = pattern + ".txt";	
	cout << ptName << endl;
	SendFile(ptName);
	
}

void Network::SendFile(string fn)
{
	string sl = "UploadMusic";
	SendCommand(sl);

	//string temp = fn;
	//string find_str = "\\";
	//string replace_str = "\\\\";

	//temp.replace(temp.find(find_str), find_str.length(), replace_str);

	char* filename = const_cast<char*>(fn.c_str());
	//cout << filename << endl;

	// ������ ������ ���� ����
	FILE* send_file = fopen(filename, "rb");
	if (send_file == NULL) {
		printf("file open error : %d, %s\n", errno, filename);
		return;
	}

	char buf[BUFSIZE];

	// ���� ũ��
	unsigned long fileSize;
	fseek(send_file, 0, SEEK_END);	// ���� �����͸� ���� ������ �̵�
	fileSize = ftell(send_file);	// ���� ���� ������ ��ġ�� ���� (���� ũ��)
	rewind(send_file);				// ���� �����͸� �ٽ� ������ �������� ����

	//len = (int)strlen(filename);
	unsigned long len = static_cast<unsigned long>(strlen(filename));
	strncpy(buf, filename, len);

	// 파일 이름 크기 전송
	retval = send(sock, (char*)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("sendnamesize()");
	}
	buf[len] = '\0';

	// 파일 이름 전송
	retval = send(sock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("sendname()");
	}

	// 파일 크기 전송
	retval = send(sock, (char*)&fileSize, sizeof(fileSize), 0);
	if (retval == SOCKET_ERROR) {
		err_display("sendfileSize()");
	}

	// 파일 전송
	int retvalRead;

	while ((retvalRead = fread(buf, 1, BUFSIZE, send_file)) > 0) {
		retval = send(sock, buf, retvalRead, 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendfile()");
			break;
		}
	}

	fclose(send_file);
}

void Network::SendReadyStatus()
{
	string sl = "ReadyStatus";
	SendCommand(sl);
}
