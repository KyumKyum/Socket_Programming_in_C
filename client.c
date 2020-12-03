/*Computer Network ENE 4019
 2019014266 Lim Kyu Min
 client.c*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUF_LEN 1024
#define IP "127.0.0.1" //Loop back
#define PORT 5000

int main(int argc, char* argv[]){
	struct sockaddr_in serv_addr; //Socket Address
	
	char send_buffer[BUF_LEN] = {'\0'}; //Send Buffer
	char recv_buffer[BUF_LEN] = {'\0'}; //Receive Buffer
	char* ptr; //Pointer for the received buffer

	int my_fd; //My socket discriptor
	int n; //Number of bytes received
	int bytes_recv; //Number of bytes to receive
	int i = 0;
	int cnt = 3;//Create three sockets.
	//Create Socket

	if((my_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		printf("[CLIENT] Failed to open socket\n");
		return -1;
	}

	//printf("[CLIENT]: All sockets opened without error.\n")

	//Create Server Socket Address
	memset(&serv_addr,0x00,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	if(inet_pton(AF_INET,IP,&serv_addr.sin_addr) != 1){
		printf("[CLIENT]: inde_pton ERROR!\n");
		return -1;
	}
	//serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_port = htons(PORT);

	//Connect
	if(connect(my_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
		printf("CLIENT: Failed to connect to server\n");
		printf("%d\n",errno);
		return -1;
	}

	//Send and receive data
	//fgets(send_buffer,BUF_LEN,stdin);
	strcpy(send_buffer,"Message from Client");
	if(send(my_fd,send_buffer,strlen(send_buffer),0) < 0){//Sending
		printf("CLIENT: Send Error\n");
		return -1;
	}

	bytes_recv = BUF_LEN;
	recv(my_fd,recv_buffer,bytes_recv,0); //Receiving
	recv_buffer[bytes_recv] = 0;
	
	//if(read(my_fd,recv_buffer,sizeof(recv_buffer)-1) < 0){
	//	printf("[CLIENT]: Read Error ERRORCODE: %d\n",errno);
	//	return -1;
	//}
	printf("[CLIENT] Received From Server: ");
	fputs(recv_buffer,stdout);
	printf("\n");

	//Close and exit;
	close(my_fd);
	exit(0);
}//End of client
