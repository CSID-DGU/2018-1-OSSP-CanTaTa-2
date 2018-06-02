#include <sys/socket.h>

#include <arpa/inet.h>
#include <sys/types.h>
#define PORT 32421
#define BUFMAX 255

int s_socket_fd;
int c_socket_fd;
struct sockaddr_in  server_address,  client_address;
int client_address_size;
char Msg2Client[BUFMAX];
char Msg2Server[BUFMAX];

void makeServer(void);
void get_client_position(void);//#20 Kim 2: 주고받는 부분 함수로 나눴음
