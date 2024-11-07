#pragma once

#include "Common.h"



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

enum sendList
{
	CheckLogin = 0,
	EnterPlayStation,
	PlayerScore,
	LeavePlayStation,
	EnterLobby,
	ReadyStatus,
	AllReadyAndMusicIndex,

};




void CheckSendList(sendList sList, SOCKET socket);
unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg);