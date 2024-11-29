#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

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
#include <array>

using namespace std;

enum class sendList : unsigned short
{
	None = 0,
	CheckLogin = 1,
	EnterPlayStation = 2,
	PlayerScore = 3,
	LeavePlayStation = 4,
	EnterLobby = 5,
	ReadyStatus = 6,
	AllReadyAndMusicIndex = 7,
	LeaveEditStation,
	EnterEditStation,

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
	unsigned short playerNum = 0;
	array<string, 2> id = { "","" };
	bool isReady=false;
	unsigned int musicIndex=0;
	array<unsigned int, 2> score = { 0,0 };
};

struct MusicData
{
	string musicName;
	string noteName;
};

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ


// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);

// ���� �Լ� ���� ���
void err_display(const char* msg);

// ���� �Լ� ���� ���
void err_display(int errcode);

// ��Ŷ �� ����
void ThrottlePackets();