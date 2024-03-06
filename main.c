#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define COORD_API_URL "dev.virtualearth.net"
#define COORD_API_KEY "AjREHFypWifBoNqhz-uuZ6TZIezmGZlsgVLeVuL0TA1ShjFh72YmO-ruN0c2eOyN"
#define PORT 80

void handle_error(const char *message) {
  perror(message);
  exit(1);
}

// Given a US zipcode, returns comma seperated long & lat (39,-84)
void getCoords(int sockfd, char* request, char* buffer, char* zipCode);

//void getWeather();

int main(int argc, char** argv) {
    int sockfd;
    char request[512];
    char buffer[4000];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        handle_error("socket");
    }

    getCoords(sockfd, request, buffer, argv[1]);

    return 0;
}

void getCoords(int sockfd, char* request, char* buffer, char* zipCode) {
    struct hostent *host;
    struct sockaddr_in server_addr;
    int bytes_sent;
    int bytes_received;
    char* start;
    char* end;
    char* output;

    host = gethostbyname(COORD_API_URL);
    if (host == NULL) {
        herror("gethostbyname");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr_list[0]);
 
    // Connect to server (dev.virtualearth.net);
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // Create HTTP request
    sprintf(request, "GET /REST/v1/Locations?q=%s&output=xml&key=%s HTTP/1.1\r\n", zipCode, COORD_API_KEY);
    strcat(request, "Host: dev.virtualearth.net\r\n");
    strcat(request, "Connection: close\r\n\r\n");

    // Send request
    bytes_sent = send(sockfd, request, strlen(request), 0);
    if (bytes_sent < 0) {
        perror("send");
        exit(1);
    }

    // Receive XML
    bytes_received = 0;
    int i = 0;
    while ((i = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        bytes_received += i;
    }
    if (i < 0) {
        perror("recv");
        exit(1);
    }

    close(sockfd);

    printf("RECEIVED: %d", bytes_received);

    buffer[bytes_received] = '\0';

    sleep(3);
    
    // Extract Latitude
    start = strstr(buffer, "<Latitude>") + strlen("<Latitude>");
    end = strstr(buffer, "</Latitude>");
    *end = '\0';
    printf("%s", start);
/*
    // Extract Longitude
    start = strstr(buffer, "<Longitude>") + strlen("<Longitude>");
    end = strstr(start, "</Longitude>");
    *end = '\0';
    sprintf(output + strlen(output), "%s", start); // Append longitude
*/
    return;
}


/*
   void getWeather() {
   struct sockaddr_in server_addr;
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(PORT);
   if (inet_pton(AF_INET, "api.weather.gov", &server_addr.sin_addr) <= 0) {
   handle_error("inet_pton");
   }  

   if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
   handle_error("connect");
   }

// HTTP GET request
sprintf(request, "GET /points/%f,%f/forecast HTTP/1.1\r\n", 40.5000, -83.4000);
strcat(request, "Host: api.weather.gov\r\n");
strcat(request, "User-Agent: ImOutOfIdeas/1.0 (jonathandoyle655321@gmail.com)\r\n");
strcat(request, "\r\n");

int bytes_sent = send(sockfd, request, strlen(request), 0);
if (bytes_sent < 0) {
handle_error("send");
}

int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
if (bytes_received < 0) {
handle_error("recv");
}

// TODO: Implement basic parsing
printf("Received %d bytes of data:\n", bytes_received);
printf("%s\n", buffer);

close(sockfd);
}*/
