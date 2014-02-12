#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>

main(int argc , char ** argv){	
		
		
	
	int server_socket_descriptor,client_socket_descriptor;
	struct sockaddr_in server_address,sockaddr_in client_address;
	int result;
	
	//requesting a descriptor
	if(server_socket_descriptor	=	socket(AF_INET,SOCK_STREAM,0)==-1){
		perror("Error: %d" , errno);
		exit(EXIT_FAILURE);
	}
	
	//init the sockaddr_in struct corresponding to the server
	server_address.sin_family	=	AF_INET;
	server_address.sin_addr.s_addr=	htonl(INADDR_ANY);
	server_address.sin_port	=	htons(8080);
	
	//binding the server to a port
	if(bind(server_socket_descriptor,(struct sockaddr_in *) &server_address , sizeof(server_address))==-1){
		perror("Error: %d" , errno);
		exit(EXIT_FAILURE);
	}
	
	//
	if(listen(server_socket_descriptor)==-1){
		perror("Error: %d" , errno);
		exit(EXIT_FAILURE);
	}
	
	exit();	
}
