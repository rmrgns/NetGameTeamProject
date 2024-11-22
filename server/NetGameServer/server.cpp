#include "server.h"

vector<MusicData> musicDataSet;
vector<LoginInfo> loginInfoSet;
vector<UserInfo> userInfoSet;
vector<LobbyInfo> lobbyInfoSet;

std::atomic<int> packet_count(0);
const int MAX_PACKETS_PER_SECOND = 30; // 초당 100개의 패킷 제한
std::chrono::steady_clock::time_point last_reset_time = std::chrono::steady_clock::now();

void CheckSendList(string sList, SOCKET client_sock)
{
	HANDLE hThread;
	
	// 소켓의 sendList를 확인해서 해당 server 함수 호출
	if (sList == "CheckLogin")
	{
		int retval;
		unsigned long len;

		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvCheckLoginAndMusicDownload, (LPVOID)client_sock, 0, NULL);
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
	int len;
	string sl = "CheckLogin";
	len = sl.length();

	ThrottlePackets();
	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload() Size");
	}

	ThrottlePackets();
	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload()");
	}
	return 0;
}

unsigned __stdcall SendPlayerScore(void* arg)
{

	return 0;
}
