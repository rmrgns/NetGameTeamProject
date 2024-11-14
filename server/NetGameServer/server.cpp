#include "server.h"

vector<MusicData> musicDataSet;
vector<LoginInfo> loginInfoSet;
vector<UserInfo> userInfoSet;
vector<LobbyInfo> lobbyInfoSet;


void CheckSendList(string sList, SOCKET client_sock)
{
	HANDLE hThread;
	// 소켓의 sendList를 확인해서 해당 server 함수 호출
	if (sList == "CheckLogin")
	{
		//RecvCheckLoginAndMusicDownload()
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvCheckLoginAndMusicDownload, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}
	/*else if (sList == sendList::EnterLobby)
	{
		
	}*/
	else
	{
		return;
	}
}

unsigned __stdcall RecvCheckLoginAndMusicDownload(void* arg)
{

	// send해서 네트워크쪽으로 데이터를 보낸다
	cout << "success" << endl;

	return 0;
}

unsigned __stdcall SendPlayerScore(void* arg)
{

	return 0;
}
