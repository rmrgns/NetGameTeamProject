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
		int retval;
		unsigned long len;

		//sendList sl = sendList::CheckLogin;	
		//len = sizeof(sl);

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
	SOCKET sock = (SOCKET)arg;
	// send해서 네트워크쪽으로 데이터를 보낸다
	int retval;
	int len;
	string sl = "CheckLogin";
	len = sl.length();
	retval = send(sock, (char*)&len, sizeof(unsigned long), 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload() Size");
	}

	retval = send(sock, sl.c_str(), len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("SendCheckLoginAndMusicDownload()");
	}
	return 0;
}

unsigned __stdcall SendPlayerScore(void* arg)
{

	return 0;
}
