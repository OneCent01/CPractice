#include<stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/socket.h> 


int main()
{
	int socket_connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(socket_connection == -1) {
		printf("FAIL: Socket creation\n");
		return 1;
	} 

	printf("SUCCESS: Socket creation\n");
	
	struct sockaddr_in server = {0};
	
	server.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.51.14");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8090 );
	
	if(connect(socket_connection, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("FAIL: Socket connection\n");
		return 1;
	} 
	
	printf("SUCCESS: Socket connection\n");

	return 0;
}
