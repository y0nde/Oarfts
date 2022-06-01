#include "connection.h"
#include "byteorder.h"
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

struct PayloadHeader {
    int type;
    int size;
    int slot1;
    int slot2;
    int slot3;
    int slot4;
};

struct Payload {
    struct PayloadHeader header;
    char* data;
};

struct PayloadHeader alignPayloadHeader(struct PayloadHeader header){

    if(isLittleEndien()){
        header.type = bswap4(header.type);
        header.size = bswap4(header.size);
        header.slot1 = bswap4(header.slot1);
        header.slot2 = bswap4(header.slot2);
        header.slot3 = bswap4(header.slot3);
        header.slot4 = bswap4(header.slot4);
    } 
    return header;
}

void freePayload(struct Payload* payload){
    if(payload != NULL){
        if(payload->data != NULL){
            free(payload->data);
        }
        free(payload);
    }
}

int sendPayload(int fd, struct Payload payload){
    int rc;

    payload.header = alignPayloadHeader(payload.header);
    if(sendData(fd, &payload.header, sizeof(struct PayloadHeader)) < 0){
        return -1;
    }

    //送るサイズが０なら送らない
    if(payload.header.size > 0){
        if((rc = sendData(fd, payload.data, payload.header.size)) < 0){
            return -1;
        }
    }
    return rc;
}

struct Payload* recvPayload(int fd){
    int rc;
    struct PayloadHeader* header;
    struct Payload* payload = NULL;

    if((header = recvData(fd)) == NULL){
        return NULL;
    }
    payload->header = alignPayloadHeader(payload->header);

    //受け取るサイズが０以上なら受け取る
    if(header->size > 0){
        payload = calloc(0, sizeof(struct Payload));
        payload->header = *header;
        payload->data = recvData(fd);
        if(payload->data == NULL){
            free(payload);
            return NULL;
        }
    }
    return payload;
}


int test_main(int argc, char* argv[]){
    if(argc < 2){
        exit(EXIT_FAILURE);
    }

    struct Payload req = {0};

    if(strcmp(argv[1], "client") == 0){
        int fd;
        struct Payload *payload;

        fd = getClientSock("127.0.0.1", 8080);

        //パケットが来ることがわかっている
        if((payload = recvPayload(fd)) == NULL){
            return -1;
        };
        printf("%d,%d,%s\n", payload->header.size, payload->header.type, payload->data);
    }else if(strcmp(argv[1], "server") == 0){
        int lfd, fd, rc;

        lfd = getServerSock(8080);
        fd = acceptSock(lfd);
   
        req.data = "This Payload is sent.";
        req.header.size = strlen(req.data) + 1;
        if(sendPayload(fd, req) < 0){
            return -1;
        }
    }else{
        printf("invalid args\n");
    }
    return 0;
}

int main(int argc, char** argv){return test_main(argc, argv);}
