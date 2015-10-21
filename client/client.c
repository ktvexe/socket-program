/*
*socket client v1.0
*
*practice socket client to do a online editer
*user can operate creating,editing,removing,listing or downloading file
*
*@author LG Liu
*@link https://github.com/ktvexe/socket-program
*/

#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/*create socket interface to be connected
  sockfd : socket descriptor
*/
void create_socket(int* sockfd);
/*initialize the parameter of socket
  client_name : basic struct of internet address
*/
void setting_sockaddr_in(struct sockaddr_in* client_name);
/*connect to other socket*/
void connect_to_server(int sockfd, struct sockaddr_in);
/*send message buffer to other socket
  fd : socket descriptor
  buf[] :char array of message
  size : size of char array of message
*/
void send_message(int fd,char buf[],int size);
/*receive message buffer from other socket*/
void receive_message(int fd,char buf[],int size);
/*do requirement*/
void switch_function(char* buf,int fd);

int main(int argc, char* const argv[])
{
    int sockfd;
	int end;
    struct sockaddr_in client_name;
	char buffer[1024];
	char choose[1];

    /* create socket */
	create_socket(&sockfd);

    /* initialize structure sockadd_in */
	setting_sockaddr_in(&client_name);
	
	/* Connecting to server */
	connect_to_server(sockfd, client_name);

	/* Receive message from the server and print to screen */
    bzero(buffer, 1024);
	receive_message(sockfd,buffer,sizeof(buffer));
    printf("%s", buffer);
    /*key in use requirement*/
    scanf("%c",choose);
	printf("Your choice: %c\n",*choose);
	/*send use choice to server*/
	send(sockfd,choose,sizeof(choose),0);
	/*do requirement*/
	switch_function(choose,sockfd);
	/* Close connection */
	close(sockfd);
	return 0;
}


void create_socket(int* sockfd){
    /*create socket and error checking
	  PF_INET : communication domain
	  SOCK_STREAM : communication semantics
	  0 : a particular protocol to be used with the socket
	*/
	*sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if( sockfd < 0){
         printf("Create Socket Failed!\n");
         exit(1);
     }
}


void setting_sockaddr_in(struct sockaddr_in* client_name){
    /*initialize structure of socket 
	  sin_family : family of address
	  sin_port : port to connect
	  sin_addr : IP address
	*/
	bzero(client_name, sizeof(*client_name));
    client_name->sin_family = AF_INET;
    client_name->sin_port = htons(3000);
    client_name->sin_addr.s_addr = inet_addr("127.0.0.1");

}

void connect_to_server(int sockfd, struct sockaddr_in client_name){
    /*connect to server and error checking*/
	if(connect(sockfd, (struct sockaddr*)&client_name, sizeof(client_name)) < 0){
		printf("Connect failed!\n");
		exit(1);
	}
}

void send_message(int fd,char* buf,int size){
	/*send message
	  0 : flag
	*/
	send(fd,buf,size,0);
    bzero(buf ,size );

}


void receive_message(int fd,char buf[],int size){
	/*receive message
	  0 : flag
	*/
	int res=recv(fd, buf, size, 0);
    if(res < 0){
		printf("Recieve failed!\n");
		exit(1);
	}

}


void switch_function(char* choose,int sockfd){
	/*create file*/
	if(*choose =='c'||*choose=='C'){
        char create[30] ;
		bzero(create,30);
		receive_message(sockfd,create,sizeof(create));
    	printf("%s\n", create);
        bzero( create,sizeof(create) );
		/*key in file name and send to server*/
		scanf("%s",create);
		printf("Create file name : %s\n",create);
		send_message(sockfd,create,sizeof(create));
    }

    /*edit file*/
	else if(*choose =='e'||*choose=='E'){
        char edit[128] ;
		FILE* fp;
		bzero(edit,sizeof(edit));
		receive_message(sockfd,edit,sizeof(edit));
    	printf("%s", edit);
        bzero( edit,sizeof(edit) );
		/*key in file name and send to server*/
		scanf("%s",edit);
		printf("Editing file : %s\n",edit);
		send_message(sockfd,edit,sizeof(edit));
		receive_message(sockfd,edit,sizeof(edit));
		int no_file =strcmp(edit,":end_no_file");
		if(no_file == 0){
			printf("File does no exist\n");
			exit(1);
		}
    	printf("%s\n", edit);
		bzero(edit,sizeof(edit));
		fgetc(stdin);
		/*send edit content to server*/
		while(1){
        	fgets(edit,sizeof(edit),stdin);
			send(sockfd,edit,sizeof(edit),0);
			int cmp =strcmp(edit,":end\n");
			if(cmp == 0){
				break;
			}
        	bzero( edit,sizeof(edit) );
		}
    	printf("Edit done\n");
    }
	
	/*delete file*/
	else if(*choose =='r'||*choose=='R'){
        char remove_buffer[50] ;
		bzero(remove_buffer,50);
		receive_message(sockfd,remove_buffer,sizeof(remove_buffer));
    	printf("%s", remove_buffer);
        bzero( remove_buffer,sizeof(remove_buffer) );
		/*key in file name and send to server*/
		scanf("%s",remove_buffer);
		printf("Remove file : %s\n",remove_buffer);
		send_message(sockfd,remove_buffer,sizeof(remove_buffer));
    }

	/*list file*/
	else if(*choose =='l'||*choose =='L'){
		char list[30];
		bzero(list,30);
		/*show the buffer from server*/
		while(1){
			receive_message(sockfd,list,sizeof(list));
			int cmp =strcmp(list,":end");
			if(cmp == 0){
				break;
			}
    		printf("%s\n", list);
        	bzero( list,sizeof(list) );
		}
	}
	

	/*download file*/
	else if(*choose =='d'||*choose =='D'){
		char down[60];
		FILE* fp;
		bzero(down,sizeof(down));
		receive_message(sockfd,down,sizeof(down));
    	printf("%s\n", down);
        bzero( down,sizeof(down) );
		/*key in file name and send to server*/
		scanf("%s",down);
        send(sockfd,down,sizeof(down),0);
		/*open file and error checking*/
		fp = fopen(down,"wb");
		if(fp == NULL ){
			printf("Can not open the file\n");
			exit(1);
		}
        bzero( down,sizeof(down) );
		int down_len =0;
		/*receive file content from server and write to client*/
    	while(down_len =recv(sockfd, down , 60, 0)){
			if(down_len < 0){
				printf("Recieve file failed!\n");
				break;
			}
			int write_len = fwrite(down,sizeof(char),down_len,fp);
		 	if (write_len<down_len){
            	printf("Write file failed\n");
            	break;
        	}
        	bzero(down,sizeof(down)); 	
		}
		printf("Receive file\n");
		/*close file*/
		fclose(fp);
	}

	/*quit socket*/
	else if(*choose =='q'||*choose =='Q'){
		char quit[30];
		bzero(quit,30);
		receive_message(sockfd,quit,sizeof(quit));
    	printf("%s\n", quit);
        bzero( quit,sizeof(quit) );
		scanf("%s",quit);
		send_message(sockfd,quit,sizeof(quit));
	}

}
