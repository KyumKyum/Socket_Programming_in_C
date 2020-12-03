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

#define PORT 5000
#define BUF_LEN 1024

int main(int argc, char* argv[]){
	//Declaration
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	char buffer[BUF_LEN];
	char* ptr;

	int serv_fd;
	int cli_fd;
	int cli_addr_len;
	int child_pid;
	int n;
	int bytes_recv;

	//Socket Initialization
	if((serv_fd = socket(AF_INET,SOCK_STREAM,0)) < 0 ){
		printf("[SERVER]: Socket Create Failed.\n");
		return -1;
	}

	//Binding
	memset(&server_addr,0x00,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if(bind(serv_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
		printf("[SERVER]: Server Binding Failed\n");
		return -1;
	}

	//Listening
	if(listen(serv_fd,3) < 0){ //Listening for 3 client
		printf("[SERVER]: Listening Failed\n");
		return -1;
	}

	printf("[SERVER]: Waiting for the connection request...\n");
	while(1){
		//Accept
		cli_addr_len = sizeof(client_addr);
		cli_fd = accept(serv_fd,(struct sockaddr*)&client_addr,&cli_addr_len);

		if(cli_fd < 0){
			printf("[SERVER]: Accept Failed\n");
			return -1;
		}

		printf("[SERVER]: Connection request from client received, accepted.\n");

		//MultiProcessing
		child_pid = fork();//fork a child
		if(child_pid == 0){//Child process
			if((close(serv_fd)) < 0 ){
				printf("[SERVER]: Close Socket Failed\n");
				return -1;
			}
			ptr = buffer;
			bytes_recv = BUF_LEN;
			while((n = recv(cli_fd,ptr,bytes_recv,0)) > 0){
				ptr = ptr + n;
				bytes_recv = bytes_recv - n;
			}
			send(cli_fd,buffer,BUF_LEN,0);
			exit(0);
		}else if(child_pid < 0){
			printf("[SERVER]: Fork Failed\n");
			return -1;
		}
		printf("[SERVER]: End of the request, closing the client server.\n");
		if((close(cli_fd)) < 0 ){
			printf("[SERVER]: Close Client Socket Failed\n");
			return -1;
		}
	}

}
