#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *method = "GET";
    char *url = "/";
    char *protocol = "HTTP/1.1";
    char request[1024];

    // Construct the request
    snprintf(request, sizeof(request), "%s %s %s\r\nHost: localhost\r\n\r\n", method, url, protocol);
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    send(sock , request , strlen(request) , 0 );
    printf("GET message sent\n");
    valread = read( sock , request, 1024);
    printf("%s\n",request );
    
    return 0;
}

