#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h> 
#include<unistd.h>
#include<string.h>
#define SA struct sockaddr

int main()
{
	int socket_connection=0, valread;
	struct sockaddr_in server;

	char *message = "Hello server";
	char buffer[1024]={0};
	socket_connection = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_connection < 0) {
		printf("FAIL: Socket creation");
		printf("\n");

		return 1;
	}

	printf("SUCCESS: Socket creation");
	printf("\n");

	server.sin_addr.s_addr = inet_addr("192.168.51.14");// htonl(INADDR_ANY);
	server.sin_family = AF_INET;
	server.sin_port = htons(8081); 

	if(connect(socket_connection, (SA*)&server, sizeof(server)) != 0) {
		printf("FAIL: Socket connection");
		printf("\n");

		return 1;
	}

	printf("SUCCESS: Socket connection");
	printf("\n");

	write(socket_connection, message, sizeof(message));

	read(socket_connection, buffer, sizeof(buffer));

	printf("Message from the server: %s", buffer);
	printf("\n");

	if((strncmp(buffer, "exit", 4)) == 0) {
		printf("Connection closed successfully!\n");
		return 0;
	}

	printf("Connection never closed. That's cool, I guess.....");
	printf("\n");


	return 0;
}