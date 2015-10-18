/*
*socket server v1.0
*
*
*
*
*@author LG Liu
*@link
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



int main()
{
    int sockfd;
    struct sockaddr_in dest;
    char buffer[1024] = "############################\nWelcom to Internet editer\n############################\nThere are some option you can choose below:\n############################\n(C)reate\n(E)dit\n(R)emove\n(L)ist\n(D)ownload\n(E)xit\n############################\nWhich do you want to choose?(C,E,R,L or D)\n";
    char buffer1[1024];
    /* create socket */
    if((sockfd = socket(PF_INET, SOCK_STREAM, 0) ) <0 ){
		printf("Create socket failed!\n");
		exit(1);
	}

    /* initialize structure dest */
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(3000);
    /* this line is different from client */
    dest.sin_addr.s_addr = htons(INADDR_ANY);

    /* Assign a port number to socket */
    if(bind(sockfd, (struct sockaddr*)&dest, sizeof(dest)) ){
		printf("Server Bind Port failed!\n");
		exit(1);
	}

    /* make it listen to socket with max 20 connections */
    if( listen(sockfd, 20) ){
		printf("Server Listen failed!\n");
		exit(1);
	}

    /* infinity loop -- accepting connection from client forever */
    while(1){

        int clientfd;
        struct sockaddr_in client_addr;
        int addrlen = sizeof(client_addr);
		char tmp[30];

        /* Wait and Accept connection */
        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
		if(clientfd < 0){
			printf("Server Accept failed!\n");
			break;
		}
        /* Send message */
        send(clientfd, buffer, sizeof(buffer), 0);
        bzero( buffer1,sizeof(buffer1) );
		int res=recv(clientfd, buffer1, sizeof(buffer1), 0);
        if(res < 0){
			printf("Recieve failed!\n");
			break;
		}
		//client_quit = server(clientfd);
		printf("receive from client: %s, %d bytes\n", buffer1,res);
       // bzero( tmp,sizeof(tmp) );
		//strcpy(tmp ,"Your choice :");
       // send(clientfd, tmp, sizeof(tmp), 0);

		if(*buffer1 =='c'||*buffer1=='C'){
			FILE *fp;
			char create[30] = "What is the name of the file ?";
			send(clientfd,create,sizeof(create),0);
        	bzero( create,sizeof(create) );
			int file=recv(clientfd, create, sizeof(create), 0);
        	if (file < 0){
				printf("Recieve failed!\n");
				break;
			}
			fp = fopen(create,"a+");
			if (fp ==NULL){
				printf("Open File failed!\n");
				exit(1);
			}
			fclose(fp);
		}
		else if(*buffer1 =='r'||*buffer1=='R'){
		//	FILE *fp;
			char remove_buffer[50] = "which file do you want to delete ?";
			send(clientfd,remove_buffer,sizeof(remove_buffer),0);
        	bzero(remove_buffer ,sizeof(remove_buffer) );
			int file=recv(clientfd,remove_buffer, sizeof(remove_buffer), 0);
        	if (file < 0){
				printf("Recieve failed!\n");
				break;
			}
			remove(remove_buffer);
		/*	fp = fopen(create,"a+");
			if (fp ==NULL){
				printf("Open File failed!\n");
				exit(1);
			}
			fclose(fp);
		*/
		}
		
		else if (*buffer1=='d'||*buffer1=='D'){
			char down[60] = " Please key in the file name which you want to download";
			char file_name[512+1];
			bzero(file_name, 513);
			FILE *fp;	
			send(clientfd,down,sizeof(down),0);
        	bzero(down ,sizeof(down) );
			if(recv(clientfd,down, sizeof(down), 0) < 0){
				printf("Recieve failed!\n");
				break;
			}
			strncpy(file_name,down,strlen(down)>512?512:strlen(down));
			fp =fopen(file_name,"rb");
			if (fp == NULL){
				printf("Can not find the file:%s\n",down);
			}
			else{
				bzero(down,sizeof(down));
				int file_len =0;
				while((file_len = fread(down,sizeof(char),60,fp))>0){
					printf("file_len=%d\n",file_len);
					
					if(send(clientfd,down,file_len,0)<0){
						printf("Send file failed");
						break;
					}
					bzero(down,sizeof(down));
				}
			}
			fclose(fp);
		}

		else if (*buffer1=='e'||*buffer1=='E'){
			char quit[30] = " Are you sure to exit ? (Y/N)";
			send(clientfd,quit,sizeof(quit),0);
        	bzero(quit ,sizeof(quit) );
			if(recv(clientfd,quit, sizeof(quit), 0) < 0){
				printf("Recieve failed!\n");
				break;
			}
			if(*quit == 'y'||*quit =='Y')
				break;
			
		}
			


/*        switch(buffer1){
			case
		}
*/
		//write(fd, buffer1, sizeof(buffer1));
        /* close(client) */
        close(clientfd);
    }

    /* close(server) , but never get here because of the loop */
    close(sockfd);
    return 0;

}

