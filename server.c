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

int server(int client);

int main(int argc,char* const argv[]){
	const char* const socket_name =argv[1];
	int socket_fd, client_quit;
	struct sockaddr_un name;

	socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
	name.sun_family = AF_LOCAL;
	strcpy(name.sun_path,socket_name);
	bind(socket_fd, &name, SUN_LEN( &name));

	listen (socket_fd,5);

	do{
		struct sockaddr_un client_name;
		socklen_t client_name_len;
		int client_socket_fd;

		client_socket_fd = accept (socket_ffd, &client_name, &client_name_len);

		client_quit = server (client_socket_fd);
		close (client_socket_fd);
	}while(!client_quit);
	

	close(socket_fd);
	unlink(socket_name);

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
		print( "%s", text);
		free( text );
		if(!strcmp (text,"quit"))
			return 1;
	}
}

