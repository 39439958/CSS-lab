#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define HOST_PORT 12345
#define BUF_SIZE 1024

void main (int argc, char argv){
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    int yes = 0;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    struct sockaddr_in host_addr;
    memset(&host_addr, 0, sizeof(host_addr));
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(HOST_PORT);
    host_addr.sin_addr.s_addr = 0;
    // #1：bind
    bind(sock_fd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr));
    // #2: listen
    listen(sock_fd, 10);

    while(1){
        struct sockaddr_in client_addr;
        memset(&client_addr, 0, sizeof(client_addr));
        unsigned int client_addr_len = sizeof(client_addr);
        // #3: accept
        int client_sock_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        char buf[1024] = "hello, world\n";
        while (1) {
            printf("sent a message to the client:\n");
            write(client_sock_fd, buf, strlen(buf));
            read(client_sock_fd, buf, 1024);
            *(strchr(buf, '\n') + 1) = '\0';
        }    
        close(client_sock_fd);
    }
}