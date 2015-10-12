#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char* const argv[])
{
    int sockfd;
    struct sockaddr_in dest;
    char* message =argv[1];
//    char resp[10]="clientack";

    /* create socket */
    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    /* initialize value in dest */
    bzero(&dest, sizeof(dest));
    dest.sin_family = PF_INET;
    dest.sin_port = htons(8889);
    dest.sin_addr.s_addr = inet_addr("127.0.0.1");
    //inet_aton("127.0.0.1", &dest.sin_addr);

    /* Connecting to server */
    connect(sockfd, (struct sockaddr*)&dest, sizeof(dest));

    /* Receive message from the server and print to screen */
//    bzero(buffer, 128);
//    recv(sockfd, buffer, sizeof(buffer), 0);
//    printf("receive from server: %s\n", buffer);
    
	
	send(sockfd,argv[1],strlen(argv[1]),0);
    //bzero(message,strlen(message));
	/* Close connection */
    close(sockfd);

    return 0;
}
