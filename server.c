/*
*socket server v1.0
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
// #include <sys/un.h>
 #include <netinet/in.h>

 #define SERVER_PORT 1111

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

int main(int argc,char* const argv[]){
	//const char* const socket_name =argv[1];
	printf("0\n");
	int socket_fd, client_quit;
	struct sockaddr_in name;
	char buffer[1024];
	printf("a\n");
	socket_fd = socket (PF_INET, SOCK_STREAM, 0);
	
	bzero(&name, sizeof(name));
 	name.sin_family = AF_INET;
    name.sin_port = htons(SERVER_PORT);

	name.sin_addr.s_addr =INADDR_ANY;	
	//strcpy(name.sun_path,socket_name);
	bind(socket_fd,(struct sockaddr*) &name, sizeof( name));
	printf("aa\n");
	listen (socket_fd,5);

	do{
		struct sockaddr_in client_name;
		socklen_t client_name_len =sizeof(client_name);
		int client_socket_fd;

		client_socket_fd = accept (socket_fd,(struct sockaddr*) &client_name, &client_name_len);
		bzero(buffer,sizeof(buffer));
		recv(client_socket_fd,buffer,sizeof(buffer),0);
		client_quit = server (client_socket_fd);
		close (client_socket_fd);
	}while(!client_quit);
	
	printf("bb\n");
	close(socket_fd);
	//unlink(socket_name);

	return 0;
}



