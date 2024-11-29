#include "Common.h"

void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

std::atomic<int> packet_count(0);
const int MAX_PACKETS_PER_SECOND = 60; // 초당 30개의 패킷 제한
std::chrono::steady_clock::time_point last_reset_time = std::chrono::steady_clock::now();

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
		std::this_thread::sleep_for(milliseconds(5)); // 5ms 대기
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