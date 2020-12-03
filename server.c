/*Computer Network
 * 2019014266 Lim Kyu Min
 * Server.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 5000
#define BUF_LEN 1024

int main(int argc, char* argv[]){
	//Declaration
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	char read_buffer[BUF_LEN];
	char send_buffer[BUF_LEN];
	char* ptr;

	int serv_fd;
	int cli_fd;
	int cli_addr_len;
	int child_pid;
	int n;
	int ret;
	int bytes_recv;

	//Socket Initialization
	if((serv_fd = socket(AF_INET,SOCK_STREAM,0)) < 0 ){
		printf("[SERVER]: Socket Failed to Open. ERRORCODE: %d\n",errno);
		return -1;
	}

	//Binding
	memset(&server_addr,0x00,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if(bind(serv_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
		printf("[SERVER]: Server Binding Failed. ERRORCODE: %d\n",errno);
		return -1;
	}

	//Listening
	if(listen(serv_fd,5) < 0){ //Listening for 3 client
		printf("[SERVER]: Listening Failed. ERRORCODE: %d\n",errno);
		return -1;
	}

	printf("[SERVER]: Waiting for the connection request...\n\n");
	while(1){
		//Accept
		cli_addr_len = sizeof(client_addr);
		cli_fd = accept(serv_fd,(struct sockaddr*)&client_addr,&cli_addr_len);

		if(cli_fd < 0){
			printf("[SERVER]: Accept Failed ERRORCODE: %d\n",errno);
			return -1;
		}

		//MultiProcessing
		child_pid = fork();//fork a child
		
		if(child_pid == 0){//Child process
			
			printf("[SERVER]: Request from the client accepted, allocating new child process.\n");

			if((close(serv_fd)) < 0 ){
				printf("[SERVER]: Close Socket Failed. ERRORCODE: %d\n",errno);
				return -1;
			}

			bytes_recv = BUF_LEN;
			recv(cli_fd,read_buffer,bytes_recv,0);
			read_buffer[bytes_recv] = 0;
			printf("[SERVER - MESSAGE RECRIVED]: Received from client: ");
			fputs(read_buffer,stdout);
			//printf("\n")
			//buffer = "Reply from the server";

			strcpy(send_buffer,"Return Message from Server");
			if(send(cli_fd,send_buffer,sizeof(send_buffer),0) < 0){
				printf("[SERVER]: Sending Error. ERRORCODE: %d\n",errno);
				return -1;
			}

			printf("[SERVER]: End of the request, closing the client server\n");
			if((close(cli_fd)) < 0){
				printf("[SERVER]: Close Cient Socket Failed. ERRORCODE: %d\n", errno);
				return -1;
			}
			exit(0);
		}else if(child_pid < 0){
			printf("[SERVER]: Fork Failed. ERRORCODE: %d\n",errno);
			return -1;
		}
	}

}
