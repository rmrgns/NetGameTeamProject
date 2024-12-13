#pragma once

#include "Common.h"
#include "packet.h"

// lobby information
static vector<LobbyInfo> lobbyData;
static LobbyInfo lobby;

#define BUFSIZE 4096	// ??? ????

// ?????? MusicDataSet ????
void InitMusicData();
vector<string> GetFileNamesFromFolder();

// ?????ũ Ŭ???????? ????? sendList üũ
void CheckSendList(string sList, SOCKET socket);


// ?α????? ?? ?????? MusicData ?۽? ???
void RecvCheckLoginAndMusicDownload(SOCKET sock);

unsigned __stdcall SendPlayerScore(void* arg);
unsigned __stdcall RecvLeaveEditStation(void* arg);
unsigned __stdcall RecvEnterEditStation(void* arg);

// PlayStation ???Լ?
void RecvEnterPlayStation(SOCKET sock);
void RecvLeavePlayStation(SOCKET sock);

// Ŭ?󿡼? ???? ????̾? ?????? ????
void RecvPlayerScore(SOCKET sock);

// Lobby
void RecvEnterLobbyAndInfo(SOCKET sock);
void SendRequestAllReadyAndMusicIndex(SOCKET sock);
void setReady();