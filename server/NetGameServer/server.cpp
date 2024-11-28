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
	cout << "명령어 수신:" << sList << endl;
	// 소켓의 sendList를 확인해서 해당 server 함수 호출
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
	else
	{
		cout << "failed" << endl;
		return;
	}
	cout << "success" << endl;
}

void RecvCheckLoginAndMusicDownload(SOCKET sock)
{
	// send해서 네트워크쪽으로 데이터를 보낸다
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
		// 파일을 소켓을 통해 전송
		FILE* send_file = fopen(path.c_str(), "rb");  // 파일을 이진 모드로 열기
		if (send_file == NULL) {
			printf("파일 열기 실패 %s\n", path.c_str());
			//break;
		}
		//cout << path << endl;
		unsigned long fileSize;
		fseek(send_file, 0, SEEK_END);	// 파일 포인터를 파일 끝으로 이동
		fileSize = ftell(send_file);	// 현재 파일 포인터 위치를 얻음 (파일 크기)
		rewind(send_file);				// 파일 포인터를 다시 파일의 시작으로 복원

		unsigned int len = static_cast<unsigned int>(strlen(path.c_str()));

		strncpy(buf, path.c_str(), len);
		ThrottlePackets();

		// 파일 이름 텍스트 크기 보내기(고정 길이)
		
		retval = send(sock, (char*)&len, sizeof(unsigned int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendnamesize()");
			break;
		}
		cout << len << endl;
		buf[len] = '\0';
		cout << buf << endl;

		ThrottlePackets();
		// 파일 이름 텍스트 보내기(가변 길이)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendname()");
			break;
		}
		cout << fileSize << endl;

		ThrottlePackets();
		// 파일 데이터 크기 보내기(고정 길이)
		retval = send(sock, (char*)&fileSize, sizeof(unsigned long), 0);
		if (retval == SOCKET_ERROR) {
			err_display("sendfileSize()");
			break;
		}

		// 파일 데이터 보내기(가변 길이)
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

unsigned __stdcall RecvEnterPlayStation(void* arg)
{
	// 사용할 소켓은 이렇게 받아온다
	SOCKET sock = (SOCKET)arg;

	// 추후 업데이트 내용
	// 로비에 대기중인 플레이어가 2명일때 입장을 허가한다

	// send해서 네트워크쪽으로 데이터를 보낸다
	int retval;

	unsigned char check = 'p';
	retval = send(sock, (char*)&check, sizeof(unsigned char), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvEnterPlayStation()");
	}
	return 0;
}

unsigned __stdcall RecvPlayerScore(void* arg)
{
	// 사용할 소켓은 이렇게 받아온다
	SOCKET sock = (SOCKET)arg;

	
	// recv한 플레이어 점수를 저장한다
	int retval;

	// 플레이어 점수 받아오는 코드
	PlayerScorePacket p;
	ThrottlePackets();
	retval = recv(sock, (char*)&p, sizeof(PlayerScorePacket), 0);
	if (retval == SOCKET_ERROR) {
		err_display("RecvPlayerScore()");
	}

	// 추후 업데이트 내용
	// 플레이어 id를 받아서 해당 플레이어의 점수에 업데이트한다
	cout << p.index << ": " << p.score << endl;

	return 0;
}
