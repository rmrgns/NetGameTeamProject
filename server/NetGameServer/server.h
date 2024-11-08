#pragma once

#include "Common.h"


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