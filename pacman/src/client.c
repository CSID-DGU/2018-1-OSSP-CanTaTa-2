//#25 kim : 1. 클라이언트 코드 추가.
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

void connectServer(char* ip)  {

	int str_len;
	/*서버에 접속해서 데이터를 입출력 하는 클라이	언트 소켓생성*/
	clientSocket=socket(PF_INET, SOCK_STREAM, 0);
	printf("Create Client Socket\n");
	/*서버의 주소 정보가 저장될 server_adress 를 0으로 초기화*/
	memset(&server_address, 0, sizeof(server_address));
	/*server_address 에 서버의 주소 정보 대입*/
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=inet_addr(ip);
	server_address.sin_port=htons(PORT);
	/*서버에 접속*/
	connect(clientSocket, (struct sockaddr*)&server_address, sizeof(server_address));
	printf("Connect Server!!\n");
	/*toServer 에 저장된 메시지를 서버로 전송*/
}

