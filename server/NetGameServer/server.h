#pragma once

#include "Common.h"

extern atomic<int> packet_count; // 송신된 패킷 수
extern const int MAX_PACKETS_PER_SECOND; // 초당 허용 패킷 수
extern chrono::steady_clock::time_point last_reset_time; // 타이머 리셋 시간

#define BUFSIZE 1024	// 버퍼 사이즈

// 서버의 MusicDataSet 초기화
void InitMusicData();
vector<string> GetFileNamesFromFolder();

// 네트워크 클래스에서 전달한 sendList 체크
void CheckSendList(string sList, SOCKET socket);

// 로그인확인 및 서버의 MusicData 송신 함수
unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg);

unsigned __stdcall SendPlayerScore(void* arg);
unsigned __stdcall RecvLeaveEditStation(void* arg);
unsigned __stdcall RecvEnterEditStation(void* arg);

// PlayStation 입장함수
unsigned __stdcall RecvEnterPlayStation(void* arg);

