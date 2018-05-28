#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 32421

int main()  {
	int clientSocket;
	struct sockaddr_in  server_address;

	char toServer[]="Hello Server!!! I'm Client!!";

	char fromServer[100];
	int str_len;
	/*서버에 접속해서 데이터를 입출력 하는 클라이언트 소켓생성*/
	clientSocket=socket(PF_INET, SOCK_STREAM, 0);
	printf("Create Client Socket\n");
	/*서버의 주소 정보가 저장될 server_adress 를 0으로 초기화*/
	memset(&server_address, 0, sizeof(server_address));
	/*server_address 에 서버의 주소 정보 대입*/
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_address.sin_port=htons(PORT);
	/*서버에 접속*/
	connect(clientSocket, (struct sockaddr*)&server_address, sizeof(server_address));
	printf("Connect Server!!\n");
	/*toServer 에 저장된 메시지를 서버로 전송*/


	while(1)
	{


		 /* 입력 받을 버퍼 생성  */
		 char message[255];
		 memset(message,0,strlen(message));

		 /* 사용자로부터 입력 받음 */
		 //printf("Client : ");
		 message[0]='o';
		 message[1]='\0';
		 //fgets(message,255,stdin);


		 /* 입력 받은 문자 센드 */
		 size_t echoStringLen = strlen(message);
		 ssize_t numBytes = send (clientSocket,message, echoStringLen, 0);


		 /* 입력 받은 문자가 /quit 일 경우 브레이크 */
		 if(strcmp(message,"/quit\n") == 0)
		  break;

		 /* 서버측으로부터 리시브  */
		 char buffer[255];
		 memset(buffer,0,sizeof(buffer));
		 numBytes = recv(clientSocket,buffer,255,0);


		 if(strcmp(buffer,"/quit\n") == 0)
		  break;
		 //printf("from : %s ",buffer);
		 memset(buffer,0,sizeof(buffer));




	}

	close(clientSocket);
	return 0;
}
