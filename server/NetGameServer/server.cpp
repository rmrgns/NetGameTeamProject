#include "server.h"

void CheckSendList(sendList sList)
{
	// ������ sendList�� Ȯ���ؼ� �ش� server �Լ� ȣ��
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
