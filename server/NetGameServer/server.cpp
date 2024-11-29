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
	wstring searchPath = L"Sound\\*"; // ���?���� �˻�
	//wstring wsearchPath = wstring(searchPath.begin(), searchPath.end());
	hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		//cerr << "Failed to open folder: " << folderPath << endl;
		return fileNames;
	}

	do 
	{
		// ���丮 ����
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

	// ������ sendList�� Ȯ���ؼ� �ش� server �Լ� ȣ��
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
		hThread = (HANDLE)_beginthreadex(NULL, 0, SendPlayerScore, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	else if (sList == "LeaveEditStation")
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvLeaveEditStation, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	else if (sList == "EnterEditStation")
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvEnterEditStation, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	else if (sList == "LeavePlayStation")
	{
		RecvLeavePlayStation(client_sock);
	}

	else if (sList == "EnterLobbyAndInfo")
	{
		RecvEnterLobbyAndInfo(client_sock);
	}

	else
	{
		cout << "failed" << endl;
		return;
	}
}

void RecvCheckLoginAndMusicDownload(SOCKET sock)
{
	// send�ؼ� ��Ʈ��ũ������ �����͸� ������
	

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
		// ������ ������ ���� ����
		FILE* send_file = fopen(path.c_str(), "rb");  // ������ ���� ���� ����
		if (send_file == NULL) {
			printf("file open error %s\n", path.c_str());
			//break;
		}
		//cout << path << endl;
		unsigned long fileSize;
		fseek(send_file, 0, SEEK_END);	// ���� �����͸� ���� ������ �̵�
		fileSize = ftell(send_file);	// ���� ���� ������ ��ġ�� ���� (���� ũ��)
		rewind(send_file);				// ���� �����͸� �ٽ� ������ �������� ����

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

unsigned __stdcall SendPlayerScore(void* arg)
{

	return 0;
}


unsigned __stdcall RecvLeaveEditStation(void* arg)
{
	// �����?������ �̷��� �޾ƿ´�
	SOCKET sock = (SOCKET)arg;

	// send�ؼ� ��Ʈ��ũ������ �����͸� ������
	int retval;
	bool checkLeaveEditStation = TRUE;


	retval = send(sock, (char*)&checkLeaveEditStation, sizeof(bool), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvLeaveEditStation()");
	}
	//cout << "success3" << endl;
	return 0;
}

void RecvEnterPlayStation(SOCKET sock)
{

	// �κ� �������?�÷��̾ 2���϶� ������ �㰡�Ѵ�

	// send�ؼ� ��Ʈ��ũ������ �����͸� ������
	int retval;

	unsigned char check = 'p';
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

	// �÷��̾� ���� �޾ƿ��� �ڵ�
	PlayerScorePacket p;
	ThrottlePackets();
	retval = recv(sock, (char*)&p, sizeof(PlayerScorePacket), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvPlayerScore()");
	}

	// ���� ������Ʈ ����
	// �÷��̾� id�� �޾Ƽ� �ش� �÷��̾��� ������ ������Ʈ�Ѵ�
	cout << "score: " << p.score << endl;

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

unsigned __stdcall RecvEnterEditStation(void* arg)
{
	// �����?������ �̷��� �޾ƿ´�
	SOCKET sock = (SOCKET)arg;

	// send�ؼ� ��Ʈ��ũ������ �����͸� ������
	int retval;
	bool checkEnterEditStation = TRUE;


	retval = send(sock, (char*)&checkEnterEditStation, sizeof(bool), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvEnterEditStation()");
	}
	//cout << "success3" << endl;
	return 0;

}
