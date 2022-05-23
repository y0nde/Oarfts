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

int handleReadRequest(ServerSession* session, ReadRequest* req){
    int rc, size;
    char* buf;
    ReadResponse res={0};
    struct stat stbuf;

    if(req == NULL){
        return -1;
    }
    res.req = req->type;
    res.res = YES;
   
    //サイズの計算
    rc = stat(req->path, &stbuf); 
    if(rc < 0){
        htonReadResponse(&res);
        sendErrorResponse(session->clientfd, (Response*)&res);
        return -1;
    }
    if((req->size + req->offset) < stbuf.st_size){
        size = req->size;
    }else{
        size = stbuf.st_size - req->offset;
    }

    htonReadResponse(&res);
    rc = sendResponse(session->clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }

    buf = malloc(size);
    rc =lseek(req->fd, req->offset, SEEK_SET);
    if(rc < 0){
        return -1;
    }
    rc = read(req->fd, buf, size);
    if(rc < 0){
        return -1;
    }
    
    rc = sendFileData(session->clientfd, buf, size);
    if(rc < 0){
        return -1;
    }
    return 0;
}

int handleWriteRequest(ServerSession* session, WriteRequest* req){
    int rc, size;
    char* buf;
    WriteResponse res={0};

    if(req == NULL){
        return -1;
    }
    
    res.req = req->type;
    res.res = YES; 
    res.size = req->size;

    htonWriteResponse(&res);
    rc = sendResponse(session->clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }

    buf = malloc(req->size);
    rc = recvFileData(session->clientfd, buf, req->size);
    if(rc < 0){
        return -1;
    }

    //書き込み
    rc =lseek(req->fd, req->offset, SEEK_SET);
    if(rc < 0){
        return -1;
    }
    rc = write(req->fd, buf, req->size);
    if(rc < 0){
        return -1;
    }
    return 0;
}

int handleStatRequest(ServerSession* session, StatRequest* req){
    int rc;
    StatResponse res = {0};
    FileAttr attr;

    if(req == NULL){
        return -1;
    }
    res.req = req->type;

    rc = stat(req->path, &attr.st);
    if(rc < 0){
        htonStatResponse(&res);
        sendErrorResponse(session->clientfd, (Response*)&res);
        return -1;
    }

    res.res = YES;
    rc = sendResponse(session->clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }

    attr.errno = 0;
    attr.index = -1;
    rc = sendFileAttr(session->clientfd, &attr);
    if(rc < 0){
        return -1;
    }
    return 0;
}

int handleReaddirRequest(ServerSession* session, ReaddirRequest* req){
    int rc;
    ReaddirResponse res = {0};
    FileAttr attr;

    if(req == NULL){
        return -1;
    }
    res.req = req->type;

    rc = stat(req->path, &attr.st);
    if(rc < 0){
        htonReaddirResponse(&res);
        sendErrorResponse(session->clientfd, (Response*)&res);
        return -1;
    }

    res.res = YES;
    rc = sendResponse(session->clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }

    //FileAttr stream
    return 0;
}

/*Client*/
ClientSession* newClientSession(char* ip, short port);
void freeClientSession(ClientSession* session);
int openRemoteFile(char* path, int mode);
int closeRemoteFile(char* path, int fd);
int readRemoteFile(char* path, int fd, char* buf, int offset, int size);
int writeRemoteFile(char* path, int fd, char* buf, int offset, int size);
int statRemoteFile(char* path, struct stat *stbuf);
int readdirRemoteFile(char* path, int fd, List* stats);

