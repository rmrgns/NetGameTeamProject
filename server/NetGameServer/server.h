#pragma once

#include "Common.h"

static SOCKET client_sock;

void CheckSendList(string sList, SOCKET socket);

unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg);
unsigned __stdcall SendPlayerScore(void* arg);
unsigned __stdcall RecvLeaveEditStation(void* arg);
unsigned __stdcall RecvEnterEditStation(void* arg);