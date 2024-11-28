#include "server.h"

vector<MusicData> musicDataSet;
vector<LoginInfo> loginInfoSet;
vector<UserInfo> userInfoSet;
vector<LobbyInfo> lobbyInfoSet;

std::atomic<int> packet_count(0);
const int MAX_PACKETS_PER_SECOND = 30; // �ʴ� 30���� ��Ŷ ����
std::chrono::steady_clock::time_point last_reset_time = std::chrono::steady_clock::now();

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
	
	// ������ sendList�� Ȯ���ؼ� �ش� server �Լ� ȣ��
	if (sList == "CheckLogin")
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvCheckLoginAndMusicDownload, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	else if (sList == "EnterPlayStation")
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvEnterPlayStation, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	else if (sList == "PlayerScore")
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, SendPlayerScore, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	else if (sList == "LeavePlayStation")
	{
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvLeavePlayStation, (LPVOID)client_sock, 0, NULL);
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
		return;
	}
}

void ThrottlePackets() 
{
	using namespace std::chrono;
	steady_clock::time_point now = steady_clock::now();

	// ���� �ð� ��� (�� ����)
	double elapsed_seconds = duration_cast<seconds>(now - last_reset_time).count();

	// 1�ʰ� �����ٸ� ��Ŷ ī��Ʈ�� �ʱ�ȭ
	if (elapsed_seconds >= 1.0) {
		packet_count = 0;
		last_reset_time = now;
	}

	// ���� �۽� ��Ŷ ���� �ʴ� �ִ�ġ�� ������ ���
	while (packet_count >= MAX_PACKETS_PER_SECOND) {
		std::this_thread::sleep_for(milliseconds(1)); // 1ms ���
		now = steady_clock::now();
		elapsed_seconds = duration_cast<seconds>(now - last_reset_time).count();
		if (elapsed_seconds >= 1.0) {
			packet_count = 0;
			last_reset_time = now;
		}
	}

	// ��Ŷ ī��Ʈ ����
	packet_count++;
}


unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg)
{
	// ����� ������ �̷��� �޾ƿ´�
	SOCKET sock = (SOCKET)arg;
	
	// send�ؼ� ��Ʈ��ũ������ �����͸� ������
	int retval;
	int len = 0;
	char buf[BUFSIZE];

	/*unsigned int size = 0;
	retval = send(sock, (char*)&size, sizeof(unsigned int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload() Size");
	}*/

	for (const auto& m : musicDataSet)
	{
		string path = "Sound/" + m.musicName;
		// ������ ������ ���� ����
		FILE* send_file = fopen(path.c_str(), "rb");  // ������ ���� ���� ����
		if (send_file == NULL) {
			printf("���� ���� ���� %s\n", path.c_str());
			break;
		}
		cout << path << endl;
		unsigned long fileSize;
		fseek(send_file, 0, SEEK_END);	// ���� �����͸� ���� ������ �̵�
		fileSize = ftell(send_file);	// ���� ���� ������ ��ġ�� ���� (���� ũ��)
		rewind(send_file);				// ���� �����͸� �ٽ� ������ �������� ����
		int len = (int)strlen(path.c_str());
		strncpy(buf, path.c_str(), len);
		//ThrottlePackets();
		// ���� �̸� �ؽ�Ʈ ũ�� ������(���� ����)
		retval = send(sock, (char*)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendnamesize()");
			break;
		}

		//ThrottlePackets();
		// ���� �̸� �ؽ�Ʈ ������(���� ����)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendname()");
			break;
		}

		//ThrottlePackets();
		// ���� ������ ũ�� ������(���� ����)
		retval = send(sock, (char*)&fileSize, sizeof(unsigned long), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendfileSize()");
			break;
		}

		// ���� ������ ������(���� ����)
		int retvalRead;
		//ThrottlePackets();
		while ((retvalRead = fread(buf, 1, BUFSIZE, send_file)) > 0) {
			retval = send(sock, buf, retvalRead, 0);
			if (retval == SOCKET_ERROR) {
				err_display("sendfile()");
				break;
			}
		}
	}
	return 0;
}

unsigned __stdcall RecvEnterPlayStation(void* arg)
{

	return 0;
}

unsigned __stdcall RecvLeavePlayStation(void* arg)
{

	return 0;
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

		//cout << "success3" << endl;
		return 0;
	}
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

		//cout << "success3" << endl;
		return 0;
	}
	return 0;

}