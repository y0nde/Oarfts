#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "byteorder.h"
#include "structCommon.h"
#include "connection.h"
#include "filetransfer.h"

void printRemoteReadRequest(RemoteReadRequest* rrr){
    printf("[RemoteReadRequest]\n");
    printf("req_t: %d\n", rrr->type);
    printf("path: %s\n", rrr->path);
    printf("fd: %d\n", rrr->fd);
    printf("offset: %d\n", rrr->offset);
    printf("size: %d\n", rrr->size);
    printf("index: %d\n", rrr->index);
}

void ntohRRRequset(RemoteReadRequest* rrr){
    if(rrr == NULL){
        return;
    }
    rrr->type = ntoh4(rrr->type);
    rrr->fd = ntoh4(rrr->fd);
    rrr->offset = ntoh4(rrr->offset);
    rrr->size = ntoh4(rrr->size);
    rrr->index = ntoh4(rrr->index);
}

void htonRRRequset(RemoteReadRequest* rrr){
    if(rrr == NULL){
        return;
    }
    memcpy(rrr, rrr, sizeof(*rrr));
    rrr->type = hton4(rrr->type);
    rrr->fd = hton4(rrr->fd);
    rrr->offset = hton4(rrr->offset);
    rrr->size = hton4(rrr->size);
    rrr->index = hton4(rrr->index);
}


void printRemoteReadResponse(RemoteReadResponse* rrr){
    printf("[RemoteReadResponse]\n");
    printf("req_t: %d\n", rrr->req);
    printf("res_t: %d\n", rrr->res);
    printf("fd: %d\n", rrr->fd);
    printf("chunks: %d\n", rrr->chunks);
}

void ntohRemoteReadResponse(RemoteReadResponse *rrr){
    if(rrr == NULL){
        return;
    }
    memcpy(rrr, rrr, sizeof(*rrr));
    rrr->req = ntoh4(rrr->req);
    rrr->fd = ntoh4(rrr->fd);
    rrr->chunks = ntoh4(rrr->chunks);
}

void htonRemoteReadResponse(RemoteReadResponse *rrr){
    if(rrr == NULL){
        return;
    }
    memcpy(rrr, rrr, sizeof(*rrr));
    rrr->req = hton4(rrr->req);
    rrr->fd = hton4(rrr->fd);
    rrr->chunks = hton4(rrr->chunks);
}

int parseRequest(Request* req){
    switch(req->type){
        case READ:
            printRemoteReadRequest((RemoteReadRequest*)req);
        default:
            printf("NONE\n");
    }
    return 0;
}

/*main <client, server>*/
int testClient(){
    int rc, clientfd;

    clientfd = getClientSock("127.0.0.1", 8080);
    if(clientfd < 0){
        return -1;
    }

    /*セッション*/
    RemoteReadRequest req = {0};
    RemoteReadResponse res = {0};

    req.fd = 1;
    req.index = 0;
    req.size = 36;
    req.offset = 0;
    strcpy(req.path, "file");

    printRemoteReadRequest(&req);
    htonRRRequset(&req);
    rc = sendRequest(clientfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }

    rc = recvResponse(clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    ntohRemoteReadResponse(&res);
    printRemoteReadResponse(&res);

    return 0;
}

int testServer(){
    int rc;
    int listenfd, clientfd;

    listenfd = getServerSock(8080);
    if(listenfd < 0){
        return -1;
    }

    clientfd = acceptSock(listenfd);
    if(clientfd < 0){
        return -1;
    }

    /*セッション*/
    RemoteReadRequest req = {0};
    RemoteReadResponse res = {0};

    rc = recvRequest(clientfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }
    ntohRRRequset(&req);
    printRemoteReadRequest(&req);

    res.req = req.type;
    res.res = YES;
    res.fd = 0;
    res.chunks = 5;

    printRemoteReadResponse(&res);
    htonRemoteReadResponse(&res);
    rc = sendResponse(clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }

    return 0;
}

int main(int argc, char** argv){
    int rc;

    if(argc < 2){
        printf("filetransfer [\"client\" or \"server\"]\n");
        return 0;
    }

    if(strcmp(argv[1], "server") == 0){
        rc = testServer();
    }else if(strcmp(argv[1], "client") == 0){
        rc = testClient();
    }else{
        printf("filetransfer [\"client\" or \"server\"]\n");
    }
    return 0;
}
