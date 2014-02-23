#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<sys/select.h>
#include<string.h>
#include<sys/ioctl.h>
#define BACKLOG 10

int handleWebRequest(char data[])	{
	//got a string full of request
	printf("data :  %s \n" , data);
	return 0;
}

main(int argc , char ** argv){	
	int server_socket_descriptor,client_socket_descriptor;
	struct sockaddr_in server_address ,client_address;
	int result;
	fd_set fd_list;	//a list of file descriptors
	
	struct timeval timeout; //the time for which select will block
		timeout.tv_sec	=	1;
		timeout.tv_usec=0;
	
	//file descriptor inits
	FD_ZERO(&fd_list);
	
	//requesting a descriptor
	
	if((server_socket_descriptor	= socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("Problem in getting socket descriptor , Error: %d" , errno);
		exit(EXIT_FAILURE);
	}
	
	//putting the server socket in the file descriptor list
	FD_SET(server_socket_descriptor,&fd_list);
	
	//init the sockaddr_in struct corresponding to the server
	server_address.sin_family	=	AF_INET;
	server_address.sin_addr.s_addr=	inet_addr("127.0.0.1");
	server_address.sin_port	=	htons(5000);
	
	int server_size	=	sizeof(server_address);
	
	//binding the server to a port
	if(bind(server_socket_descriptor,(struct sockaddr *) &server_address , server_size)==-1){
		printf("Problem in binding , Error: %d" , errno);
		exit(EXIT_FAILURE);
	}
	
	//queing the connections
	if(listen(server_socket_descriptor,BACKLOG)==-1){
		printf("Problem at the listen call , Error: %d" , errno);
		exit(EXIT_FAILURE);
	}
	
	while(1){
		//the message loop to the server
		printf("server waiting for connection");
		result	=	select(FD_SETSIZE,&fd_list,(fd_set*)0,(fd_set*)0,(struct timeval *)0);	
		int fd;
			for(fd=0;fd<FD_SETSIZE;fd++){
				if(FD_ISSET(fd,&fd_list)){
					//the descriptor is present in the list
					if(server_socket_descriptor==fd){
						//this is the server socket
						int len	=	sizeof(client_address);
						int client_socket_descriptor	=	accept(server_socket_descriptor,(struct sockaddr *)&client_address,&len);
						printf("got a new connection %d" ,client_socket_descriptor);
						//got connection , means the descriptor is not in the list
						FD_SET(client_socket_descriptor,&fd_list);
					}else {
					//not the server descriptor i.e some other socket
						char data[1200];
						int char_read=0;		//the number of characters read
						memset(data,'\0',sizeof(data));
						
						ioctl(fd , FIONREAD , &char_read); //this just to check how much data do we have in the buffer
							//for read
								if(char_read==0){
								//nothing read means the connnection wants to close
								close(fd);
								FD_CLR(fd,&fd_list);
								printf("connection with client descriptor %d ended" , fd);
								}else {
									//means got full or partial data
									read(fd,data,sizeof(data));
									int result	=	handleWebRequest(data);
									char routine[100]="this better reach";
									if(write(fd,routine,strlen(routine))==-1){
										perror("error in sending message to client \n");
										exit(EXIT_FAILURE);
									}
									
								}
					}
				} //fd_isset
			}//for loop
	}//message loop
	exit(EXIT_SUCCESS);	
}
