#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int server(int client);

int main()
{
    int sockfd,client_quit;
    struct sockaddr_in dest;
    char buffer[20] = "Hi client! ";
    char buffer1[1024];
    /* create sockett */
    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    /* initialize structure dest */
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(8889);
    /* this line is different from client */
    dest.sin_addr.s_addr = INADDR_ANY;

    /* Assign a port number to socket */
    bind(sockfd, (struct sockaddr*)&dest, sizeof(dest));

    /* make it listen to socket with max 20 connections */
    listen(sockfd, 20);

    /* infinity loop -- accepting connection from client forever */
    do
    {
        int clientfd;
        struct sockaddr_in client_addr;
        int addrlen = sizeof(client_addr);

        /* Wait and Accept connection */
        clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

        /* Send message */
        //send(clientfd, buffer, sizeof(buffer), 0);
        int res=recv(clientfd, buffer1, sizeof(buffer1), 0);
        printf("receive from client: %s, %d bytes\n", buffer1,res);
        //write(fd, buffer1, sizeof(buffer1));
        /* close(client) */
		client_quit = server(clientfd);
        close(clientfd);
    }while(!client_quit);

    /* close(server) , but never get here because of the loop */
    close(sockfd);
    return 0;

}
int server(int client){
     while(1){
         int length;
         char* text;
 
         if(read(client, &length ,sizeof(length))==0)
             return 0;
 
         text = (char*)malloc (length);
 
         read( client,text, length);
         printf( "%s", text);
         free( text );
         if(!strcmp (text,"quit"))
             return 1;
    }
 }

