#include "connection.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>

struct Header {
    int size;
};

struct Header swapHeader(struct Header _header){
    struct Header header = {0};

    header.size = ntohl(_header.size);
    return header;
}

int sendData(int fd, void* buf, int size){
    int rc;
    int errno;
    struct Header header;

    header.size = size;

    //send header
    header = swapHeader(header);
    if((rc = write(fd, &header, sizeof(struct Header))) < 0){
        printf("error: %s\n", strerror(errno));
        return -1;
    };

    //send data
    if((rc = write(fd, buf, size)) < 0){
        printf("error: %s\n", strerror(errno));
        return -1;
    };
    
    return rc; 
}

void* recvData(int fd){
    int rc;
    int errno;
    struct Header header = {0};
    void* buf;

    //recv header
    if((rc = read(fd, &header, sizeof(struct Header))) < 0){
        printf("error: %s\n", strerror(errno));
        return NULL;
    };
    header = swapHeader(header);

    //recv data
    if(header.size < 0){
        return NULL;
    }
    buf = calloc(0, header.size);
    if((rc = read(fd, buf, header.size)) < 0){
        printf("error: %s\n", strerror(errno));
        return NULL;
    };

    return buf;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        exit(EXIT_FAILURE);
    }

    struct Packet{
        int a;
        int b;
        char c[16];
    } packetA;


    if(strcmp(argv[1], "client") == 0){
        int fd;
        struct Packet *pack;

        fd = getClientSock("127.0.0.1", 8080);

        //パケットが来ることがわかっている
        if((pack = recvData(fd)) == NULL){
            return -1;
        };

        printf("%d,%d,%s\n", pack->a, pack->b, pack->c);
    }else if(strcmp(argv[1], "server") == 0){
        int lfd, fd, rc;

        lfd = getServerSock(8080);
        fd = acceptSock(lfd);
    
        packetA.a = 7;
        packetA.b = 8;
        bzero(packetA.c, 16);strcpy(packetA.c, "This is server.");

        if(sendData(fd, &packetA, sizeof(struct Packet)) < 0){
            return -1;
        }
        printf("%d,%d,%s\n", packetA.a, packetA.b, packetA.c);
    }else{
        printf("invalid args\n");
    }
    return 0;
}
