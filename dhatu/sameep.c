#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

void portQuery(int * port){
	printf("Enter the localhost port number");
	scanf("%d" , port);
}

void main(int argc , char ** argv){
int fd;	
int len;
struct sockaddr_in addr;
int port;
	if(argc>1){
		port	=	(int) atoi(argv[1]);
	}else {
		portQuery(&port);
	}
	
fd	=	socket(AF_INET,SOCK_STREAM,0);
addr.sin_family	=	AF_INET;
addr.sin_addr.s_addr	=	inet_addr("127.0.0.1");
addr.sin_port	=	htons(port);

len	=	sizeof(addr);
	while(1){
			if(connect(fd,(struct sockaddr*) &addr,len)==-1){
				perror("no host on the specified port\n");
				portQuery(&port);
				addr.sin_port	=	htons(port);	
			}else {
				break;
			}
			sleep(5);
	}
	
	
	char message[900]="GET / HTTP/1.0\r\nHost: localhost\r\n\r\n";
							
	fprintf(stdout,"before recv");				   
	if(write(fd,message,strlen(message))==-1){
		perror("tamsomaa jyotirgamaya:send");
		exit(1);
	}
	
	char mess[9000];
	memset(mess,'\0',9000);
	
	if(read(fd,mess,9000,0)==-1){
		perror("tamsomaa jyotirgamaya:send");
		exit(1);
	}
	
	printf("%s",mess);		
	close(fd);
	exit(0);
}
