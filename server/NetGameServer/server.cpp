#include "server.h"

vector<MusicData> musicDataSet;
vector<LoginInfo> loginInfoSet;
vector<UserInfo> userInfoSet;
vector<LobbyInfo> lobbyInfoSet;


void CheckSendList(sendList sList, SOCKET client_sock)
{
	HANDLE hThread;
	// ������ sendList�� Ȯ���ؼ� �ش� server �Լ� ȣ��
	if (sList == sendList::CheckLogin)
	{
		//RecvCheckLoginAndMusicDownload()
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvCheckLoginAndMusicDownload, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}
	else if (sList == sendList::EnterLobby)
	{
		
	}
	else
	{
		return;
	}
}

unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg)
{
	return 0;
}