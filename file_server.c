#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for printf
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero

#define HELLO_WORLD_SERVER_PORT    6666
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512

int main(int argc, char **argv)
{
    //設置一個socket位址結構server_addr,代表伺服器internet位址, 埠
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr)); //把一段記憶體區的內容全部設置為0
    server_addr.sin_family = AF_INET; //位址資料族系，同樣設定為AF_INET
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
//主機IP位址 in_addr資料格式
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
//主機開啟的通訊埠號 用htons() 寫入
    //創建用於internet的流協議(TCP)socket,用server_socket代表伺服器socket
    int server_socket = socket(PF_INET,SOCK_STREAM,0);
    if( server_socket < 0)
    {
        printf("Create Socket Failed!");
        exit(1);
    }
     //把socket和socket位址結構聯繫起來
    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
    {
        printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT);
        exit(1);
    }
    //server_socket用於監聽
    if ( listen(server_socket, LENGTH_OF_LISTEN_QUEUE) )
    {
        printf("Server Listen Failed!");
        exit(1);
    }
    while (1) //伺服器端要一直運行
    {
        //定義用戶端的socket位址結構client_addr
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        //接受一個到server_socket代表的socket的一個連接
        //如果沒有連接請求,就等待到有連接請求--這是accept函數的特性
        //accept函數返回一個新的socket,這個socket(new_server_socket)用於同連接到的客戶的通信
        //new_server_socket代表了伺服器和用戶端之間的一個通信通道
        //accept函數把連接到的用戶端資訊填寫到用戶端的socket位址結構client_addr中
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
            printf("Server Accept Failed!\n");
            break;
        }
        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);
        length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
        if (length < 0)
        {
            printf("Server Recieve Data Failed!\n");
            break;
        }
        char file_name[FILE_NAME_MAX_SIZE+1];
        bzero(file_name, FILE_NAME_MAX_SIZE+1);
        strncpy(file_name, buffer, strlen(buffer)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buffer));
//        int fp = open(file_name, O_RDONLY);
//        if( fp < 0 )
        FILE * fp = fopen(file_name,"r");
        if(NULL == fp )
        {
            printf("File:\t%s Not Found\n", file_name);
        }
        else
        {
            bzero(buffer, BUFFER_SIZE);
            int file_block_length = 0;
//            while( (file_block_length = read(fp,buffer,BUFFER_SIZE))>0)
            while( (file_block_length = fread(buffer,sizeof(char),BUFFER_SIZE,fp))>0)
            {
                printf("file_block_length = %d\n",file_block_length);
                //發送buffer中的字串到new_server_socket,實際是給用戶端
                if(send(new_server_socket,buffer,file_block_length,0)<0)
                {
                    printf("Send File:\t%s Failed\n", file_name);
                    break;
                }
                bzero(buffer, BUFFER_SIZE);
            }
//            close(fp);
            fclose(fp);
            printf("File:\t%s Transfer Finished\n",file_name);
        }
        //關閉與用戶端的連接
        close(new_server_socket);
    }
    //關閉監聽用的socket
    close(server_socket);
    return 0;
}
