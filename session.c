#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "filetransfer.h"
#include "session.h"

/*Server*/
ServerSession* newServerSession(int clientfd){
    ServerSession* session;
    
    session = malloc(sizeof(ServerSession));
    bzero(session, sizeof(ServerSession));

    session->clientfd = clientfd;
    session->openfiles = newList();
    return session;
}

void freeServerSession(ServerSession* session){
    close(session->clientfd);    
    freeList(session->openfiles, free);
    free(session);
}

int handleOpenRequest(ServerSession* session, OpenRequest* req){
    int rc;
    OpenResponse res = {0};

    if(req == NULL){
        htonResponse((Response*)&res);
        rc = sendErrorResponse(session->clientfd, (Response*)&res);
        return -1;
    }
    res.req = req->type;

    rc = open(req->path, req->mode);
    if(rc < 0){
        htonResponse((Response*)&res);
        rc = sendErrorResponse(session->clientfd, (Response*)&res);
        return -1;
    }
  
    //responseの生成
    res.fd = rc;
    res.res = YES;

    htonOpenResponse(&res);
    rc = sendResponse(session->clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    return 0;
}

int handleCloseRequest(ServerSession* session, CloseRequest* req){
    int rc;
    CloseResponse res = {0};

    if(req == NULL){
        htonResponse((Response*)&res);
        rc = sendErrorResponse(session->clientfd, (Response*)&res);
        return -1;
    }
    res.req = req->type;

    rc = close(req->fd);
    if(rc < 0){
        htonResponse((Response*)&res);
        rc = sendErrorResponse(session->clientfd, (Response*)&res);
        return -1;
    }
  
    //responseの生成
    res.res = YES;

    htonCloseResponse(&res);
    rc = sendResponse(session->clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    return 0;
}

int handleReadRequest(ServerSession* session, ReadRequest* req);
int handleWriteRequest(ServerSession* session, WriteRequest* req);
int handleStatRequest(ServerSession* session, StatRequest* req);
int handleReaddirRequest(ServerSession* session, ReaddirRequest* req);

/*Client*/
ClientSession* newClientSession(char* ip, short port);
void freeClientSession(ClientSession* session);
int openRemoteFile(char* path, int mode);
int closeRemoteFile(char* path, int fd);
int readRemoteFile(char* path, int fd, char* buf, int offset, int size);
int writeRemoteFile(char* path, int fd, char* buf, int offset, int size);
int statRemoteFile(char* path, struct stat *stbuf);
int readdirRemoteFile(char* path, int fd, List* stats);

