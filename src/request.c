#include "request.h"

void formatRequest(char* request, const char* domain_name, const char* endpoint) {
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n", endpoint, domain_name);
    strcat(request, "Connection: close\r\n\r\n");
}

void getRequest(int sockfd, const char* domain_name, const char* request, char* buffer) {
    struct hostent *host;
    struct sockaddr_in server_addr;
    int bytes_received = 1;

    host = gethostbyname(domain_name); // Obsolete (but I don't care)
    if (host == NULL) {
        perror("gethostbyname");
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
}

char* parseTag(char* buffer, char* tag) {
    char* start;
    char* end;

    start = strstr(buffer, tag);
    if (NULL == start) {
        printf("The tag, %s, was not found", tag);
        return "\0";
    } 

    start += strlen(tag);
    end = strstr(start, "<");

    *end = '\0';

    return start;
}


