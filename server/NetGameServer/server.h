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

void CheckSendList(sendList sList);
void RecvCheckLoginAndMusicDownload(SOCKET socket, char* name, char* password, vector<MusicData> musicDataSet);