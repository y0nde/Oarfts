#include "transfer.h"
#include "list.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define PATH_MAZ 256
#define CHUNK_SIZE 4048

typedef enum {
    NONE,
    OPEN,
    CLOSE,
    READ,
    WRITE,
    STAT,
    READDIR,
} req_t;

typedef enum {
    DENY,
    YES,
    NO,
    DISCONNECT,
    BUSY,
    WAIT
} res_t;

typedef struct FileAttr{
    int errno;
    int index;
    struct stat st;
    char path[PATH_MAZ];
} FileAttr;

int requestOpen(int fd, char* path, int mode){
    int fi, rc;
    struct Payload payload;
    struct Payload* ppayload;

    payload.header.type = OPEN;
    payload.header.size = strlen(path) + 1;
    payload.header.slot1 = mode;
    payload.data = strdup(path);
  
    //リクエストを送信
    if((rc = sendPayload(fd, payload)) < 0){
        return -1;
    }
    //レスポンスの受信
    if((ppayload = recvPayload(fd)) == NULL){
        return -1;
    }
    if(ppayload->header.type != YES){
        return -1;
    }
    //ファイルディスクリプタはslot1に挿入される。
    fi = ppayload->header.slot1;
    freePayload(ppayload);
    free(payload.data);
    return ppayload->header.slot1;
}

int responseRead(int fd, struct Payload request){
    int rc;
    struct Payload response;

    rc = open(request.data, request.header.slot1);
    if(rc < 0){
        response.header.type = NO;
        if((rc = sendPayload(fd, response)) < 0 ){
            return -1;
        }
        return 0;
    }
}

int requestClose(int fd, char* path, int mode){
    int fi, rc;
    struct Payload payload;
    struct Payload* ppayload;

    payload.header.type = CLOSE;
    payload.header.size = strlen(path) + 1;
    payload.data = strdup(path);
  
    //リクエストを送信
    if((rc = sendPayload(fd, payload)) < 0){
        return -1;
    }
    //レスポンスの受信
    if((ppayload = recvPayload(fd)) == NULL){
        return -1;
    }
    if(ppayload->header.type != YES){
        return -1;
    }
    //ファイルディスクリプタはslot1に挿入される。
    freePayload(ppayload);
    free(payload.data);
    return 0;
}

int requestRead(int sockfd, int fd, char* buf, int offset, int size){
    int fi, rc, rsize = 0;
    struct Payload payload;
    struct Payload* ppayload;

    payload.header.type = WRITE;
    payload.header.slot1 = fd;
    payload.header.slot2 = offset;
    payload.header.slot3 = size;
  
    //リクエストを送信
    if((rc = sendPayload(fd, payload)) < 0){
        return -1;
    }
    //レスポンスの受信とファイルデータの転送
    while(1){
        ppayload = recvPayload(fd);
        if(ppayload == NULL){
            return -1;
        }
        if(ppayload->header.type != YES){
            break;
        }
        if(ppayload->header.size <= 0){
            break;
        }
        memcpy(buf, ppayload->data, ppayload->header.size);
        buf += ppayload->header.size;
        rsize +=  ppayload->header.size;
        freePayload(ppayload);
    }
    freePayload(ppayload);
    return rsize;
}

int responseRead(int fd, struct Payload request){
    int rc;


}

int requestWrite(int sockfd, int fd, char* buf, int offset, int size){
    int rc, sendsize, flag = 0, wsize = 0;
    struct Payload payload;

    //バッファーチェック
    if(buf == NULL){
        return -1;
    }

    payload.header.type = WRITE;
    payload.header.slot1 = fd;
    payload.header.slot2 = offset;
    payload.header.slot3 = size;
 
    //リクエストを送信
    if((rc = sendPayload(sockfd, payload)) < 0){
        return -1;
    }
    //レスポンスの受信とファイルデータの転送
    while(1){
        //送るサイズの計算
        if(size > CHUNK_SIZE){
            sendsize = CHUNK_SIZE;
        }else if(size > 0){
            sendsize = size;
            flag = -1;
        }else{
            //送るサイズが０
            break;
        }

        //payloadの作成
        payload.data = malloc(sendsize);
        memcpy(payload.data, buf, sendsize);
        payload.header.slot1 = flag;
        payload.header.slot2 = sendsize;

        //payloadの送信
        if((rc = sendPayload(sockfd, payload)) < 0){
            free(payload.data);
            return -1;
        }

        buf += sendsize;
        wsize +=  sendsize;
        free(payload.data);
    }
    return wsize;
}

