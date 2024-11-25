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
	char* id;
	unsigned int maxScore;
};

struct LobbyInfo
{
	char* id;
	bool isReady;
	unsigned int musicIndex;
	unsigned int score;
};

struct MusicData
{
	char* musicName;
	char* noteName;
};

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ


// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg);

// ���� �Լ� ���� ���
void err_display(const char* msg);

// ���� �Լ� ���� ���
void err_display(int errcode);