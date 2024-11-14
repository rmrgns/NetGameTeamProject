#pragma once

#include "Common.h"


void CheckSendList(string sList, SOCKET socket);


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
unsigned __stdcall SendPlayerScore(void* arg);
unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg);