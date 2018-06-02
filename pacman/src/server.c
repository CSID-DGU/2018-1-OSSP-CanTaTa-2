#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "renderer.h"

void makeServer(void)//#20 Kim 2: 일단 채팅 프로그램 복사임
{
	/*클라이언트의 접속을 기다리는 서버소켓 객체 생성*/
	s_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	printf("서버 만들어졌다.\n");
	/*서버의 IP 와 PORT 를 저장할 server_address 객체를 0으로 초기화 시킴*/


	memset(&server_address, 0, sizeof(server_address));
	/*server_address 에 서버의 주소 정보 대입*/
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=htonl(INADDR_ANY);
	server_address.sin_port=htons(PORT);
	bind(s_socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) ;
	/*클라이언트의 접속을 기다리는 대기 모드로 전환*/
   listen(s_socket_fd, 5);
   printf("클라이언트 접속 기다림.\n");

   client_address_size = sizeof(client_address);
    /*클라이언트가 서버에 접속하면 접속을 수락함*/
   c_socket_fd = accept( s_socket_fd, (struct sockaddr*)&client_address, &client_address_size);

   printf("Client Connect!!!\n");
    /*클라이언트가 전달한 메시지를 읽어 들여서 fromClient에 대입*/
   //char fromClient[256];


}
void get_client_position(void)//#20 Kim 2: 주고받는 부분 함수로 나눴음
{//#20 Kim : 1. test 를 위해서 그냥 임시로 메세지 주고받는거 추가
	/* 수신 받을 버퍼 생성 */
	char buffer[255];
	memset(buffer,0,sizeof(buffer));

	/* 데이터 수신 받음 */
	ssize_t numbytesrcvd = recv(c_socket_fd,buffer,255,0);

	/* 수신받은 데이터 출력*/
	//printf(": %s  ",buffer);
	buffer[strlen(buffer)-1] = '\0';

	memset(buffer,0,sizeof(buffer));
	/* 서버측 입력받을 버퍼 생성   */
	char message[255];
	memset(message,0,sizeof(message));
	/* 보낼 메시지 입력 받음 */
//	printf("Server : ");
	message[0]='d';
	message[1]='\0';
	//fgets(message,255,stdin);
	send(c_socket_fd,message,strlen(message),0);
}
