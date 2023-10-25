#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)




#define SERVER_PORT 12345
#define BUF_SIZE 1024

int main(){
    int server_sock_fd, client_sock_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];

    server_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_sock_fd == -1){
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Enable address reuse option
    int reuseaddr = 1;
    if(setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1){
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;


    if(bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", SERVER_PORT);

    while(1){
        socklen_t client_addr_len = sizeof(client_addr);

        int recv_num = recvfrom(server_sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (recv_num < 0) {
            printf("recv error\n");
			exit(EXIT_FAILURE);
        } else {
            buffer[recv_num] = '\0';
        }

        if (buffer[0] == 'i') {
            int n = atoi(buffer + 2);
            sprintf(buffer, "%d", n*n);
        } else if (buffer[0] == 'f') {
            float n = atof(buffer + 2);
            sprintf(buffer, "%f", n*n);
        } else {
            sprintf(buffer, "%s", "flase format");
        }

        int send_num = sendto(server_sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, client_addr_len);
		if(send_num < 0){
			perror("send error");
			exit(EXIT_FAILURE);
		}
        
    }

    close(server_sock_fd);

    return 0;
}
