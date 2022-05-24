#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "list.h"
#include "filetransfer.h"
#include "session.h"
#include "connection.h"
#include "byteorder.h"

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
    res.size = size;

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

//アロケート済みリストを返却
List* getAttrList(char* path){
    int rc;
    DIR* dir;
    struct dirent* entry;
    struct stat stbuf;
    List* attrs;
    FileAttr fileattr = {0};

    attrs = newList();

    dir = opendir(".");
    if(dir == NULL){
        return NULL;
    }

    while(1){
        bzero(&fileattr, sizeof(FileAttr));
        entry = readdir(dir);
        if(entry == NULL){
            break;
        }
        strcpy(fileattr.path, entry->d_name);
        
        rc = stat(entry->d_name, &fileattr.st);
        if(rc < 0){
            return NULL;
        }

        push_front(attrs, &fileattr, sizeof(FileAttr));
    }

    closedir(dir);
    return attrs;
}

int handleReaddirRequest(ServerSession* session, ReaddirRequest* req){
    int rc, fileNum;
    ReaddirResponse res = {0};
    FileAttr attr;
    List* attrs;

    if(req == NULL){
        return -1;
    }
    res.req = req->type;

    //存在を確認
    rc = stat(req->path, &attr.st);
    if(rc < 0){
        htonReaddirResponse(&res);
        sendErrorResponse(session->clientfd, (Response*)&res);
        return -1;
    }
    //ディレクトリか
    if(!S_ISDIR(attr.st.st_mode)){
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
    attrs = getAttrList(req->path);
    if(attrs == NULL){
        htonReaddirResponse(&res);
        sendErrorResponse(session->clientfd, (Response*)&res);
        return -1;
    }
    rc = sendDir(session->clientfd, attrs);
    if(rc < 0){
        return -1;
    }
    //Free Attrs
    freeList(attrs, free);
    return 0;
}

int parsePrintResponse(ServerSession *session, Request* req){
    req_t type;

    type = ntoh4(req->type);
    switch(type){
        case OPEN:
            handleOpenRequest(session, (OpenRequest*)req);
            break;
        case CLOSE:
            handleCloseRequest(session, (CloseRequest*)req);
            break;
        case READ:
            handleReadRequest(session, (ReadRequest*)req);
            break;
        case WRITE:
            handleWriteRequest(session, (WriteRequest*)req);
            break;
        case STAT:
            handleStatRequest(session, (StatRequest*)req);
            break;
        case READDIR:
            handleReaddirRequest(session, (ReaddirRequest*)req);
            break;
        default:
            printf("NONE\n");
    }
    return 0;
}

/*Client*/
ClientSession* newClientSession(char* ip, short port){
    ClientSession* session;

    session = malloc(sizeof(ClientSession));
    bzero(session, sizeof(ClientSession));
    strcpy(session->ip, ip);
    session->port = port;

    //connect
    session->serverfd = getClientSock(ip, port);
    if(session->serverfd < 0){
        return NULL;
    }
    return session;
}

void freeClientSession(ClientSession* session){
    close(session->serverfd);    
    free(session);
}

int openRemoteFile(ClientSession* session, char* path, int mode){
    int rc;

    OpenRequest req = {0};
    OpenResponse res = {0};

    req.type = OPEN;
    req.mode = mode;
    strcpy(req.path, path);

    htonOpenRequest(&req);
    rc = sendRequest(session->serverfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }

    rc = recvResponse(session->serverfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    ntohOpenResponse(&res);

    //レスポンスの審査
    if(res.res != YES){
        return -1;
    }

    //ファイルディスクリプタを返す。
    return res.fd;
}

int closeRemoteFile(ClientSession* session, char* path, int fd){
    int rc;

    CloseRequest req = {0};
    CloseResponse res = {0};

    req.type = CLOSE;
    req.fd = fd;
    strcpy(req.path, path);

    htonCloseRequest(&req);
    rc = sendRequest(session->serverfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }

    rc = recvResponse(session->serverfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    ntohCloseResponse(&res);

    //レスポンスの審査
    if(res.res != YES){
        return -1;
    }

    return 0;
}

int readRemoteFile(ClientSession* session, char* path, int fd, char* buf, int offset, int size){
    int rc;

    ReadRequest req = {0};
    ReadResponse res = {0};

    req.type = READ;
    req.fd = fd;
    req.offset = offset;
    req.size = size;;
    strcpy(req.path, path);

    htonReadRequest(&req);
    rc = sendRequest(session->serverfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }

    rc = recvResponse(session->serverfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    ntohReadResponse(&res);

    //レスポンスの審査
    if(res.res != YES){
        return -1;
    }
    size = res.size; //読み取りサイズを修正
    
    rc = recvFileData(session->serverfd, buf, size); 
    if(rc < 0){
        return -1;
    }

    return rc; 
}

int writeRemoteFile(ClientSession* session, char* path, int fd, char* buf, int offset, int size){
    int rc;

    WriteRequest req = {0};
    WriteResponse res = {0};

    req.type = WRITE;
    req.fd = fd;
    req.offset = offset;
    req.size = size;
    strcpy(req.path, path);

    htonWriteRequest(&req);
    rc = sendRequest(session->serverfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }

    rc = recvResponse(session->serverfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    ntohWriteResponse(&res);

    //レスポンスの審査
    if(res.res != YES){
        return -1;
    }
    
    rc = sendFileData(session->serverfd, buf, size); 
    if(rc < 0){
        return -1;
    }

    return rc; 
}

int statRemoteFile(ClientSession* session, char* path, struct stat *stbuf){
    int rc;

    FileAttr fileattr = {0};
    StatRequest req = {0};
    StatResponse res = {0};

    req.type = STAT;
    strcpy(req.path, path);

    htonStatRequest(&req);
    rc = sendRequest(session->serverfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }

    rc = recvResponse(session->serverfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    ntohStatResponse(&res);

    //レスポンスの審査
    if(res.res != YES){
        return -1;
    }
    
    rc = recvFileAttr(session->serverfd, &fileattr); 
    if(rc < 0){
        return -1;
    }

    *stbuf = fileattr.st;
    return 0; 

}

int readdirRemoteFile(ClientSession* session, char* path, int fd, List* stats){
    int rc;

    FileAttr fileattr = {0};
    ReaddirRequest req = {0};
    ReaddirResponse res = {0};

    req.type = READDIR;
    strcpy(req.path, path);

    htonReaddirRequest(&req);
    rc = sendRequest(session->serverfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }

    rc = recvResponse(session->serverfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    ntohReaddirResponse(&res);

    //レスポンスの審査
    if(res.res != YES){
        return -1;
    }
    
    rc = recvDir(session->serverfd, stats);
    if(rc < 0){
        return -1;
    }

    return 0; 
}

