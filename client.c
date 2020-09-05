#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>

#define PORT 9001
void error(char *message)
{
	perror(message);
	exit(0);
}

int main()
{
	int socket_server;
	struct sockaddr_in address_socket;

	char buffer1[256];
	char buffer2[256];
	address_socket.sin_family = AF_INET;    
 	address_socket.sin_port = htons(PORT);   
 	address_socket.sin_addr.s_addr = INADDR_ANY;   

 	socket_server =  socket(AF_INET, SOCK_STREAM,0);

 	if(socket_server < 0)
 	{
 		error("Error opening socket. \n");
 	}
 	int socket_connection = connect(socket_server,(struct sockaddr *)&address_socket, sizeof(address_socket));
    
    if(socket_connection==-1){
        error("Cannot connect to server \n");
    }
    printf("Enter the file name to send : ");

    fgets(buffer1,256,stdin);

    int server_send = send(socket_server, buffer1,256,0);

    if(server_send < 0)
    {
    	error("Error sending to server.\n");
    }

    printf("File name sent\n");


    close(socket_server);
}