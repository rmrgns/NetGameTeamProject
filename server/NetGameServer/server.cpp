#include "server.h"

void CheckSendList(sendList sList)
{
	// 소켓의 sendList를 확인해서 해당 server 함수 호출
	if (sList == sendList::CheckLogin)
	{

	}
	else if (sList == sendList::EnterLobby)
	{

	}
	else
	{
		return;
	}
}

void RecvCheckLoginAndMusicDownload(SOCKET socket, char* name, char* password, vector<MusicData> musicDataSet)
{

}
