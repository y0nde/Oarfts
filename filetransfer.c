#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "byteorder.h"
#include "list.h"
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
    printf("size: %d\n", res->size);
}

void ntohReadResponse(ReadResponse *res){
    if(res == NULL){
        return;
    }
    res->req = ntoh4(res->req);
    res->size = ntoh4(res->size);
}

void htonReadResponse(ReadResponse *res){
    if(res == NULL){
        return;
    }
    res->req = hton4(res->req);
    res->size = hton4(res->size);
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
    printf("size: %d\n", res->size);
}

void ntohWriteResponse(WriteResponse *res){
    if(res == NULL){
        return;
    }
    res->req = ntoh4(res->req);
    res->size = ntoh4(res->size);
}

void htonWriteResponse(WriteResponse *res){
    if(res == NULL){
        return;
    }
    res->req = hton4(res->req);
    res->size = hton4(res->size);
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
    printf("[CloseResponse]\n");
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
    res->res = hton4(res->res);
}

/*Readdir*/
void printReaddirRequest(ReaddirRequest* req){
    printf("[ReaddirRequest]\n");
    printf("req_t: %d\n", req->type);
    printf("path: %s\n", req->path);
}

void ntohReaddirRequest(ReaddirRequest* req){
    if(req == NULL){
        return;
    }
    req->type = ntoh4(req->type);
}

void htonReaddirRequest(ReaddirRequest* req){
    if(req == NULL){
        return;
    }
    memcpy(req, req, sizeof(*req));
    req->type = hton4(req->type);
}

void printReaddirResponse(ReaddirResponse* res){
    printf("[ReaddirResponse]\n");
    printf("res_t: %d\n", res->req);
    printf("res_t: %d\n", res->res);
}

void ntohReaddirResponse(ReaddirResponse *res){
    if(res == NULL){
        return;
    }
    res->req = ntoh4(res->req);
}

void htonReaddirResponse(ReaddirResponse *res){
    if(res == NULL){
        return;
    }
    res->req = hton4(res->req);
}

int parsePrintRequest(Request* req){
    req_t type;

    type = ntoh4(req->type);
    switch(type){
        case OPEN:
            ntohOpenRequest((OpenRequest*)req);
            printOpenRequest((OpenRequest*)req);
            break;
        case CLOSE:
            ntohCloseRequest((CloseRequest*)req);
            printCloseRequest((CloseRequest*)req);
            break;
        case READ:
            ntohReadRequest((ReadRequest*)req);
            printReadRequest((ReadRequest*)req);
            break;
        case WRITE:
            ntohWriteRequest((WriteRequest*)req);
            printWriteRequest((WriteRequest*)req);
            break;
        case STAT:
            ntohStatRequest((StatRequest*)req);
            printStatRequest((StatRequest*)req);
            break;
        case READDIR:
            ntohReaddirRequest((ReaddirRequest*)req);
            printReaddirRequest((ReaddirRequest*)req);
            break;
        default:
            printf("NONE\n");
    }
    return 0;
}

/*AttrTransfer*/
int sendDir(int sockfd, List* attrs){
    int rc;
    int index = 0;
    FileAttr attr = {0};
    FileAttr *pattr;
    Node* node;

    if(attrs->head == NULL){//からである。
        attr.errno = 3;
        attr.index = -1;
        rc = sendFileAttr(sockfd, &attr); 
        return -1;
    }
    for(node = attrs->head; node != NULL; node = node->next){
        pattr = node->data;
        pattr->errno = 0;
        if(node->next == NULL){
            pattr->index = -1;
        }else{
            pattr->index = index;
        }
        rc = sendFileAttr(sockfd, pattr); 
        if(rc < 0){
            return -1;
        }
        index++;
    }
    return 0;
}

int recvDir(int sockfd, List* attrs){
    int rc;
    int index = 0;
    FileAttr* attr;

    //attrsがNULLの場合、対応できない
    while(1){
        attr = malloc(sizeof(FileAttr));
        rc = recvFileAttr(sockfd, attr);
        if(rc < 0){
            return -1;
        }

        //エラーチェック
        if(attr->errno > 0){
            return -1;
        }
        push_front(attrs, attr, sizeof(FileAttr));

        //indexが-1の時に終了
        if(attr->index == -1){
            break;
        }
    }

    return 0;
}

/*FileTransfer*/
int sendFileData(int sockfd, char *buf, int size){
    int rc, chunks; 
    int cnt = 0;
    FileChunk chunk = {0};

    if(buf == NULL){
        chunk.index = -2;
        rc = sendFileChunk(sockfd, &chunk);
        if(rc < 0){
            printf("send FileChunk fail\n");
            return -1;
        }
        return 0;
    }

    chunks = (size / CHUNK_SIZE) + 1;
    for(int i = 0; i < chunks; i++){
        //送る分を計算
        if(size <= CHUNK_SIZE){//最後の送信チャンク
            chunk.index = -1;//最後の送信チャンクはindexに-1を入れて送信先に知らせる。
            chunk.size = size;
        }else{
            chunk.index = i;
            chunk.size = CHUNK_SIZE;
        }
        memcpy(chunk.data, buf + (i * CHUNK_SIZE), chunk.size);
        size -= chunk.size;
        rc = sendFileChunk(sockfd, &chunk);
        if(rc < 0){
            printf("send FileChunk fail\n");
            return -1;
        }
    } 
    return 0;
}

int recvFileData(int sockfd, char *buf, int size){
    int rc, chunks; 
    int cnt = 0;
    FileChunk chunk = {0};

    chunks = (size / CHUNK_SIZE) + 1;
    for(int i = 0; i < chunks; i++){
        //送る分を計算
        rc = recvFileChunk(sockfd, &chunk);
        if(rc < 0){
            printf("recv FileChunk fail\n");
            return -1;
        }

        //エラーチェック
        if(chunk.index == -2){
            printf("remote operation fail\n");
            return -1;
        }

        cnt += chunk.size;
        memcpy(buf + (i * CHUNK_SIZE), chunk.data, chunk.size);
        //最後のパケットか確認、であれば終了
        if(chunk.index == -1){
            break;
        }
    } 
    return 0;
}

int parsePrintResponse(Response* res){
    req_t type;

    type = ntoh4(res->req);
    switch(type){
        case OPEN:
            ntohOpenResponse((OpenResponse*)res);
            printOpenResponse((OpenResponse*)res);
            break;
        case CLOSE:
            ntohCloseResponse((CloseResponse*)res);
            printCloseResponse((CloseResponse*)res);
            break;
        case READ:
            ntohReadResponse((ReadResponse*)res);
            printReadResponse((ReadResponse*)res);
            break;
        case WRITE:
            ntohWriteResponse((WriteResponse*)res);
            printWriteResponse((WriteResponse*)res);
            break;
        case STAT:
            ntohStatResponse((StatResponse*)res);
            printStatResponse((StatResponse*)res);
            break;
        case READDIR:
            ntohReaddirResponse((ReaddirResponse*)res);
            printReaddirResponse((ReaddirResponse*)res);
            break;
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

    //Read
    ReadRequest req = {0};
    ReadResponse res = {0};

    req.type = READ;
    req.fd = 1;
    req.size = 36;
    req.offset = 0;
    strcpy(req.path, "file");

    htonReadRequest(&req);
    rc = sendRequest(clientfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&req);

    rc = recvResponse(clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&res);

    //Write
    WriteRequest wreq = {0};
    WriteResponse wres = {0};

    wreq.type = WRITE;
    wreq.fd = 1;
    wreq.size = 36;
    wreq.offset = 0;
    strcpy(wreq.path, "file");

    htonWriteRequest(&wreq);
    rc = sendRequest(clientfd, (Request*)&wreq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&wreq);

    rc = recvResponse(clientfd, (Response*)&wres);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&wres);

    //Open
    OpenRequest oreq = {0};
    OpenResponse ores = {0};

    oreq.type = OPEN;
    oreq.mode = 1;
    strcpy(oreq.path, "file");

    htonOpenRequest(&oreq);
    rc = sendRequest(clientfd, (Request*)&oreq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&oreq);

    rc = recvResponse(clientfd, (Response*)&ores);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&ores);

    //Close
    CloseRequest creq = {0};
    CloseResponse cres = {0};

    creq.type = CLOSE;
    creq.fd = 1;
    strcpy(creq.path, "file");

    htonCloseRequest(&creq);
    rc = sendRequest(clientfd, (Request*)&creq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&creq);

    rc = recvResponse(clientfd, (Response*)&cres);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&cres);

    //Stat
    StatRequest sreq = {0};
    StatResponse sres = {0};
    
    sreq.type = STAT;
    strcpy(sreq.path, "file");

    htonStatRequest(&sreq);
    rc = sendRequest(clientfd, (Request*)&sreq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&sreq);

    rc = recvResponse(clientfd, (Response*)&sres);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&sres);

    //Readdir
    ReaddirRequest rrreq = {0};
    ReaddirResponse rrres = {0};

    rrreq.type = READDIR;
    strcpy(rrreq.path, "file");

    htonReaddirRequest(&rrreq);
    rc = sendRequest(clientfd, (Request*)&rrreq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&rrreq);

    rc = recvResponse(clientfd, (Response*)&rrres);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&rrres);

    //StatTransfer
    FileAttr fileattr = {0};
    rc = stat("Makefile", &fileattr.st);
    if(rc < 0){
        return -1;
    }

    fileattr.errno = 0;
    strcpy(fileattr.path, "Makefile");
    rc = sendFileAttr(clientfd, &fileattr);
    if(rc < 0){
        return -1;
    }
    printFileAttr(&fileattr);
    
    //FileTransfer
    FILE* file;
    char ftbuf[5120] = {0};

    file = fopen("./Makefile", "r");
    rc = fread(ftbuf, 1, 5120, file);
    rc = sendFileData(clientfd, ftbuf, rc);

    //DirTransfer
    DIR* dir;
    struct dirent* entry;
    struct stat stbuf;
    List* attrs;

    attrs = newList();

    rc = recvDir(clientfd, attrs);
    if(rc < 0){
        return -1;
    }

    printList(attrs, printFileAttr);

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

    //READ
    ReadRequest req = {0};
    ReadResponse res = {0};

    rc = recvRequest(clientfd, (Request*)&req);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&req);

    res.req = req.type;
    res.res = YES;
    res.size = 5;

    htonReadResponse(&res);
    rc = sendResponse(clientfd, (Response*)&res);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&res);

    //WRITE
    WriteRequest wreq = {0};
    WriteResponse wres = {0};

    rc = recvRequest(clientfd, (Request*)&wreq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&wreq);

    wres.req = wreq.type;
    wres.res = YES;
    wres.size = 5;

    htonWriteResponse(&wres);
    rc = sendResponse(clientfd, (Response*)&wres);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&wres);

    //OPEN
    OpenRequest oreq = {0};
    OpenResponse ores = {0};

    rc = recvRequest(clientfd, (Request*)&oreq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&oreq);

    ores.req = oreq.type;
    ores.res = YES;
    ores.fd = 4;

    htonOpenResponse(&ores);
    rc = sendResponse(clientfd, (Response*)&ores);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&ores);

    //CLOSE
    CloseRequest creq = {0};
    CloseResponse cres = {0};

    rc = recvRequest(clientfd, (Request*)&creq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&creq);

    cres.req = creq.type;
    cres.res = YES;

    htonCloseResponse(&cres);
    rc = sendResponse(clientfd, (Response*)&cres);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&cres);

    //STAT
    StatRequest sreq = {0};
    StatResponse sres = {0};

    rc = recvRequest(clientfd, (Request*)&sreq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&sreq);

    sres.req = sreq.type;
    sres.res = YES;

    htonStatResponse(&sres);
    rc = sendResponse(clientfd, (Response*)&sres);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&sres);

    //READDIR
    ReaddirRequest rrreq = {0};
    ReaddirResponse rrres = {0};

    rc = recvRequest(clientfd, (Request*)&rrreq);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintRequest((Request*)&rrreq);

    rrres.req = rrreq.type;
    rrres.res = YES;

    htonReaddirResponse(&rrres);
    rc = sendResponse(clientfd, (Response*)&rrres);
    if(rc < 0){
        return -1;
    }
    rc = parsePrintResponse((Response*)&rrres);

    //StatTransfer
    FileAttr fileattr;

    rc = recvFileAttr(clientfd, &fileattr);
    if(rc < 0){
        return -1;
    }
    if(fileattr.errno == 0){
        printFileAttr(&fileattr);
    }

    //FileTransfer
    FILE* file;
    char ftbuf[5120] = {0};

    rc = recvFileData(clientfd, ftbuf, 5120);
    printf("[FileData]\n%s", ftbuf);

    //DirTransfer
    DIR* dir;
    struct dirent* entry;
    struct stat stbuf;
    List* attrs;

    attrs = newList();

    dir = opendir(".");
    if(dir == NULL){
        return -1;
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
            return -1;
        }

        push_front(attrs, &fileattr, sizeof(FileAttr));
    }

    closedir(dir);

    rc = sendDir(clientfd, attrs);
    if(rc < 0){
        return -1;
    }

    return 0;
}

int test_main(int argc, char** argv){
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
