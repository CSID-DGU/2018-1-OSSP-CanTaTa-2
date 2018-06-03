//#25 kim : 1. 클라이언트 코드 추가.
#include <arpa/inet.h>
#define PORT 32421
#define BUFMAX 255
int clientSocket;
struct sockaddr_in server_address;
char toServer[BUFMAX];
char fromServer[BUFMAX];
void connectServer(char* ip);
