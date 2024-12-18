#include "server.h"

#define SERVERPORT 9000
#define BUFSIZE    512

CRITICAL_SECTION cs;
//static int num = 0;

unsigned __stdcall ProcessClient(void* arg)
{
    //num++;
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    struct sockaddr_in clientaddr;
    char addr[INET_ADDRSTRLEN];
    int addrlen;
    unsigned int len; // 고정 길이 데이터
    char buf[BUFSIZE + 1]; // 가변 길이 데이터
    int localclientid;

    char name[BUFSIZE + 1];

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
    inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
    //cout << num << endl;
    while (1) {
        // sendList 확인
        retval = recv(client_sock, (char*)&len, sizeof(unsigned int), MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("recvnamesize()");
            break;
        }

        retval = recv(client_sock, buf, len, MSG_WAITALL);
        if (retval == SOCKET_ERROR) {
            err_display("recvsendlist()");
            break;
        }

        buf[retval] = '\0';
        CheckSendList(buf, client_sock);
    }
    // 소켓 닫기
    closesocket(client_sock);
    return 0;
}

int main() {
    int retval;
    InitializeCriticalSection(&cs);
    InitMusicData();
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
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

    // 데이터 통신에 사용할 변수
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

        // 접속한 클라이언트 정보 출력
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
        printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            addr, ntohs(clientaddr.sin_port));

        // 스레드 생성
        hThread = (HANDLE)_beginthreadex(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) { closesocket(client_sock); }
        else { CloseHandle(hThread); }

    }

    // 소켓 닫기
    closesocket(listen_sock);
    DeleteCriticalSection(&cs);
    // 윈속 종료
    WSACleanup();
    return 0;
}