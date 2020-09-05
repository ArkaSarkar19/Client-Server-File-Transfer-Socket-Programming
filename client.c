#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>

#define PORT 9001

void error(char *message);
void recieve_file(int socket_client, FILE *fp);

int main()
{
	int socket_server;
	struct sockaddr_in address_socket;

	char buffer1[256] = {0};
	char buffer2[256] = {0};
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

    scanf("%s", &buffer1);

    int server_send = send(socket_server, buffer1,256,0);

    if(server_send < 0)
    {
    	error("Error sending to server.\n");
    }

    printf("File name sent\n");

    printf("Now recieving File.\n");

    FILE *fp = fopen(buffer1, "w");

    recieve_file(socket_server,fp);

    close(fp);
    close(socket_server);

    
}


void error(char *message)
{
	perror(message);
	exit(1);
}

void recieve_file(int socket_server, FILE *fp)
{
	char data[512] = {0};

	int bits = recv(socket_server, data, 512,0);
	if(bits < 0)
	{
		error("Error in recieving File.\n");
		// break;
		return;
	}
	// printf("%s\n", data);
	int write_bits = fwrite(data, sizeof(char), bits, fp);
	if(write_bits!=bits) error("Error in writing File.\n");
	bzero(data,512);
	
}