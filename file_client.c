#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero

#define HELLO_WORLD_SERVER_PORT    6666
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: ./%s ServerIPAddress\n",argv[0]);
        exit(1);
    }
    //設置一個socket位址結構client_addr,代表客戶機internet位址, 埠
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr)); //把一段記憶體區的內容全部設置為0
    client_addr.sin_family = AF_INET;    //internet協議族
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);//INADDR_ANY表示自動獲取本機位址
    client_addr.sin_port = htons(0);    //0表示讓系統自動分配一個空閒埠
    //創建用於internet的流協議(TCP)socket,用client_socket代表客戶機socket
    int client_socket = socket(AF_INET,SOCK_STREAM,0);
    if( client_socket < 0)
    {
        printf("Create Socket Failed!\n");
        exit(1);
    }
    //把客戶機的socket和客戶機的socket位址結構聯繫起來
    if( bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr)))
    {
        printf("Client Bind Port Failed!\n");
        exit(1);
    }
  //設置一個socket位址結構server_addr,代表伺服器的internet位址, 埠
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(inet_aton(argv[1],&server_addr.sin_addr) == 0) //伺服器的IP位址來自程式的參數
    {
        printf("Server IP Address Error!\n");
        exit(1);
    }
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
    socklen_t server_addr_length = sizeof(server_addr);
    //向伺服器發起連接,連接成功後client_socket代表了客戶機和伺服器的一個socket連接
    if(connect(client_socket,(struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        printf("Can Not Connect To %s!\n",argv[1]);
        exit(1);
    }
    char file_name[FILE_NAME_MAX_SIZE+1];
    bzero(file_name, FILE_NAME_MAX_SIZE+1);
    printf("Please Input File Name On Server:\t");
    scanf("%s", file_name);
   
    char buffer[BUFFER_SIZE];
    bzero(buffer,BUFFER_SIZE);
    strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE?BUFFER_SIZE:strlen(file_name));
    //向伺服器發送buffer中的資料
    send(client_socket,buffer,BUFFER_SIZE,0);
//    int fp = open(file_name, O_WRONLY|O_CREAT);
//    if( fp < 0 )
    FILE * fp = fopen(file_name,"w");
    if(NULL == fp )
    {
        printf("File:\t%s Can Not Open To Write\n", file_name);
        exit(1);
    }
 //從伺服器接收資料到buffer中
    bzero(buffer,BUFFER_SIZE);
    int length = 0;
    while( length = recv(client_socket,buffer,BUFFER_SIZE,0))
    {
        if(length < 0)
        {
            printf("Recieve Data From Server %s Failed!\n", argv[1]);
            break;
        }
//        int write_length = write(fp, buffer,length);
        int write_length = fwrite(buffer,sizeof(char),length,fp);
        if (write_length<length)
        {
            printf("File:\t%s Write Failed\n", file_name);
            break;
        }
        bzero(buffer,BUFFER_SIZE);   
    }
    printf("Recieve File:\t %s From Server[%s] Finished\n",file_name, argv[1]);
    fclose(fp);
    //關閉socket
    close(client_socket);
    return 0;
}
