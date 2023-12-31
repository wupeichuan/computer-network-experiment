#include "net.h"

int main() {
    int i;
    int sockfd[10];
    for(i=0;i<10;i++){
    /* 建立套接字 */
    sockfd[i] = Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    int enable = 1;
    Setsockopt(sockfd[i], SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    /* 绑定端口 */
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT+i);
    if (inet_pton(AF_INET, UDP_SERVER_ADDRESS, &server_addr.sin_addr.s_addr) != 1) {
        perror_and_exit("inet_pton");
    }
     
    Bind(sockfd[i], (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

    /* 接收数据 */
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    char buf[UDP_BUF_LENGTH]; 
   
    size_t pkt_len = Recvfrom(sockfd[i], buf, UDP_BUF_LENGTH, 0, (struct sockaddr *)&client_addr, &client_addr_len);

    buf[pkt_len] = '\0';
    printf("Message received: %s[%zu bytes]\n", buf, pkt_len);
    printf("IP address:%d:port:%d\n",client_addr.sin_addr.s_addr,client_addr.sin_port);
    Sendto(sockfd[i],buf,strlen(buf),0,(struct  sockaddr*)&client_addr,sizeof(client_addr));
    /* 关闭套接字 */
    close(sockfd[i]);
    }
    return 0;
}
