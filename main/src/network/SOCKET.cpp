#include"network/SOCKET.h"
//#include"headers.h"
WINSOCK::WINSOCK() {}

bool WINSOCK::initialize()
{
	err = WSAStartup(MAKEWORD(2,0), &wsaData);

	if (err != 0)
	{
		switch (err)
		{
		case WSASYSNOTREADY:
			printf("エラー：システムの準備が整っていません。");
			break;
		case WSAVERNOTSUPPORTED:

			printf("エラー：Winsockがサポートされていません。");
			break;
		case WSAEINPROGRESS:

			printf("エラー：サービスプロバイダがビジーです。");
			break;
		case WSAEPROCLIM:

			printf("エラー：Winsockはこれ以上多くのプロセスを扱えません。");
			break;
		case WSAEFAULT:

			printf("エラー：Winsockのデータが無効です");
			break;
		}
		return false;
	}

	printf("winsockの初期化に成功\n");
	// ホスト名取得処理
    char lpszHostName[100];        // ホストネーム格納領域
                              // エラーナンバー

    if( gethostname(lpszHostName, 100) == 0){
		printf("ホスト名: %s\n", lpszHostName);
    }
    else{
        err = WSAGetLastError();
        printf("ホスト名の取得に失敗しました。\n");
        printf("エラー%dが発生しました\n", err);
       return false;
    }

    // IPアドレス取得処理
    char lpszIP[16];
    PHOSTENT phostent;
    IN_ADDR in;

	//if ((phostent = gethostbyname(lpszHostName)) != NULL){
	if ((phostent = gethostbyname(lpszHostName)) != NULL)
	{
        memcpy(&in, phostent->h_addr, 4);
        wsprintf(lpszIP, inet_ntoa(in));
        printf("IPアドレス：%s\n", lpszIP);
		strcpy_s(myIP, 15, lpszIP);
    }
	return true;
}
WINSOCK::~WINSOCK()
{
	closesocket(sock);
	WSACleanup();
}

bool WINSOCK::createSocket(int port)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	//sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		printf("socket failed error : %d\n", WSAGetLastError());
		//MessageBox(0, "socket failed\n", NULL, MB_OK);
		return 0;
	}
	this->port = port;
	DWORD  dwNonBlocking = 1;
	ioctlsocket(sock, FIONBIO, &dwNonBlocking);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//addr.sin_addr.S_un.S_addr = inet_addr(IP);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	int err = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if(err != 0)
	{
		printf("socket failed error : %d\n", WSAGetLastError());
		return 0;
	}

	 return 1;
}
void WINSOCK::destroySocket()
{
	shutdown(sock, SD_BOTH);
	closesocket(sock);
}
int WINSOCK::sendString(char* buf, char* IP, int port, int len)
{
	youraddr.sin_family = AF_INET;
	youraddr.sin_port = htons(port);
	youraddr.sin_addr.S_un.S_addr = inet_addr(IP);
	int i = 0;
	if((i = sendto(sock, buf, len, 0, (struct sockaddr *)&youraddr, sizeof(youraddr))) > 0)
	{
		//printf("%dバイト送信しました\n", i);
		//printf("%12sと送りました\n", buf);
		return i;
	}
	printf("接続に失敗しました\n");
	printf("error : %d\n", WSAGetLastError());
	return 0;
}
int WINSOCK::recieveString(char *buf, int size, char* IP, struct sockaddr_in *addr_out)
{

	struct sockaddr_in addr_from;
	int fromlen = sizeof(struct sockaddr_in);
	int i = recvfrom(sock, buf, BUFFERSIZE, 0, (struct sockaddr *)&addr_from, &fromlen);
	//int i = recv(sock, buf, BUFFERSIZE, 0);
	if(i < 0)
	{
		//printf("受信に失敗しました\n");
		//printf("error : %d\n", WSAGetLastError());
		return 0;
	}
	else if(i == 0)
		return 0;
	//printf("%sと送られてきました\n", buf);

	if(addr_out)
		*addr_out = addr_from;

	return i;
}