#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <dirent.h>
#define PORT 9001

void error(char *message);
int check_file(char *filename);
void send_file(FILE *fp, int client_socket);

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

    char buffer[256] = {0};
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
	 
	int file_c = check_file(buffer);
	if(file_c == 0)
	{
		error("File Not Found : Invalid Filename.\n");
	}
	// send(client_socket, "textToSend", strlen("textToSend"), 0);
	 
	char fileToSend[5000] = {0};
	FILE *fp;
	char PATH[512] = "shared_drive/";
	strcat(PATH, buffer);

	printf("File PATH : %s\n",	PATH );

	fp = fopen(PATH,"r");

	printf("%s\n", fileToSend);

	char stash[100] = {0};
	// printf("Press Enter to Continue");
	// while( getchar() != '\n' );

	send_file(fp, client_socket);

	close(fp);
	close(server_socket);
	close(client_socket);

}

void error(char *message)
{
	perror(message);
	exit(1);
}

int check_file(char *filename)
{
	struct dirent *de;

	DIR *dr = opendir("shared_drive");

	if(dr == NULL)
	{
		error("Could not open directory. \n");
	}

	while ((de = readdir(dr)) != NULL){
		     // printf("%s\n", de->d_sockname); 
		     // printf("%d\n", strcmp(de->d_name,filename));
		     if(strcmp(de->d_name,filename) == 0)
		     {
		     	printf("File found : %s  \n", filename );
		     	return 1;
		     }


	} 
  
    closedir(dr);
    return 0;

}

void send_file(FILE *fp, int client_socket)
{
	if(fp == NULL) error("File cannot be opened.\n");
	int n;

    char data[1024] = {0}; 
    while ((n = fread(data, sizeof(char), 1024, fp)) > 0) 
    {
	    
        printf("%s\n", data);
        if (send(client_socket, data, n, 0) < 0)
        {
            error("Error Sending file to client");
        }
        bzero(data,1024);
    }

    if (send(client_socket, "end_of_file", sizeof("end_of_file"), 0) < 0)
    {
    	error("Error sending EOF.\n");
    }
	
}