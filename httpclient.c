#include "connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTTP_PORT 80

char* httpGetRequest(char* path){
    int size;
    const char* get = "GET ";
    const char* protocol = " HTTP/1.0";
    char* request;

    size = strlen(path) + 4 + 10;
    request = malloc(size);
    bzero(request, size);

    strcpy(request, get);
    strcat(request, path);
    strcat(request, protocol);

    return request;
}

int main(){
    char* path = "google.com";

    printf("%s\n", httpGetRequest(path));
    return 0;
}
