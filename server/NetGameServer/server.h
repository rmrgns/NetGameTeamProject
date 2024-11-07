#pragma once

enum class sendList
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