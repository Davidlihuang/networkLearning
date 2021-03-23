#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <signal.h>

#define BUFFSIZE 2048
#define DEFAULT_PORT 16555
#define MAXLINK 2048 
#define LOG_BUFFSIZE 65536

//日志类
class Logger
{
private:
    char buff[LOG_BUFFSIZE];
    int buffLen;
    FILE* fp;
public:
    Logger(){
        bzero(buff, sizeof(buff));
        buffLen = 0;
        fp = fopen("TrainServer.log", "a");
    }
    void Flush()
    {
        fputs(buff, fp);
        bzero(buff, sizeof(buff));
        buffLen = 0;
    }
    void Log(const char* str, int len)
    {
        if(buffLen + len > LOG_BUFFSIZE -10)
        {
            Flush();
        }
        for(int i=0; i< len; i++)
        {
            buff[buffLen] = str[i];
            buffLen++;
        }
    }
    ~Logger()
    {
        if(buffLen != 0) Flush();
        fclose(fp);
    }
}logger;


int sockfd, connfd;
void stopServerRuning(int p)
{
    close(sockfd);
    printf("Close Server...\n");
    exit(0);
}

void setResponse(char*buff)
{
    bzero(buff, sizeof(buff));
    strcat(buff, "HTTP/1.1 200 OK\r\n");
    strcat(buff, "Connection: close\r\n");
    strcat(buff, "\r\n");
    strcat(buff, "Hello network!\n");
}

int main()
{
    struct sockaddr_in servaddr; //用于存放ip和端口的结构
    char buff[BUFFSIZE];

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( -1 == sockfd) { printf("Create socket error(%d): %s\n", errno, strerror(errno)); return -1;}

    // bind address
    bzero(&servaddr, sizeof(servaddr)); //清空servaddr
    servaddr.sin_family = AF_INET; //ipv4协议族
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons( DEFAULT_PORT);
    if(-1 == bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)))
    {
        printf("Bind error(%d):%s\n", errno, strerror(errno));
        return -1;
    }

    //Listening ...
    if( -1 == listen(sockfd, MAXLINK))
    {
        printf("Listen error(%d):%s\n", errno, strerror(errno));
        return -1;
    }
    printf("Listening...\n");

    while(true)
    {
        signal(SIGINT, stopServerRuning); //在输入ctrl+c时关闭服务器
    
    //connected
        connfd =  accept(sockfd, NULL, NULL);
        if( -1 == connfd)
        {
            printf("Accept error(%d):%s", errno, strerror(errno));
            return -1;
        }      
    
    // recive data
    bzero(buff, BUFFSIZE);
    recv(connfd, buff, BUFFSIZE-1, 0);
    //printf("Recv: %s\n", buff);
    logger.Log("Recv:", 6);
    logger.Log(buff, strlen(buff)); //性能优化，一段时间后将log写入文件

    setResponse(buff);   // 2. http正确的返回串头和正文了
    //send data
    send(connfd, buff, strlen(buff), 0);

    close(connfd);
    }
    return 0;
}