#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "renderer.h"
#define PORT 32421

int main()
{
	int s_socket_fd;
	int c_socket_fd;
	struct sockaddr_in  server_address,  client_address;

	int client_address_size;
	char Msg2Client[]="Hello Client!!\n";
	char Msg2Server[100];
	/*클라이언트의 접속을 기다리는 서버소켓 객체 생성*/
	s_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	draw_wait_client("MAKE ROOM....");

	/*서버의 IP 와 PORT 를 저장할 server_address 객체를 0으로 초기화 시킴*/
	memset(&server_address, 0, sizeof(server_address));
	/*server_address 에 서버의 주소 정보 대입*/
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=htonl(INADDR_ANY);
	server_address.sin_port=htons(PORT);


	bind(s_socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) ;
	/*클라이언트의 접속을 기다리는 대기 모드로 전환*/
   listen(s_socket_fd, 5);



   client_address_size = sizeof(client_address);
    /*클라이언트가 서버에 접속하면 접속을 수락함*/
   c_socket_fd = accept( s_socket_fd, (struct sockaddr*)&client_address, &client_address_size);


   printf("Client Connect!!!\n");
    /*클라이언트가 전달한 메시지를 읽어 들여서 fromClient에 대입*/
   //char fromClient[256];

   while(1){
	   /* 수신 받을 버퍼 생성 */
	   char buffer[255];
	   memset(buffer,0,sizeof(buffer));

	   /* 데이터 수신 받음 */
	   ssize_t numbytesrcvd = recv(c_socket_fd,buffer,255,0);

	   /* 수신받은 데이터 출력*/
	   printf(": %s  ",buffer);
	   buffer[strlen(buffer)-1] = '\0';
	   if(strcmp(buffer,"/quit") == 0)
	   break;
	   memset(buffer,0,sizeof(buffer));
	   /* 서버측 입력받을 버퍼 생성   */
	   char message[255];
	   memset(message,0,sizeof(message));
	   /* 보낼 메시지 입력 받음 */
	   printf("Server : ");
	   fgets(message,255,stdin);

	   /* 서버측 문자열 입력 버퍼 센드 */
	   send(c_socket_fd,message,strlen(message),0);
//
//	   if(read(c_socket_fd, Msg2Server ,sizeof(Msg2Server)))
//	   {
//		printf("From Client Message: %s\n",Msg2Server);
//	   }
//	   scanf("%s",tosv);
//	   	write(c_socket_fd, tosv, sizeof(tosv));
   }
	close(c_socket_fd);

	printf("d");
	return 0;
}
