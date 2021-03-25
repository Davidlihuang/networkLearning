#include <unp.h>

int main(int argc, char **argv)
{
    int sockfd, n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr; // 存放网络协议和地址的数据结构

    if(argc != 2) err_quit("usage: a.out <IPaddress>");
    
    //创建socket套接字句柄
    sockfd = socket(AF_INET, SOCK_STREAM,  IPPROTO_TCP); 
    if(sockfd < 0) err_sys("Socket error");

    //配置IP地址
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port     = htons(16555); 
    int ipflag = inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    if(ipflag <= 0) err_quit("inet_pton error for %s", argv[1]);

    //连接服务器
    if( -1 == connect(sockfd, (SA* )&servaddr, sizeof(servaddr)))  err_sys("connect error");

    //接收数据
    while ((n = read(sockfd, recvline, MAXLINE))> 0)
    {
        recvline[n] = 0;
        if( fputs(recvline, stdout) == EOF) err_sys("fputs error");
    }
    if( n < 0) err_sys("read error");
    
    return 0;
}
