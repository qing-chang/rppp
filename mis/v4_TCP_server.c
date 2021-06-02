#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int main()
{
     int server_fd,client_fd;
     int client_len;
     struct sockaddr_in server_address,client_address;
     server_fd = socket(AF_INET,SOCK_STREAM, 0);
     server_address.sin_family = AF_INET;
     server_address.sin_port = htons(80);
     server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
     bind(server_fd,(struct sockaddr *)&server_address, sizeof(struct sockaddr_in));
     listen(server_fd,5);
     printf("waiting...\n");
     client_len=sizeof(struct sockaddr_in);
     client_fd = accept(server_fd, (struct sockaddr*)&client_address, (socklen_t *__restrict)&client_len);
     while(1)
     {
          char ch;
          read(client_fd,&ch,1);
          // ch++;
          write(client_fd,&ch,1);
     }
     close(client_fd);
     return 0;
}