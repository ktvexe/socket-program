/*
*socket client v1.0
*
*
*
*
*@author LG Liu
*@link
*/

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <sys/un.h>

 #define SERVER_PORT 1111

/* Write TEXT to the socket given by file descriptor SOCKET_FD. */
void write_text (int socket_fd, const char* text)
{
/* Write the number of bytes in the string, including
NUL-termination. */
	int length = strlen (text) + 1;
	write (socket_fd, &length, sizeof (length));
/* Write the string. */
	write (socket_fd, text, length);
}

int main (int argc, char* const argv[])
{
	//const char* const socket_name = argv[1];
	const char* const message = argv[1];
	int socket_fd;
	struct sockaddr_in name;
/* Create the socket. */
	socket_fd = socket (PF_INET, SOCK_STREAM, 0);
/* Store the server’s name in the socket address. */
	name.sin_family = AF_INET;
	name.sin_port = htons(SERVER_PORT);
	name.sin_addr.s_addr = inet_addr("127.0.0.1");

	//strcpy (name.sun_path, socket_name);
/* Connect the socket. */
	connect (socket_fd,(struct sockaddr*) &name, sizeof(name));
/* Write the text on the command line to the socket. */
	send(socket_fd,message,sizeof(message),0);	
//	write_text (socket_fd, message);
	close (socket_fd);
	return 0;
}
