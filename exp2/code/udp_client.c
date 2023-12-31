#include "net.h"

int main() {
    int sockfd[10];
    int i;
    char buf[10];
    char msg [7] ={"hello"};
    /* 建立套接字 */
    

    /* 发送数据 */
  
    for(i=0;i<10;i++){
    sockfd[i] = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT+i);
    if (inet_pton(AF_INET,UDP_SERVER_ADDRESS, &server_addr.sin_addr.s_addr) != 1) {
        perror_and_exit("inet_pton");
    }
    Connect(sockfd[i], (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    msg[5]=i+'0';
    msg[6]='\0';
    printf("Send message: %s[%zu bytes]\n", msg, strlen(msg));

    
    Send(sockfd[i], msg, strlen(msg), 0);
    Recv(sockfd[i], buf, strlen(msg), 0);
    /* 关闭套接字 */
    close(sockfd[i]);
    }
    return 0;
}
