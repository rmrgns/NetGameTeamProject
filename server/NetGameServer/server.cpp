#include "server.h"
#include "packet.h"

vector<MusicData> musicDataSet;
vector<LoginInfo> loginInfoSet;
vector<UserInfo> userInfoSet;
vector<LobbyInfo> lobbyInfoSet;

void InitMusicData()
{
	vector<string>fileNames = GetFileNamesFromFolder();
	for (const auto& f : fileNames)
	{
		MusicData temp;
		temp.musicName = f;
		temp.noteName = " ";
		musicDataSet.push_back(temp);
	}
}

string WStringToString(const wstring& wstr) 
{
	string str(wstr.begin(), wstr.end());
	return str;
}

vector<string> GetFileNamesFromFolder() 
{
	vector<string> fileNames;
	WIN32_FIND_DATAW findFileData;
	HANDLE hFind;
	string str;

	wstring searchPath = L"Sound\\*"; // 占쏙옙占?占쏙옙占쏙옙 占싯삼옙

	//wstring wsearchPath = wstring(searchPath.begin(), searchPath.end());
	hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		//cerr << "Failed to open folder: " << folderPath << endl;
		return fileNames;
	}

	do 
	{
		// 占쏙옙占썰리 占쏙옙占쏙옙
		if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
		{
			fileNames.emplace_back(WStringToString(findFileData.cFileName));
		}
	} while (FindNextFileW(hFind, &findFileData) != 0);

	FindClose(hFind);
	return fileNames;
}

void CheckSendList(string sList, SOCKET client_sock)
{
	HANDLE hThread;

	// 占쏙옙占쏙옙占쏙옙 sendList占쏙옙 확占쏙옙占쌔쇽옙 占쌔댐옙 server 占쌉쇽옙 호占쏙옙
	if (sList == "CheckLogin")
	{
		/*hThread = (HANDLE)_beginthreadex(NULL, 0, RecvCheckLoginAndMusicDownload, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }*/
		RecvCheckLoginAndMusicDownload(client_sock);
	}
	else if (sList == "EnterPlayStation")
	{
		RecvEnterPlayStation(client_sock);
	}
	else if (sList == "SendPlayerScore")
	{
		/*hThread = (HANDLE)_beginthreadex(NULL, 0, RecvPlayerScore, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }*/
		RecvPlayerScore(client_sock);
	}
	else if (sList == "PlayerScore")
	{
		/*hThread = (HANDLE)_beginthreadex(NULL, 0, SendPlayerScore, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }*/
		SendPlayerScore(client_sock);
	}

	else if (sList == "LeaveEditStation")
	{
		/*hThread = (HANDLE)_beginthreadex(NULL, 0, RecvLeaveEditStation, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }*/
		RecvLeaveEditStation(client_sock);
	}

	else if (sList == "EnterEditStation")
	{
		//hThread = (HANDLE)_beginthreadex(NULL, 0, RecvEnterEditStation, (LPVOID)client_sock, 0, NULL);
		//if (hThread == NULL) { closesocket(client_sock); }
		//else { CloseHandle(hThread); }
		RecvEnterEditStation(client_sock);
	}

	else if (sList == "LeavePlayStation")
	{
		RecvLeavePlayStation(client_sock);
	}

	else if (sList == "EnterLobbyAndInfo")
	{
		RecvEnterLobbyAndInfo(client_sock);
	}
	else if (sList == "UploadMusic")
	{
		RecvUploadMusic(client_sock);
	}
	else if (sList == "ReadyStatus")
	{


	}
	else
	{
		cout << "failed" << endl;
		return;
	}
}

void RecvCheckLoginAndMusicDownload(SOCKET sock)
{
	// send占쌔쇽옙 占쏙옙트占쏙옙크占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싶몌옙 占쏙옙占쏙옙占쏙옙
	

	/*int num = 0;
	retval = recv(sock, (char*)&num, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload() Size");
	}*/
	//cout << size << endl;
	for (const auto& m : musicDataSet)
	{
		int retval;
		char buf[BUFSIZE];

		string path = "Sound/" + m.musicName;
		// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
		FILE* send_file = fopen(path.c_str(), "rb");  // 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
		if (send_file == NULL) {
			printf("file open error %s\n", path.c_str());
			//break;
		}
		//cout << path << endl;
		unsigned long fileSize;
		fseek(send_file, 0, SEEK_END);	// 占쏙옙占쏙옙 占쏙옙占쏙옙占싶몌옙 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占싱듸옙
		fileSize = ftell(send_file);	// 占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙치占쏙옙 占쏙옙占쏙옙 (占쏙옙占쏙옙 크占쏙옙)
		rewind(send_file);				// 占쏙옙占쏙옙 占쏙옙占쏙옙占싶몌옙 占쌕쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙

		unsigned long len= static_cast<unsigned long>(strlen(path.c_str()));
		strncpy(buf, path.c_str(), len);
		
		// send file name size
		ThrottlePackets();
		retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendnamesize()");
			break;
		}
		cout << len << endl;
		buf[len] = '\0';
		cout << buf << endl;

		
		// send file name
		ThrottlePackets();
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendname()");
			break;
		}
		cout << fileSize << endl;

		
		// send file size
		ThrottlePackets();
		retval = send(sock, (char*)&fileSize, sizeof(unsigned long), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendfileSize()");
			break;
		}

		// send file
		int retvalRead;
		
		while ((retvalRead = fread(buf, 1, BUFSIZE, send_file)) > 0) {
			ThrottlePackets();
			retval = send(sock, buf, retvalRead, 0);
			if (retval == SOCKET_ERROR) {
				err_display("sendfile()");
				break;
			}
		}
		cout << "successSV" << endl;
	}
}

void SendPlayerScore(SOCKET sock)
{


}


void RecvLeaveEditStation(SOCKET sock)
{

	// send占쌔쇽옙 占쏙옙트占쏙옙크占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싶몌옙 占쏙옙占쏙옙占쏙옙
	int retval;
	bool checkLeaveEditStation = TRUE;


	retval = send(sock, (char*)&checkLeaveEditStation, sizeof(bool), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvLeaveEditStation()");
	}
	//cout << "success3" << endl;
}

void RecvEnterPlayStation(SOCKET sock)
{


	// 占싸븝옙 占쏙옙占쏙옙占쏙옙占?占시뤄옙占싱어가 2占쏙옙占싹띰옙 占쏙옙占쏙옙占쏙옙 占썬가占싼댐옙

	// send占쌔쇽옙 占쏙옙트占쏙옙크占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싶몌옙 占쏙옙占쏙옙占쏙옙
	int retval;

	unsigned char isReady = ' ';
	unsigned short index = 0;

	ThrottlePackets();
	retval = recv(sock, (char*)&isReady, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("EnterPlayStation");
	}
	if (isReady == 'p')
	{
		if (lobby.isReady == false)
		{
			//cout << "1" << endl;
			lobby.isReady = true;
			index = 0;
			ThrottlePackets();
			retval = send(sock, (char*)&index, sizeof(unsigned short), 0);
			if (retval == SOCKET_ERROR) {
				err_display("RecvEnterPlayStation()");
			}
		}
		else
		{
			//cout << "2" << endl;
			lobby.isReady2 = true;
			index = 1;
			ThrottlePackets();
			retval = send(sock, (char*)&index, sizeof(unsigned short), 0);
			if (retval == SOCKET_ERROR) {
				err_display("RecvEnterPlayStation()");
			}
		}
	}
	else
	{
		index = 0;
		ThrottlePackets();
		retval = send(sock, (char*)&index, sizeof(unsigned short), 0);
		if (retval == SOCKET_ERROR) {
			err_display("RecvEnterPlayStation()");
		}
	}

	

	unsigned char check = 'p';
	ThrottlePackets();
	retval = send(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvEnterPlayStation()");
	}
}

void RecvLeavePlayStation(SOCKET sock)
{
	int retval;

	unsigned char check = '4';

	retval = send(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvLeavePlayStation()");
	}
}

void RecvPlayerScore(SOCKET sock)
{
	int retval;

	// 占시뤄옙占싱억옙 占쏙옙占쏙옙 占쌨아울옙占쏙옙 占쌘듸옙
	PlayerScorePacket p;
	ThrottlePackets();
	retval = recv(sock, (char*)&p, sizeof(PlayerScorePacket), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvPlayerScore()");
	}

	lobby.score[p.index] = p.score;

	// ���� ������Ʈ ����
	// �÷��̾� id�� �޾Ƽ� �ش� �÷��̾��� ������ ������Ʈ�Ѵ�
	cout << p.index <<" score: " << p.score << endl;

	p.index = 0;
	p.score = lobby.score[0];
	ThrottlePackets();
	retval = send(sock, (char*)&p, sizeof(PlayerScorePacket), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvPlayerScore()");
	}

	p.index = 1;
	p.score = lobby.score[1];

	ThrottlePackets();
	retval = send(sock, (char*)&p, sizeof(PlayerScorePacket), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvPlayerScore()");
	}
}

void RecvEnterLobbyAndInfo(SOCKET sock)
{
	int retval;
	unsigned int len = 0;
	char buf[BUFSIZE];

	// send file name size
	ThrottlePackets();
	retval = recv(sock, (char*)&len, sizeof(unsigned int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvIDSIzeAtEnterLobby");
		return;
	}
	cout << len << endl;
	

	// send file name
	ThrottlePackets();
	retval = recv(sock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvIDAtEnterLobby");
		return;
	}
	buf[len] = '\0';
	cout << buf << endl;

	// input userData -> lobbyData
	if (lobbyData.empty())
	{
		LobbyInfo lf;
		lobbyData.push_back(lf);
		lobbyData.back().id[0] = buf;
		lobbyData.back().musicIndex = 0;
	}
	else
	{
		unsigned short num = lobbyData.back().playerNum;
		if (num < 2)
		{
			lobbyData.back().id[num] = buf;
		}
		else
		{
			LobbyInfo lf;
			lobbyData.push_back(lf);
			lobbyData.back().id[0] = buf;
			lobbyData.back().musicIndex = 0;
		}

	}	
	
	unsigned char check = '5';
	ThrottlePackets();
	retval = send(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvEnterLobbyAndInfo");
		return;
	}

	lobbyData.back().playerNum += 1;
}

void RecvEnterEditStation(SOCKET sock)
{


	// send占쌔쇽옙 占쏙옙트占쏙옙크占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占싶몌옙 占쏙옙占쏙옙占쏙옙
	int retval;
	bool checkEnterEditStation = TRUE;


	retval = send(sock, (char*)&checkEnterEditStation, sizeof(bool), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvEnterEditStation()");
	}
	//cout << "success3" << endl;
}

void RecvUploadMusic(SOCKET sock)
{
	int retval;
	unsigned long filesize = 0;
	char buf[BUFSIZE + 1] = { 0 };
	char name[BUFSIZE + 1];

	retval = recv(sock, (char*)&filesize, sizeof(unsigned long), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recvnamesize()");
		return;
	}
	else if (retval == 0) {
		printf("1\n");
		return;
	}

	// 파일 이름 전송

	retval = recv(sock, buf, filesize, MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recvname()");
		return;
	}
	else if (retval == 0) {
		printf("2\n");
		return;
	}

	buf[retval] = '\0';
	strcpy(name, buf);
	FILE* recvFile = fopen(name, "wb");
	if (recvFile == NULL) {
		printf("file open error\n");
		return;
	}

	// 파일 크기 전송
	retval = recv(sock, (char*)&filesize, sizeof(filesize), MSG_WAITALL);
	if (retval == SOCKET_ERROR) {
		err_display("recvfilesize()");
		return;
	}
	else if (retval == 0) {
		printf("3\n");
		return;
	}

	// 파일 전송    
	unsigned long totalBytesReceived = 0;

	//printf("\033[%d;1H", localclientid);
	//printf("\033[K");

	while (totalBytesReceived < filesize) {
		retval = recv(sock, buf, BUFSIZE, 0);

		if (retval == SOCKET_ERROR) {
			err_display("recvfile()");
			break;
		}

		fwrite(buf, 1, retval, recvFile);
		totalBytesReceived += retval;

		//// ���൵ ǥ��
		//EnterCriticalSection(&cs);
		//// �� ����: Ŭ���̾�Ʈ�� ���� �ٷ� �̵� �� ���൵ ǥ��
		//printf("\033[%d;1H", localclientid);  // Ŭ���̾�Ʈ ID�� ���� �ٸ� �ٷ� �̵�
		//printf("[Ŭ���̾�Ʈ %d] ���൵: %d%% / ��ü ������ ũ��: %ld, ���� ���� ũ��: %ld\n",
		//	localclientid,
		//	(int)(((float)totalBytesReceived / (float)len) * 100.f),
		//	len, totalBytesReceived);

		//LeaveCriticalSection(&cs);
	}
	cout << "success" << endl;
	fclose(recvFile);  // ���� �ݱ�	
}