#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "byteorder.h"
#include "structCommon.h"
#include "connection.h"
#include "filetransfer.h"

/*Read*/
void printReadRequest(ReadRequest* req){
    printf("[ReadRequest]\n");
    printf("req_t: %d\n", req->type);
    printf("path: %s\n", req->path);
    printf("fd: %d\n", req->fd);
    printf("offset: %d\n", req->offset);
    printf("size: %d\n", req->size);
}

void ntohReadRequest(ReadRequest* req){
    if(req == NULL){
        return;
    }
    req->type = ntoh4(req->type);
    req->fd = ntoh4(req->fd);
    req->offset = ntoh4(req->offset);
    req->size = ntoh4(req->size);
}

void htonReadRequest(ReadRequest* req){
    if(req == NULL){
        return;
    }
    memcpy(req, req, sizeof(*req));
    req->type = hton4(req->type);
    req->fd = hton4(req->fd);
    req->offset = hton4(req->offset);
    req->size = hton4(req->size);
}


void printReadResponse(ReadResponse* res){
    printf("[ReadResponse]\n");
    printf("req_t: %d\n", res->req);
    printf("res_t: %d\n", res->res);
    printf("fd: %d\n", res->fd);
    printf("chunks: %d\n", res->chunks);
}

void ntohReadResponse(ReadResponse *res){
    if(res == NULL){
        return;
    }
    res->req = ntoh4(res->req);
    res->fd = ntoh4(res->fd);
    res->chunks = ntoh4(res->chunks);
}

void htonReadResponse(ReadResponse *res){
    if(res == NULL){
        return;
    }
    res->req = hton4(res->req);
    res->fd = hton4(res->fd);
    res->chunks = hton4(res->chunks);
}

int parseRequest(Request* req){
    switch(req->type){
        case READ:
            printReadRequest((ReadRequest*)req);
        default:
            printf("NONE\n");
    }
    return 0;
}

/*Write*/
void printWriteRequest(WriteRequest* req){
    printf("[ReadRequest]\n");
    printf("req_t: %d\n", req->type);
    printf("path: %s\n", req->path);
    printf("fd: %d\n", req->fd);
    printf("offset: %d\n", req->offset);
    printf("size: %d\n", req->size);
}

void ntohWriteRequest(WriteRequest* req){
    if(req == NULL){
        return;
    }
    req->type = ntoh4(req->type);
    req->fd = ntoh4(req->fd);
    req->offset = ntoh4(req->offset);
    req->size = ntoh4(req->size);
}

void htonWriteRequest(WriteRequest* req){
    if(req == NULL){
        return;
    }
    memcpy(req, req, sizeof(*req));
    req->type = hton4(req->type);
    req->fd = hton4(req->fd);
    req->offset = hton4(req->offset);
    req->size = hton4(req->size);
}


void printWriteResponse(WriteResponse* res){
    printf("[OpenResponse]\n");
    printf("req_t: %d\n", res->req);
    printf("res_t: %d\n", res->res);
    printf("fd: %d\n", res->fd);
    printf("chunks: %d\n", res->chunks);
}

void ntohWriteResponse(WriteResponse *res){
    if(res == NULL){
        return;
    }
    res->req = ntoh4(res->req);
    res->fd = ntoh4(res->fd);
    res->chunks = ntoh4(res->chunks);
}

void htonWriteResponse(WriteResponse *res){
    if(res == NULL){
        return;
    }
    res->req = hton4(res->req);
    res->fd = hton4(res->fd);
    res->chunks = hton4(res->chunks);
}

/*Open*/
void printOpenRequest(OpenRequest* req){
    printf("[OpenRequest]\n");
    printf("req_t: %d\n", req->type);
    printf("path: %s\n", req->path);
    printf("mode: %d\n", req->mode);
}

void ntohOpenRequest(OpenRequest* req){
    if(req == NULL){
        return;
    }
    req->type = ntoh4(req->type);
    req->mode = ntoh4(req->mode);
}

void htonOpenRequest(OpenRequest* req){
    if(req == NULL){
        return;
    }
    memcpy(req, req, sizeof(*req));
    req->type = hton4(req->type);
    req->mode = hton4(req->mode);
}


void printOpenResponse(OpenResponse* res){
    printf("[OpenResponse]\n");
    printf("req_t: %d\n", res->req);
    printf("res_t: %d\n", res->res);
    printf("fd: %d\n", res->fd);
}

void ntohOpenResponse(OpenResponse *res){
    if(res == NULL){
        return;
    }
    res->req = ntoh4(res->req);
    res->res = ntoh4(res->res);
    res->fd = ntoh4(res->fd);
}

void htonOpenResponse(OpenResponse *res){
    if(res == NULL){
        return;
    }
    res->req = hton4(res->req);
    res->res = hton4(res->res);
    res->fd = hton4(res->fd);
}

/*Close*/
void printCloseRequest(CloseRequest* req){
    printf("[CloseRequest]\n");
    printf("req_t: %d\n", req->type);
    printf("path: %s\n", req->path);
    printf("fd: %d\n", req->fd);
}

void ntohCloseRequest(CloseRequest* req){
    if(req == NULL){
        return;
    }
    req->type = ntoh4(req->type);
    req->fd = ntoh4(req->fd);
}

void htonCloseRequest(CloseRequest* req){
    if(req == NULL){
        return;
    }
    memcpy(req, req, sizeof(*req));
    req->type = hton4(req->type);
    req->fd = hton4(req->fd);
}


void printCloseResponse(CloseResponse* res){
    printf("[ReadResponse]\n");
    printf("res_t: %d\n", res->req);
    printf("res_t: %d\n", res->res);
}

void ntohCloseResponse(CloseResponse *res){
    if(res == NULL){
        return;
    }
    res->req = ntoh4(res->req);
}

void htonCloseResponse(CloseResponse *res){
    if(res == NULL){
        return;
    }
    res->req = hton4(res->req);
}

/*Stat*/
void printStatRequest(StatRequest* req){
    printf("[StatRequest]\n");
    printf("req_t: %d\n", req->type);
    printf("path: %s\n", req->path);
}

void ntohStatRequest(StatRequest* req){
    if(req == NULL){
        return;
    }
    req->type = ntoh4(req->type);
}

void htonStatRequest(StatRequest* req){
    if(req == NULL){
        return;
    }
    memcpy(req, req, sizeof(*req));
    req->type = hton4(req->type);
}


void printStatResponse(StatResponse* res){
    printf("[StatResponse]\n");
    printf("res_t: %d\n", res->req);
    printf("res_t: %d\n", res->res);
}

void ntohStatResponse(StatResponse *res){
    if(res == NULL){
        return;
    }
    res->req = ntoh4(res->req);
    res->res = ntoh4(res->res);
}

void htonStatResponse(StatResponse *res){
    if(res == NULL){
        return;
    }
    res->req = hton4(res->req);
    res->fd = hton4(res->fd);
    res->chunks = hton4(res->chunks);
}

/*Readdir*/
void printReadRequest(ReadRequest* req){
    printf("[ReadRequest]\n");
    printf("req_t: %d\n", req->type);
    printf("path: %s\n", req->path);
    printf("fd: %d\n", req->fd);
    printf("offset: %d\n", req->offset);
    printf("size: %d\n", req->size);
}

void ntohReadRequest(ReadRequest* req){
    if(req == NULL){
        return;
    }
    req->type = ntoh4(req->type);
    req->fd = ntoh4(req->fd);
    req->offset = ntoh4(req->offset);
    req->size = ntoh4(req->size);
}

void htonReadRequest(ReadRequest* req){
    if(req == NULL){
        return;
    }
    memcpy(req, req, sizeof(*req));
    req->type = hton4(req->type);
    req->fd = hton4(req->fd);
    req->offset = hton4(req->offset);
    req->size = hton4(req->size);
}


void printReadResponse(ReadResponse* res){
    printf("[ReadResponse]\n");
    printf("res_t: %d\n", res->req);
    printf("res_t: %d\n", res->res);
    printf("fd: %d\n", res->fd);
    printf("chunks: %d\n", res->chunks);
}

void ntohReadResponse(ReadResponse *res){
    if(res == NULL){
        return;
    }
    res->req = ntoh4(res->req);
    res->fd = ntoh4(res->fd);
    res->chunks = ntoh4(res->chunks);
}

void htonReadResponse(ReadResponse *res){
    if(res == NULL){
        return;
    }
    res->req = hton4(res->req);
    res->fd = hton4(res->fd);
    res->chunks = hton4(res->chunks);
}

int parseRequest(Request* req){
    switch(req->type){
        case READ:
            printReadRequest((ReadRequest*)req);
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
    ReadRequest req = {0};
    ReadResponse res = {0};

    req.fd = 1;
    req.index = 0;
    req.size = 36;
    req.offset = 0;
    strcpy(req.path, "file");

    printReadRequest(&req);
    htonRRRequest(&req);
    rc = sendRequest(clientfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }

    rc = recvResponse(clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    ntohReadResponse(&res);
    printReadResponse(&res);

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
    ReadRequest req = {0};
    ReadResponse res = {0};

    rc = recvRequest(clientfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }
    ntohReadRequest(&req);
    printReadRequest(&req);

    res.req = req.type;
    res.res = YES;
    res.fd = 0;
    res.chunks = 5;

    printReadResponse(&res);
    htonReadResponse(&res);
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
