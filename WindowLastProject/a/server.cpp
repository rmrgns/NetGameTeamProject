#include "server.h"

vector<MusicData> musicDataSet;
vector<LoginInfo> loginInfoSet;
vector<UserInfo> userInfoSet;
vector<LobbyInfo> lobbyInfoSet;

std::atomic<int> packet_count(0);
const int MAX_PACKETS_PER_SECOND = 30; // 초당 30개의 패킷 제한
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
	wstring searchPath = L"Sound\\*"; // 모든 파일 검색
	//wstring wsearchPath = wstring(searchPath.begin(), searchPath.end());
	hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		//cerr << "Failed to open folder: " << folderPath << endl;
		return fileNames;
	}

	do 
	{
		// 디렉토리 제외
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
	
	// 소켓의 sendList를 확인해서 해당 server 함수 호출
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

	// 지난 시간 계산 (초 단위)
	double elapsed_seconds = duration_cast<seconds>(now - last_reset_time).count();

	// 1초가 지났다면 패킷 카운트를 초기화
	if (elapsed_seconds >= 1.0) {
		packet_count = 0;
		last_reset_time = now;
	}

	// 현재 송신 패킷 수가 초당 최대치를 넘으면 대기
	while (packet_count >= MAX_PACKETS_PER_SECOND) {
		std::this_thread::sleep_for(milliseconds(1)); // 1ms 대기
		now = steady_clock::now();
		elapsed_seconds = duration_cast<seconds>(now - last_reset_time).count();
		if (elapsed_seconds >= 1.0) {
			packet_count = 0;
			last_reset_time = now;
		}
	}

	// 패킷 카운트 증가
	packet_count++;
}


unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg)
{
	// 사용할 소켓은 이렇게 받아온다
	SOCKET sock = (SOCKET)arg;
	
	// send해서 네트워크쪽으로 데이터를 보낸다
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
		// 파일을 소켓을 통해 전송
		FILE* send_file = fopen(path.c_str(), "rb");  // 파일을 이진 모드로 열기
		if (send_file == NULL) {
			printf("파일 열기 실패 %s\n", path.c_str());
			break;
		}
		cout << path << endl;
		unsigned long fileSize;
		fseek(send_file, 0, SEEK_END);	// 파일 포인터를 파일 끝으로 이동
		fileSize = ftell(send_file);	// 현재 파일 포인터 위치를 얻음 (파일 크기)
		rewind(send_file);				// 파일 포인터를 다시 파일의 시작으로 복원
		int len = (int)strlen(path.c_str());
		strncpy(buf, path.c_str(), len);
		//ThrottlePackets();
		// 파일 이름 텍스트 크기 보내기(고정 길이)
		retval = send(sock, (char*)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendnamesize()");
			break;
		}

		//ThrottlePackets();
		// 파일 이름 텍스트 보내기(가변 길이)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendname()");
			break;
		}

		//ThrottlePackets();
		// 파일 데이터 크기 보내기(고정 길이)
		retval = send(sock, (char*)&fileSize, sizeof(unsigned long), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendfileSize()");
			break;
		}

		// 파일 데이터 보내기(가변 길이)
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
	// 사용할 소켓은 이렇게 받아온다
	SOCKET sock = (SOCKET)arg;

	// send해서 네트워크쪽으로 데이터를 보낸다
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
	// 사용할 소켓은 이렇게 받아온다
	SOCKET sock = (SOCKET)arg;

	// send해서 네트워크쪽으로 데이터를 보낸다
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