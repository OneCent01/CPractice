#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h> 
#include<unistd.h>
#define SA struct sockaddr

int main()
{
	int socket_connection = socket(AF_INET, SOCK_STREAM, 0);
	int live_conn, len, valread;
	char buffer[1024] = {0}, *message="Hello, client";

	if(socket_connection == -1) {
		printf("FAIL: Socket creation\n");
		return 1;
	} 

	printf("SUCCESS: Socket creation\n");
	
	struct sockaddr_in server;
	
	server.sin_addr.s_addr =  inet_addr("192.168.51.14");// htonl(INADDR_ANY);
	server.sin_family = AF_INET;
	server.sin_port = htons(8081); 
	
	if(bind(socket_connection, (SA*)&server, sizeof(server)) < 0) {
		printf("FAIL: Socket connection\n");
		return 1;
	} 
	
	printf("SUCCESS: Socket connection\n");


	if((listen(socket_connection, 5)) != 0) {
		printf("FAIL: Socket listening :(\n");
		return 1;
	}

	printf("SUCCESS: Socket listening...\n");

	len = sizeof(server);

	live_conn = accept(socket_connection, (SA*)&server, (socklen_t*)&len);

	if(live_conn < 0) {
		printf("FAIL: Socket accepting connection :(\n");
		return 1;
	}

	printf("SUCCESS: Socket accepting connections!\n");

	valread = read(live_conn, buffer, 1024);
	printf("Incoming request: %s", buffer);
	printf("\n");

	send(live_conn, message, sizeof(message), 0);

	printf("Message sent. Closing socket...");
	printf("\n");


	close(live_conn);

	printf("SOCKET CLOSED.");
	printf("\n");

	return 0;
}
