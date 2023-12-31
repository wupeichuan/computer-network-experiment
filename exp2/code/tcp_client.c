#include "net.h"
void handler(int sig){
    printf("recv a sig=%d\n",sig);
    exit(EXIT_SUCCESS);
}
int main() {
    int i = 0;
    /* 建立套接字 */
    int sockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* 建立tcp连接 */
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TCP_SERVER_PORT);
    if (inet_pton(AF_INET, TCP_SERVER_ADDRESS, &server_addr.sin_addr.s_addr) != 1) {
        perror_and_exit("inet_pton");
    }
    Connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    pid_t pid;
    int SIGN;
    pid=fork();
    if(pid==-1) printf("fork error\n");
    else if(pid==0){
    //child
    signal(SIGN,handler);
    char msg[TCP_BUF_LENGTH];
    while(1){
    if(fgets(msg,TCP_BUF_LENGTH-1,stdin)==NULL){
        break;
    }
    Send(sockfd, msg, strlen(msg), 0);
    }
    exit(EXIT_SUCCESS);
    }
    else{
    char buf[TCP_BUF_LENGTH];
    while(1){
    size_t pkt_len = Recv(sockfd, buf, TCP_BUF_LENGTH, 0);
    if (pkt_len > 0) {
        buf[pkt_len] = '\0'; 
        fputs(buf,stdout);
    } 
    else break;
    }
    }
    kill(pid,SIGN);
    exit(EXIT_SUCCESS);
    /* 关闭套接字 */
    close(sockfd);
    return 0;
}
