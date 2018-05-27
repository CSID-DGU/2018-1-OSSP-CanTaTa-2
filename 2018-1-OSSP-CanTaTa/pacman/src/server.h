#include <sys/socket.h>

#define PORT 32421
#define BUFMAX 255

int s_socket_fd;
int c_socket_fd;
struct sockaddr_in  server_address,  client_address;
int client_address_size;
char Msg2Client[BUFMAX];
char Msg2Server[BUFMAX];

void makeServer();
