#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *const *argv)
{    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr; 
    memset(&serv_addr,0,sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3490);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("调用connect()失败，退出！\n");
        exit(1);
    }
    // char ch[] = {"asdfg"};
    // write(sockfd,&ch,5);
    int m,n = 0;
    char recvline[1000 + 1]; 
    while((m = read(sockfd,recvline,1000)) > 0)
    {
        n+=m;
    }
    recvline[n] = 0;
    printf("收到的内容为：%s\n",recvline);
    close(sockfd);
    printf("程序执行完毕，退出!\n");
    return 0;
}

