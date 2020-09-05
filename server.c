#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <dirent.h>
#define PORT 9001
void error(char *message)
{
	perror(message);
	exit(0);
}

int main(){
	int server_socket;
	int client_socket;
	int addLen = sizeof(server_socket);
	struct sockaddr_in address_socket;
	address_socket.sin_family = AF_INET;
	address_socket.sin_port = htons(PORT);
 	address_socket.sin_addr.s_addr = INADDR_ANY;
	server_socket = socket(AF_INET, SOCK_STREAM, 0 );

	if(server_socket < 0)
	{
		error("Errpr in opening socket \n");
	}

	

 	int server_bind = bind(server_socket,(struct sockaddr *) &address_socket,  sizeof(address_socket));
    if(server_bind == -1){
        error("Error establishing a server socket connection \n");
    }

    int listen_server = listen(server_socket,0);

    if(listen_server < 0)
    {
    	error("Error while Listening from Server. \n");
    }
    printf("Server Online\n");

    char buffer[256] = {};
    client_socket = accept(server_socket, (struct sockaddr *)&address_socket, (socklen_t *)&addLen);

    if(client_socket < 0)
    {
    	error("Can't Connect to client\n");
    }
 
	printf("Client Connected \n");
	int status = recv(client_socket,buffer,256,0);
    if(status == -1){
        error("Error in recieving message");
    }
	printf("Client : %s\n", buffer);
	 
	// printf("Enter Text to send w/o space\n");
	struct dirent *de;

	DIR *dr = opendir("shared_drive");

	if(dr == NULL)
	{
		error("Could not open directory. \n");
	}

	while ((de = readdir(dr)) != NULL) 
            printf("%s\n", de->d_name); 
  
    closedir(dr);

	// send(client_socket, textToSend, strlen(textToSend), 0);
	 
	 
	 
	close(client_socket);

}