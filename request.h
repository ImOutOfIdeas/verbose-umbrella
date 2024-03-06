#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 80

void getRequest(int sockfd, const char* domain_name, const char* request, char* buffer);
