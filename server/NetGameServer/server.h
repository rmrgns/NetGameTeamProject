#pragma once

#include "Common.h"

void CheckSendList(string sList, SOCKET socket);

unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg);
unsigned __stdcall SendPlayerScore(void* arg);
unsigned __stdcall RecvLeaveEditStation(void* arg);
unsigned __stdcall RecvEnterEditStation(void* arg);