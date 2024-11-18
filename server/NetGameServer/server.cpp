#include "server.h"

vector<MusicData> musicDataSet;
vector<LoginInfo> loginInfoSet;
vector<UserInfo> userInfoSet;
vector<LobbyInfo> lobbyInfoSet;


void CheckSendList(string sList, SOCKET client_sock)
{
	HANDLE hThread;
	// ������ sendList�� Ȯ���ؼ� �ش� server �Լ� ȣ��
	if (sList == "CheckLogin")
	{
		//RecvCheckLoginAndMusicDownload()
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvCheckLoginAndMusicDownload, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}
	else if (sList == "PlayerScore")
	{
		//SendPlayerScore()
		hThread = (HANDLE)_beginthreadex(NULL, 0, SendPlayerScore, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

	}
	else if (sList == "LeaveEditStation")
	{
		//RecvLeaveEditStation()
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvLeaveEditStation, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

	}
	else if (sList == "EnterEditStation")
	{
		//RecvEnterEditStation()
		hThread = (HANDLE)_beginthreadex(NULL, 0, RecvEnterEditStation, (LPVOID)client_sock, 0, NULL);
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

	// send�ؼ� ��Ʈ��ũ������ �����͸� ������
	//cout << "success" << endl;

	return 0;
}

unsigned __stdcall SendPlayerScore(void* arg)
{
	//cout << "success" << endl;
	return 0;
}

unsigned __stdcall RecvLeaveEditStation(void* arg)
{
	cout << "success" << endl;
	return 0;
}

unsigned __stdcall RecvEnterEditStation(void* arg)
{
	cout << "success" << endl;
	return 0;
}