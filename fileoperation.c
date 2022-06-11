#include "fileoperation.h"
#include "byteorder.h"
#include "list.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#define PATH_MAZ 256
#define CHUNK_SIZE 4048

int requestOpen(int fd, char* path, int mode){
    int fi, rc;
    struct Payload payload = {0};
    struct Payload* ppayload;

    payload.header.type = OPEN;
    payload.header.size = strlen(path) + 1;
    payload.header.slot1 = mode;
    payload.data = path;
  
    //リクエストを送信
    if((rc = sendPayload(fd, payload)) < 0){
        return -2;
    }
    //レスポンスの受信
    if((ppayload = recvPayload(fd)) == NULL){
        return -2;
    }
    if(ppayload->header.type != YES){
        return -1;
    }
    //ファイルディスクリプタはslot1に挿入される。
    fi = ppayload->header.slot1;
    freePayload(ppayload);
    return fi;
}

int responseOpen(int sockfd, struct Payload request){
    int fd;
    struct Payload response = {0};

    puts("responseOpen");

    fd = open(request.data, request.header.slot1);
    if(fd < 0){
        response.header.type = NO;
        if(sendPayload(sockfd, response) < 0 ){
            return -1;
        }
        return 0;
    }
    
    //responseの生成
    response.header.type = YES;
    response.header.slot1 = fd;

    //responseの送信
    if(sendPayload(sockfd, response) < 0){
        return -1;
    }

    return 0;
}

int requestClose(int sockfd, int fd){
    int fi, rc;
    struct Payload payload = {0};
    struct Payload* ppayload;

    payload.header.type = CLOSE;
    payload.header.size = 0;
    payload.header.slot1 = fd;
  
    //リクエストを送信
    if(sendPayload(sockfd, payload) < 0){
        puts("1");
        return -1;
    }
    //レスポンスの受信
    if((ppayload = recvPayload(sockfd)) == NULL){
        puts("2");
        return -1;
    }

    if(ppayload->header.type != YES){
        puts("3");
        return -1;
    }
    
    freePayload(ppayload);
    return 0;
}

int responseClose(int sockfd, struct Payload request){
    int rc;
    struct Payload response = {0};

    puts("responseClose");

    rc = close(request.header.slot1);
    if(rc < 0){
        response.header.type = NO;
        response.header.size = 0;
        if(sendPayload(sockfd, response) < 0 ){
            return -1;
        }
        return 0;
    }
    
    //responseの生成
    response.header.type = YES;

    //responseの送信
    if(sendPayload(sockfd, response) < 0){
        return -1;
    }

    return 0;
}

int requestRead(int sockfd, int fd, char* buf, int offset, int size){
    int fi, rc, rsize = 0;
    struct Payload payload = {0};
    struct Payload* ppayload;

    payload.header.type = READ;
    payload.header.slot1 = fd;
    payload.header.slot2 = offset;
    payload.header.slot3 = size;
  
    //リクエストを送信
    if((rc = sendPayload(sockfd, payload)) < 0){
        return -1;
    }
    //レスポンスの受信とファイルデータの転送
    while(1){
        ppayload = recvPayload(sockfd);
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

        //最後のペイロード
        if(ppayload->header.slot1 == -1){
            break;
        }
        freePayload(ppayload);
    }
    freePayload(ppayload);
    return rsize;
}

int responseRead(int fd, struct Payload request){
    int rc, filesize, size, sendsize, sizesum = 0, flag = 0;
    struct stat stbuf;
    struct Payload response = {0};
    char buf[CHUNK_SIZE] = {0};

    puts("responseRead");

    if(fstat(request.header.slot1, &stbuf) < 0){
        response.header.type = NO;
        if(sendPayload(fd, response) < 0){
            return -1;
        }
        return 0;
    }
    filesize = stbuf.st_size;

    if(lseek(request.header.slot1, request.header.slot2, SEEK_SET) < 0){
        response.header.type = NO;
        if(sendPayload(fd, response) < 0){
            return -1;
        }
        return 0;
    }

    //サイズの調整
    size = request.header.slot3;
    if(size + request.header.slot2 > filesize){
        size = filesize - request.header.slot2;
    }

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

        bzero(buf, CHUNK_SIZE);
        if((rc = read(request.header.slot1, buf, sendsize)) < 0){
            response.header.type = NO;
            if(sendPayload(fd, response) < 0){
                return -1;
            }
            return 0;
        }

        //payloadの作成
        response.data = buf;
        response.header.type = YES;
        response.header.size = sendsize;
        response.header.slot1 = flag;

        //payloadの送信
        if((rc = sendPayload(fd, response)) < 0){
            return -1;
        }

        size -= rc; 
        sizesum += rc;
    }
    return sizesum;
}

int requestWrite(int sockfd, int fd, char* buf, int offset, int size){
    int rc, sendsize, flag = 0, wsize = 0;
    struct Payload payload = {0};
    struct Payload* ppayload;

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

    //許可レスポンスの受信
    if((ppayload = recvPayload(sockfd)) == NULL){
        return -1;
    }
    if(ppayload->header.type != YES){
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
        payload.header.size = sendsize;

        //payloadの送信
        if((rc = sendPayload(sockfd, payload)) < 0){
            free(payload.data);
            return -1;
        }

        buf += sendsize;
        size -= sendsize;
        wsize +=  sendsize;
        free(payload.data);
    }
    return wsize;
}

int responseWrite(int fd, struct Payload request){
    int fi, rc, wsize = 0;
    struct Payload response = {0};
    struct Payload* recvdata;

    puts("responseWrite");

    //最初に書き込みが可能か返信する
    rc = write(request.header.slot1, NULL, 0);
    if(rc < 0){
        response.header.type = NO;
        response.header.size = 0;
        if((rc = sendPayload(fd, response)) < 0){
            return -1;
        }
        return 0;
    }else{
        response.header.type = YES;
        response.header.size = 0;
        if((rc = sendPayload(fd, response)) < 0){
            return -1;
        }
    }
    //writeができるならseekはエラーを起こさないだろう
    lseek(request.header.slot1, request.header.slot2, SEEK_SET);

    //レスポンスの受信とファイルデータの転送
    while(1){
        recvdata = recvPayload(fd);
        if(recvdata == NULL){
            return -1;
        }

        if(recvdata->header.size <= 0){
            break;
        }

        rc = write(request.header.slot1, recvdata->data, recvdata->header.size);

        wsize += rc;
        //最後のペイロード
        if(recvdata->header.slot1 == -1){
            break;
        }
        freePayload(recvdata);
    }
    freePayload(recvdata);
    return wsize;
}

void swapStat(struct stat* stbuf){
    if(stbuf == NULL){
        return;
    }
    if(isLittleEndien()){
        stbuf->st_size = bswap4(stbuf->st_size);
        stbuf->st_mode = bswap4(stbuf->st_mode);
        stbuf->st_gid = bswap4(stbuf->st_gid);
        stbuf->st_uid = bswap4(stbuf->st_uid);
        stbuf->st_blksize = bswap8(stbuf->st_blksize);
        stbuf->st_blocks = bswap8(stbuf->st_blocks);
        stbuf->st_ino = bswap8(stbuf->st_ino);
        stbuf->st_dev = bswap8(stbuf->st_dev);
        stbuf->st_rdev = bswap8(stbuf->st_rdev);
        stbuf->st_nlink = bswap8(stbuf->st_nlink);
        stbuf->st_mtime = bswap8(stbuf->st_mtime);
        stbuf->st_atime = bswap8(stbuf->st_atime);
        stbuf->st_ctime = bswap8(stbuf->st_ctime);
    }
}

int requestStat(int sockfd, char* path, struct stat* stbuf){
    struct Payload payload = {0};
    struct Payload* ppayload;

    payload.header.type = STAT;
    payload.header.size = strlen(path) + 1;
    payload.data = path;
  
    //リクエストを送信
    if(sendPayload(sockfd, payload) < 0){
        return -2;
    }
    //レスポンスの受信
    if((ppayload = recvPayload(sockfd)) == NULL){
        return -2;
    }
    if(ppayload->header.type != YES){
        return -1;
    }
    //statを取り出す
    *stbuf = *(struct stat*)ppayload->data;
    swapStat(stbuf);

    freePayload(ppayload);
    return 0;
}

int responseStat(int sockfd, struct Payload request){
    int rc;
    struct Payload response = {0};
    struct stat stbuf;

    puts("responseStat");

    rc = stat(request.data, &stbuf);
    if(rc < 0){
        response.header.type = NO;
        if(sendPayload(sockfd, response) < 0 ){
            return -1;
        }
        return 0;
    }
    
    //responseの生成
    response.header.type = YES;
    response.header.size = sizeof(struct stat);
    swapStat(&stbuf);
    response.data = (char*)&stbuf;

    //responseの送信
    if(sendPayload(sockfd, response) < 0){
        return -1;
    }

    return 0;
}

void printdirstat(void* _dstat){
    if(_dstat == NULL){
        return;
    }
    struct dirstat* dstat = _dstat;
    printf("%s %ld %ld %ld \n", dstat->path, dstat->st.st_size, dstat->st.st_mtime, dstat->st.st_ctime);
}

List* requestReaddir(int sockfd, char* path){
    int rc;
    List* stats;
    struct dirstat* attr;
    struct Payload payload = {0};
    struct Payload* ppayload;
    

    payload.header.type = READDIR;
    payload.header.size = strlen(path) + 1;
    payload.data = path;
  
    //リクエストを送信
    if(sendPayload(sockfd, payload) < 0){
        return NULL;
    }

    stats = newList();

    while(1){
        //レスポンスの受信
        if((ppayload = recvPayload(sockfd)) == NULL){
            freeList(stats, free);
            return NULL;
        }

        if(ppayload->header.type != YES){
            freeList(stats, free);
            return NULL;
        }

        if(ppayload->header.slot1 == -1){
            break;
        }

        //statを取り出す
        attr = malloc(sizeof(struct dirstat));
        *attr = *(struct dirstat*)ppayload->data;
        swapStat(&attr->st);

        push_front(stats, attr, sizeof(struct dirstat));
        freePayload(ppayload);
    }
    
    return stats;
}

int responseReaddir(int sockfd, struct Payload request){
    int rc;
    DIR* dir;
    struct dirent* entry;
    struct Payload response = {0};
    struct dirstat dstat = {0};

    puts("responseReaddir");

    dir = opendir(request.data);
    if(dir == NULL){
        response.header.type = NO;
        if(sendPayload(sockfd, response) < 0 ){
            return -1;
        }
        return 0;
    }
   
    while((entry = readdir(dir)) != NULL){
        //responseの生成
        response.header.type = YES;
        response.header.size = sizeof(struct dirstat);
        response.header.slot1 = 0;

        bzero(&dstat, sizeof(struct dirstat));
        strcpy(dstat.path, entry->d_name);
        stat(entry->d_name, &dstat.st);
        swapStat(&dstat.st);

        response.data = (char*)&dstat;
        //responseの送信
        if(sendPayload(sockfd, response) < 0){
            return -1;
        }
    }
    //responseの生成
    response.header.type = YES;
    response.header.size = 0;
    response.header.slot1 = -1;

    //responseの送信
    if(sendPayload(sockfd, response) < 0){
        return -1;
    }

    closedir(dir);

    return 0;
}
