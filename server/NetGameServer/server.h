#pragma once

#include "Common.h"

extern atomic<int> packet_count; // 송신된 패킷 수
extern const int MAX_PACKETS_PER_SECOND; // 초당 허용 패킷 수
extern chrono::steady_clock::time_point last_reset_time; // 타이머 리셋 시간

void InitMusicData();
vector<string> GetFileNamesFromFolder();

void CheckSendList(string sList, SOCKET socket);

unsigned __stdcall SendPlayerScore(void* arg);
unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg);