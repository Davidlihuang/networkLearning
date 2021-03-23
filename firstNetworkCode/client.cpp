#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE 2048
#define SERVER_IP "192.168.0.111" //指定服务器的ip
#define SERVER_PORT 16555   //指定服务段的端口

int main()
{
    struct sockaddr_in servaddr;
    char buff[BUFFSIZE];
    int sockfd;

    //创建socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sockfd == -1) 
    {
        printf("Create socket error(%d):%s\n", errno, strerror(errno));
        return -1;
    }

    //连接
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);
    servaddr.sin_port = htons(SERVER_PORT); 
    if( -1 == connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        printf("Connect error(%d):%s\n", errno, strerror(errno));
        return -1;
    } 

    //发送数据
    printf("Please input: ");
    scanf("%s", buff);
    send(sockfd, buff, strlen(buff), 0);

    //接受数据
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, BUFFSIZE-1, 0);
    printf("Recv: %s\n", buff);

    //关闭客户端
    close(sockfd);
    return 0;
}

