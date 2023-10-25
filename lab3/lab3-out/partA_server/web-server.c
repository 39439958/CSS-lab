#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <sbuf.h>

#define TODO()\
do{\
    extern int printf(char *, ...);\
    printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__);\
}while(0)

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024
#define SBUFSIZE 16
#define NTHREADS 4

sbuf_t sbuf;
char web_root[256];

void serve_client(int client_sock){
    char request[BUF_SIZE];
    ssize_t bytes_received = read(client_sock, request, sizeof(request) - 1);
    if(bytes_received < 0){
        perror("read");
        return;
    }
    request[bytes_received] = '\0';

    // Parse the HTTP request, extract the requested file path, and handle the request.
    printf("Received request:\n%s\n", request);

    char method[16], path[128];
    int scanResult = sscanf(request, "%s %s", method, path);
    if(scanResult != 2){
        char response[] = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        write(client_sock, response, strlen(response));
        close(client_sock);
        return;  
    }

    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath), "%s%s", web_root, path);  

    // Handle GET requests for serving files.
    if(strcasecmp(method, "GET") == 0){

        // Exercise 5.
        // Add your code here:
        char body[BUF_SIZE], header[BUF_SIZE];
        FILE *file;

        if ((file = fopen(fullpath, "r")) == NULL) {
            perror("open file failed");
            
            return;
        }
        fread(body, 1, sizeof(body), file);
        fclose(file);

        snprintf(header, sizeof(header), "HTTP/1.1 200 OK\r\nContent-Type: text/html \r\nContent-Length: %ld\r\n\r\n", strlen(body));
        write(client_sock, header, strlen(header));
        write(client_sock, body, strlen(body));
    }
    // Handle DELETE requests for deleting files.
    
    else if(strcasecmp(method, "DELETE") == 0){
        // Exercise 6: Implement DELETE here
        // Add your code here:
        char response[BUF_SIZE];
        if(remove(fullpath) == 0) {
            sprintf(response, "HTTP/1.1 200 OK\r\n<html><body><h1>File deleted.</h1></body></html>");
        } else {
            sprintf(response, "HTTP/1.1 404 Not Found\r\n<html><body><h1>File Not Found.</h1></body></html>");
        }

    }else{
        char response[] = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
        write(client_sock, response, strlen(response));
    }

    close(client_sock);
}

void *thread(void *vargp) {
    pthread_detach(pthread_self());
    while (1) {
        int connfd = sbuf_remove(&sbuf);
        serve_client(connfd);
        close(connfd);
    }
}

int main(){
    strcpy(web_root, getenv("HOME"));
    strcat(web_root, "/mywebsite");

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t tid;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int reuseaddr = 1;
    if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1){
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(listen(server_sock, 5) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    sbuf_init(&sbuf, SBUFSIZE);
    for (int i = 0; i < NTHREADS; i++) {
        pthread_create(&tid, NULL, thread, NULL);
    }

    while(1){
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        } 
        
        serve_client(client_sock);

        // Challenge: multiple process
        // Add your code here:
        //TODO();
        //sbuf_insert(&sbuf, client_sock); 
    }

    close(server_sock);
    return 0;
}
