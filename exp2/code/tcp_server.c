#include "net.h"
void handler(int sig){
    printf("recv a sig=%d\n",sig);
    exit(EXIT_SUCCESS);
}
int main() {

    /* 建立服务端套接字 */
    int server_sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    int enable = 1;
    Setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    /* 绑定端口 */
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TCP_SERVER_PORT);
    if (inet_pton(AF_INET, TCP_SERVER_ADDRESS, &server_addr.sin_addr.s_addr) != 1) {
        perror_and_exit("inet_pton");
    }

    Bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    /* 监听端口 */
    Listen(server_sockfd, CONNECTION_NUMBER);

    /* 建立tcp连接 */
    struct sockaddr_in client_addr;
    unsigned int client_addr_len = sizeof(struct sockaddr_in);
    int client_sockfd = Accept(server_sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    //printf("Accept client %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    pid_t pid;
    int SIGN;
    pid=fork();
    if(pid==-1) printf("fork error\n");
    else if(pid==0){
        //child
        signal(SIGN,handler);
        char sendbuf[TCP_BUF_LENGTH];
        while(1){
            if(fgets(sendbuf,TCP_BUF_LENGTH-1,stdin)==NULL){
                break;
            }
            Send(client_sockfd,sendbuf,strlen(sendbuf),0);
        }
        exit(EXIT_SUCCESS);
    }
    else{
        //parent
        char recvbuf[TCP_BUF_LENGTH];
        while(1){
        size_t pkt_len = Recv(client_sockfd, recvbuf, TCP_BUF_LENGTH, 0);
        if (pkt_len>0) {
            recvbuf[pkt_len] = '\0';
            fputs(recvbuf,stdout);
        }
        else break;
        }
    }
    kill(pid,SIGN);
    exit(EXIT_SUCCESS);
    /* 关闭套接字 */
    close(client_sockfd);
    close(server_sockfd);
    
    return 0;
}
