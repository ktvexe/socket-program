/*
*socket server v1.0
*
*
*
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
#include <dirent.h>

void create_socket(int* );
void setting_sockaddr_in(struct sockaddr_in* );
void assign_port(int* ,struct sockaddr_in* );
void listen_to_client(int* );
void accept_connect(int* ,int* ,struct sockaddr_in*,int*);
int handle_receive_choice(int* ,char* );	
void switch_function(int ,int ,int* );
void send_message(int fd,char buf[],int size);
void receive_message(int fd,char buf[],int size);

int main()
{
    int sockfd;
	int end = 0;
    struct sockaddr_in server_name;
    char buffer[1024] = "############################\nWelcom to Internet editer\n############################\nThere are some option you can choose below:\n############################\n(C)reate\n(E)dit\n(R)emove\n(L)ist\n(D)ownload\n(Q)uit\n############################\nWhich do you want to choose?(C,E,R,L,D or Q)\n";
    char buffer1[1];
    
	/* create socket */
	create_socket(&sockfd);

	/* initialize structure sockaddr_in */
    setting_sockaddr_in(&server_name );

    /* Assign a port number to socket */
	assign_port(&sockfd ,&server_name );

    /* make it listen to socket with max 20 connections */
	listen_to_client(&sockfd );

	printf("**********SOCKET SERVER**********\n");	
    
	/* infinity loop -- accepting connection from client forever */
    do{

        int clientfd;
        struct sockaddr_in client_addr;
        int addrlen = sizeof(client_addr);
		char tmp[30];
		printf("Waiting...\n");
        /* Wait and Accept connection */
		accept_connect( &clientfd, &sockfd, &client_addr, &addrlen);
		printf("Connect...\n");

		/* Send message */
        send(clientfd, buffer, sizeof(buffer), 0);
        bzero( buffer1,sizeof(buffer1) );
		
		/*receive function choice*/
		int choice_case = handle_receive_choice(&clientfd,buffer1 );	
		switch_function(choice_case ,clientfd, &end);
		
		//printf("case:%d",choice_case);

  		close(clientfd);
    }while(!end);

    close(sockfd);
    return 0;
}


void create_socket(int* sockfd){

    if((*sockfd = socket(PF_INET, SOCK_STREAM, 0) ) <0 ){
		printf("Create socket failed!\n");
		exit(1);
	}
}


void setting_sockaddr_in(struct sockaddr_in* server_name){

	bzero(server_name, sizeof(*server_name));
    server_name->sin_family = AF_INET;
    server_name->sin_port = htons(3000);
    /* this line is different from client */
    server_name->sin_addr.s_addr = htons(INADDR_ANY);
}


void assign_port(int* sockfd,struct sockaddr_in* server_name){

    if(bind(*sockfd, (struct sockaddr*)server_name, sizeof(*server_name)) ){
		printf("Server Bind Port failed!\n");
		exit(1);
	}
}


void listen_to_client(int* sockfd){

    if( listen(*sockfd, 20) ){
		printf("Server Listen failed!\n");
		exit(1);
	}
}


void accept_connect(int* clientfd,int* sockfd,struct sockaddr_in* client_addr,int* addrlen){
	 *clientfd = accept(*sockfd, (struct sockaddr*)client_addr, addrlen);
		if(*clientfd < 0){
			printf("Server Accept failed!\n");
			exit(1);
		}
}


int handle_receive_choice(int* clientfd,char* buffer1){
	receive_message(*clientfd,buffer1,sizeof(buffer1));
	if(*buffer1 =='c'||*buffer1=='C'){
		return 1;
	}
	else if(*buffer1 =='e'||*buffer1=='E'){
		return 2;
	}
	else if(*buffer1 =='r'||*buffer1=='R'){
		return 3;
	}
	else if(*buffer1 =='l'||*buffer1=='L'){
		return 4;
	}
	else if(*buffer1 =='d'||*buffer1=='D'){
		return 5;
	}
	else if(*buffer1 =='q'||*buffer1=='Q'){
		return 6;
	}
	else
		return -1;
}


void switch_function(int choice_case ,int clientfd,int* end){
	switch (choice_case){
		case 1:{
			FILE *fp;
			char create[30] = "What is the name of the file ?";
			send_message(clientfd,create,sizeof(create));
			receive_message(clientfd,create,sizeof(create));
			fp = fopen(create,"w");
			if (fp ==NULL){
				printf("Open File failed!\n");
				exit(1);
			}
			fclose(fp);
			break;
		}

		case 2:{
			char edit[128] = "please key in the file name which you want to edit:\n";
			char file_name[64];
			FILE* fpr,*fpa;
			send_message(clientfd,edit,sizeof(edit));
			receive_message(clientfd,edit,sizeof(edit));
			fpr = fopen(edit,"r+");
			if(fpr ==NULL){
        		bzero(edit ,sizeof(edit) );
				printf("file does not exit\n");
				strcpy(edit,":end_no_file");
				send(clientfd,edit,sizeof(edit),0);
				break;
			}
			fclose(fpr);
			strcpy(file_name,edit);
			fpa =fopen(edit ,"a+");
        	bzero(edit ,sizeof(edit) );
			strcpy(edit,"Please key in what you want to append to the file\nIf you work done,please key in :end to finish it");
			send_message(clientfd,edit,sizeof(edit));
			int edit_len =0;
			while(edit_len = recv(clientfd ,edit,sizeof(edit),0)){
				if (edit_len < 0){
        			bzero(edit ,sizeof(edit) );
					printf("Write file failed\n");
					strcpy(edit,"Write file failed\n");
					send(clientfd,edit,sizeof(edit),0);
					exit(1);
				}
				int cmp = strcmp(edit,":end\n");
				if(cmp ==0){
					break;
				}
				//fprintf(fpa,"%s\n",edit);
				fputs(edit,fpa);
				bzero(edit,sizeof(edit));
			
			}
			fclose(fpa);
			printf("edit done\n");

			break;
		}

		case 3:{
			char remove_buffer[50] = "which file do you want to delete ?";
			send_message(clientfd,remove_buffer,sizeof(remove_buffer));
			receive_message(clientfd,remove_buffer,sizeof(remove_buffer));
			remove(remove_buffer);

			break;
		}

		case 4:{
			char list[30] = "These are document of server:";
			DIR* dir;
			struct dirent* ptr;
			dir = opendir(".");
			send_message(clientfd,list,sizeof(list));
			while((ptr =readdir(dir)) != NULL){
        		bzero(list ,sizeof(list) );
				strcpy(list,ptr->d_name);
				if(strcmp(list,".") && strcmp(list,"..")){
					send_message(clientfd,list,sizeof(list));
				}
			}
			close(dir);
			strcpy(list,":end");
			send_message(clientfd,list,sizeof(list));

			break;
		}

		case 5:{
			char down[60] = " Please key in the file name which you want to download";
			FILE *fp;	
			send_message(clientfd,down,sizeof(down));
			receive_message(clientfd,down,sizeof(down));
			fp =fopen(down,"rb");
			if (fp == NULL){
				printf("Can not find the file:%s\n",down);
			}
			else{
				bzero(down,sizeof(down));
				int file_len =0;
				while((file_len = fread(down,sizeof(char),60,fp))>0){
					if(send(clientfd,down,file_len,0)<0){
						printf("Send file failed");
						exit(1);
					}
					bzero(down,sizeof(down));
				}
			}
			fclose(fp);

			break;
		}

		case 6:{
			char quit[30] = "Are you sure to quit ? (Y/N)";
			send_message(clientfd,quit,sizeof(quit));
			receive_message(clientfd,quit,sizeof(quit));
			if(*quit == 'y'||*quit =='Y'){
				printf("Close server\n");
				*end = 1;
				break;
			}
			break;
		}
		default:
			printf("err");
	}
	
	
}

void send_message(int fd,char* buf,int size){
	send(fd,buf,size,0);
    bzero(buf ,size );

}


void receive_message(int fd,char buf[],int size){
	int res=recv(fd, buf, size, 0);
    if(res < 0){
		printf("Recieve failed!\n");
		exit(1);
	}

}
