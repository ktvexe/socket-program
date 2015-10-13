#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(int argc, char* const argv[])
{
    int sockfd;
    struct sockaddr_in dest;
	char buffer[1024];
	char choose[1];

    /* create socket */
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if( sockfd < 0)
     {
         printf("Create Socket Failed!\n");
         exit(1);
     }

    /* initialize value in dest */
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(3000);
    dest.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	//client don't have to bind,because which port client use is not important
    /* Connecting to server */
    if(connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) < 0){
		printf("Connect failed!\n");
		exit(1);
	}
    /* Receive message from the server and print to screen */
    bzero(buffer, 1024);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);
    
	//int len =strlen(argv[1]	);
	//send(sockfd,message,len,0);
    scanf("%c",choose);
	printf("Your choice: %c\n",*choose);

	send(sockfd,choose,sizeof(choose),0);
	
//    bzero(buffer, 1024);
   /*
   	int len=recv(sockfd, buffer, sizeof(buffer), 0);
    if(len < 0){
		printf("Recieve failed!\n");
		break;
	}
	*/
	
	if(*choose =='c'||*choose=='C'){
        char create[30] ;
		bzero(create,30);
    	int len =recv(sockfd, create, sizeof(create), 0);
		if(len < 0){
			printf("Recieve failed!\n");
			exit(1);
		}
    	printf("%s\n", create);
        bzero( create,sizeof(create) );
		scanf("%s",create);
		printf("The file name is: %s\n",create);
        send(sockfd,create,sizeof(create),0);
        bzero( create,sizeof(create) );
     //   int file=recv(clientfd, buffer1, sizeof(buffer1), 0);
 
    }

	else if(*choose =='r'||*choose=='R'){
        char remove_buffer[50] ;
		bzero(remove_buffer,50);
    	int len =recv(sockfd, remove_buffer, sizeof(remove_buffer), 0);
		if(len < 0){
			printf("Recieve failed!\n");
			exit(1);
		}
    	printf("%s\n", remove_buffer);
        bzero( remove_buffer,sizeof(remove_buffer) );
		scanf("%s",remove_buffer);
		printf("The file name is: %s\n",remove_buffer);
        send(sockfd,remove_buffer,sizeof(remove_buffer),0);
        bzero( remove_buffer,sizeof(remove_buffer) );
     //   int file=recv(clientfd, buffer1, sizeof(buffer1), 0);
 
    }
	
	//bzero(message,strlen(message));
	/* Close connection */
	
	
	close(sockfd);

    return 0;
}
