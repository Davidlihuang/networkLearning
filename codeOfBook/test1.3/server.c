#include <unp.h>
#include <time.h>

int main(int argc, char **argv)
{
    int sockfd, connfd;
    struct sockaddr_in servaddr;
    char buffer[MAXLINE];
    time_t ticks;

    //创建套接字
    sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //绑定端口
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(16555);
    printf("binding...");
    Bind(sockfd, (SA*)&servaddr, sizeof(servaddr));

    //listen:套接字转换为监听套接字
    Listen(sockfd, LISTENQ);
    
    //接受连接，发送应答
    printf("listening...");
    while(1)
    {
        connfd = Accept(sockfd, (SA*)NULL, NULL);
        ticks = time(NULL);
        snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buffer, strlen(buffer));

        Close(connfd);
    }
    return 0;
}