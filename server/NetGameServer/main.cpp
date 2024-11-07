
#include "server.h"

#define SERVERPORT 9000
#define BUFSIZE    1024

CRITICAL_SECTION cs;

DWORD WINAPI ProcessClient(LPVOID arg)
{

    SOCKET client_sock = (SOCKET)arg;
    int retval;
    struct sockaddr_in clientaddr;
    char addr[INET_ADDRSTRLEN];
    int addrlen;
    unsigned long len; // ���� ���� ������
    char buf[BUFSIZE + 1]; // ���� ���� ������
    int localclientid;

    sendList sList;
    char name[BUFSIZE + 1];

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    //getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
    inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
    while (1) {
        //EnterCriticalSection(&cs);
        // sendList Ȯ��
        int temp;
        retval = recv(client_sock, reinterpret_cast<char*>(&temp), sizeof(short), MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("recvsendlist()");
            break;
        }
        else if (retval == 0)
            break;
        sList = static_cast<sendList>(temp);
        CheckSendList(sList);
           

            
        
        //LeaveCriticalSection(&cs);
    }
    // ���� �ݱ�
    closesocket(client_sock);
    return 0;
}

int main() {
    int retval;
    InitializeCriticalSection(&cs);

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // ���� ����
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // ������ ��ſ� ����� ����
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;
    HANDLE hThread;


    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        // ������ Ŭ���̾�Ʈ ���� ���
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("\033[%d;1H[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", 0,
            addr, ntohs(clientaddr.sin_port));

        // ������ ����
        hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }

    }

    // ���� �ݱ�
    closesocket(listen_sock);
    DeleteCriticalSection(&cs);
    // ���� ����
    WSACleanup();
    return 0;
}