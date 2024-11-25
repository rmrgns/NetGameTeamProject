#pragma once

#include "Common.h"

extern atomic<int> packet_count; // �۽ŵ� ��Ŷ ��
extern const int MAX_PACKETS_PER_SECOND; // �ʴ� ��� ��Ŷ ��
extern chrono::steady_clock::time_point last_reset_time; // Ÿ�̸� ���� �ð�

#define BUFSIZE 1024	// ���� ������

// ������ MusicDataSet �ʱ�ȭ
void InitMusicData();
vector<string> GetFileNamesFromFolder();

// ��Ʈ��ũ Ŭ�������� ������ sendList üũ
void CheckSendList(string sList, SOCKET socket);

// �α���Ȯ�� �� ������ MusicData �۽� �Լ�
unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg);

unsigned __stdcall SendPlayerScore(void* arg);
unsigned __stdcall RecvLeaveEditStation(void* arg);
unsigned __stdcall RecvEnterEditStation(void* arg);

// PlayStation �����Լ�
unsigned __stdcall RecvEnterPlayStation(void* arg);

