#include "request.h"

#define COORDINATES_DOMAIN "dev.virtualearth.net"
#define COORDINATES_API_KEY "AjREHFypWifBoNqhz-uuZ6TZIezmGZlsgVLeVuL0TA1ShjFh72YmO-ruN0c2eOyN"
#define PORT 80

// return 0 and truncate ZIP to 5 digits if valid and -1 otherwise 
int valid_zip(char* zip) {
    char* end;

    if (5 < strlen(zip)) zip[5] = '\0'; // Truncate input to 5 characters

    strtol(zip, &end, 10);

    if (5 != (end - zip)) {
        return -1;
    } 

    return 0;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <ZIP code>\n", argv[0]);
        exit(1);
    }
    if (-1 == valid_zip(argv[1])) {
        printf("Invalid ZIP code. Must be five digit number\n");
        exit(1);
    }

    int sockfd;
    char request[1024];
    char buffer[4096];
    char endpoint[256];
    char coords[128];
    char* domain_name = COORDINATES_DOMAIN;

    sprintf(endpoint, "/REST/v1/Locations?q=%s&output=xml&key=%s", argv[1], COORDINATES_API_KEY);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    formatRequest(request, domain_name, endpoint);
    getRequest(sockfd, domain_name, request, buffer);

    sprintf(coords, "%s%%2C", parseTag(buffer, "<Latitude>"));
    strcat(coords, parseTag(buffer, "<Longitude>")); 

    printf("%s", coords); 

    return 0;
}

