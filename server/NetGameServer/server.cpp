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
	wstring searchPath = L"Sound\\*"; // ��� ���� �˻�
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
	cout << "���ɾ� ����:" << sList << endl;
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
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvEnterPlayStation, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}
	else if (sList == "SendPlayerScore")
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvPlayerScore, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
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

	else
	{
		cout << "failed" << endl;
		return;
	}
	cout << "success" << endl;
}

void RecvCheckLoginAndMusicDownload(SOCKET sock)
{
	// send�ؼ� ��Ʈ��ũ������ �����͸� ������
	int retval;
	char buf[BUFSIZE];

	/*int num = 0;
	retval = recv(sock, (char*)&num, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload() Size");
	}*/
	//cout << size << endl;
	for (const auto& m : musicDataSet)
	{
		string path = "Sound/" + m.musicName;
		// ������ ������ ���� ����
		FILE* send_file = fopen(path.c_str(), "rb");  // ������ ���� ���� ����
		if (send_file == NULL) {
			printf("���� ���� ���� %s\n", path.c_str());
			//break;
		}
		//cout << path << endl;
		unsigned long fileSize;
		fseek(send_file, 0, SEEK_END);	// ���� �����͸� ���� ������ �̵�
		fileSize = ftell(send_file);	// ���� ���� ������ ��ġ�� ���� (���� ũ��)
		rewind(send_file);				// ���� �����͸� �ٽ� ������ �������� ����

		unsigned int len = static_cast<unsigned int>(strlen(path.c_str()));

		strncpy(buf, path.c_str(), len);
		ThrottlePackets();

		// ���� �̸� �ؽ�Ʈ ũ�� ������(���� ����)
		
		retval = send(sock, (char*)&len, sizeof(unsigned int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendnamesize()");
			break;
		}
		cout << len << endl;
		buf[len] = '\0';
		cout << buf << endl;

		ThrottlePackets();
		// ���� �̸� �ؽ�Ʈ ������(���� ����)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendname()");
			break;
		}
		cout << fileSize << endl;

		ThrottlePackets();
		// ���� ������ ũ�� ������(���� ����)
		retval = send(sock, (char*)&fileSize, sizeof(unsigned long), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendfileSize()");
			break;
		}

		// ���� ������ ������(���� ����)
		int retvalRead;
		ThrottlePackets();
		while ((retvalRead = fread(buf, 1, BUFSIZE, send_file)) > 0) {
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
	// ����� ������ �̷��� �޾ƿ´�
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

unsigned __stdcall RecvEnterPlayStation(void* arg)
	// ���� ������Ʈ ����
	// �κ� ������� �÷��̾ 2���϶� ������ �㰡�Ѵ�

	// send�ؼ� ��Ʈ��ũ������ �����͸� ������
	int retval;

	unsigned char check = 'p';
	retval = send(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvEnterPlayStation()");
	}
	return 0;
}

unsigned __stdcall RecvEnterEditStation(void* arg)
{
	// ����� ������ �̷��� �޾ƿ´�
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
unsigned __stdcall RecvPlayerScore(void* arg)
{
	// recv�� �÷��̾� ������ �����Ѵ�
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
	cout << p.index << ": " << p.score << endl;

	return 0;
}
