#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...
#include <iostream>
#include <vector>
#include <process.h>
#include <atomic>
#include <chrono>
#include <thread>

using namespace std;

enum class sendList : unsigned short
{
	None = 0,
	CheckLogin,
	EnterPlayStation,
	PlayerScore,
	LeavePlayStation,
	EnterLobby,
	ReadyStatus,
	AllReadyAndMusicIndex,

};

struct LoginInfo
{
	char* id;
	char* password;
};

struct UserInfo
{
	string id;
	unsigned int maxScore;
};

struct LobbyInfo
{
	string id;
	bool isReady;
	unsigned int musicIndex;
	unsigned int score;
};

struct MusicData
{
	string musicName;
	string noteName;
};

struct PlayerScorePacket
{
	unsigned int index = 0;
	unsigned int score = 0;
};

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크


// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);

// 소켓 함수 오류 출력
void err_display(const char* msg);

// 소켓 함수 오류 출력
void err_display(int errcode);
