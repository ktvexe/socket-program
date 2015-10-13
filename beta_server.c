#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int server(int client);

int main()
{
    int sockfd,client_quit;
    struct sockaddr_in dest;
    char buffer[1024] = "############################\nWelcom to Internet editer\n############################\nThere are some option you can choose below:\n############################\n(C)reate\n(E)dit\n(R)emove\n(L)ist\n(D)ownload\n############################\nWhich do you want to choose?(C,E,R,L or D)\n";
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
    do
    {
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
			char create[30] = "What is the name of the file ?";
			send(clientfd,create,sizeof(create),0);
        	bzero( buffer1,sizeof(buffer1) );
			int file=recv(clientfd, create, sizeof(create), 0);
			
		}
			


/*        switch(buffer1){
			case
		}
*/
		//write(fd, buffer1, sizeof(buffer1));
        /* close(client) */
		//client_quit = server(clientfd);
        close(clientfd);
    }while(!client_quit);

    /* close(server) , but never get here because of the loop */
    close(sockfd);
    return 0;

}
int server(int client){
  //   while(1){
         int length;
         char* text;
 
         //if(read(client, &length ,sizeof(length))==0)
         //    return 0;
 
         text = (char*)malloc (length);
 
         read( client,text, length);
         printf( "%s", text);
         free( text );
         if(!strcmp (text,"quit"))
             return 1;
		return 0;
//    }
 }

