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
    servaddr.sin_port     = htons(13); // 
    return 0;
}
