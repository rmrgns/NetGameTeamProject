#pragma once

#include "Common.h"
#include "packet.h"



#define BUFSIZE 1024	// 버퍼 사이즈

// 서버의 MusicDataSet 초기화
void InitMusicData();
vector<string> GetFileNamesFromFolder();

// 네트워크 클래스에서 전달한 sendList 체크
void CheckSendList(string sList, SOCKET socket);

// 로그인확인 및 서버의 MusicData 송신 함수
void RecvCheckLoginAndMusicDownload(SOCKET sock);

unsigned __stdcall SendPlayerScore(void* arg);
unsigned __stdcall RecvLeaveEditStation(void* arg);
unsigned __stdcall RecvEnterEditStation(void* arg);

// PlayStation 입장함수
unsigned __stdcall RecvEnterPlayStation(void* arg);

// 클라에서 보낸 플레이어 점수를 저장
unsigned __stdcall RecvPlayerScore(void* arg);

