#include "request.h"

int main( void ) {
    int sockfd;
    char* domain_name = "dev.virtualearth.net";
    char* api = "/REST/v1/Locations?q=43302&output=xml&key=AjREHFypWifBoNqhz-uuZ6TZIezmGZlsgVLeVuL0TA1ShjFh72YmO-ruN0c2eOyN";
    char request[1024];
    char buffer[4096];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sprintf(request, "GET %s HTTP/1.1\r\n", api);
    strcat(request, "Host: dev.virtualearth.net\r\n");
    strcat(request, "Connection: close\r\n\r\n");

    getRequest(sockfd, domain_name, request, buffer);

    printf("%s", buffer);
}

void getRequest(int sockfd, const char* domain_name, const char* request, char* buffer) {
    struct hostent *host;
    struct sockaddr_in server_addr;
    int bytes_received = 1, count = 0;

    host = gethostbyname(domain_name); // Obsolete (but I don't care)
    if (host == NULL) {
        herror("gethostbyname");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr_list[0]);

    if (0 > connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        perror("connect");
        exit(1);
    }

    if (0 > send(sockfd, request, strlen(request), 0)) {
        perror("send");
        exit(1);
    }

    while(0 < bytes_received) {
        char line[100];

        bytes_received = recv(sockfd, line, sizeof(line) - 1, 0);
        strcat(buffer, line);
    }
    if (-1 == bytes_received) {
        perror("recv");
        exit(1);
    }

    close(sockfd);

    return;
}
