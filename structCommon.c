/*構造体転送のためのルール*/
/*ポインタのない構造体の作成*/
/*デバッグのために構造体をプリントする関数を用意*/
/*構造体のバイトオーダーを双方向に変換できる関数を用意*/

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "byteorder.h"
#include "structCommon.h"

/*********/
/*Request*/
/*********/
void printRequest(Request* req){
    printf("[Request]\ntype: %d\n", req->type);
    printf("path: %s\n", req->path);
    printf("data: %s\n", req->data);
}

void htonRequest(Request* dst, Request* frm){
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    dst->type = hton4(frm->type);
}

void ntohRequest(Request* dst, Request* frm){
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    dst->type = ntoh4(frm->type);
}

int sendRequest(int sockfd, Request* preq){
    int rc;
    Request req;

    if(preq == NULL){
        return -1;
    }

    htonRequest(&req, preq);
    rc = send(sockfd, &req, sizeof(req), 0);
    if(rc < 0){
        printf("sendRequest fail\n");
        return -1;
    }
    return rc;
}

int recvRequest(int sockfd, Request* preq){
    int rc;
    Request req;

    if(preq == NULL){
        return -1;
    }

    rc = recv(sockfd, &req, sizeof(req), 0);
    if(rc < 0){
        printf("recvRequest fail\n");
        return -1;
    }
    ntohRequest(preq, &req);
    return rc;
}

/**********/
/*Response*/
/**********/
void printResponse(Response* res){
    printf("[Response]\ntype: %d\n", res->type);
}


void htonResponse(Response* dst, Response* frm){
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    dst->type = hton4(frm->type);
}

void ntohResponse(Response* dst, Response* frm){
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    dst->type = ntoh4(frm->type);
}

int sendResponse(int sockfd, Response* pres){
    int rc;
    Response res;

    if(pres == NULL){
        return -1;
    }

    htonResponse(&res, pres);
    rc = send(sockfd, &res, sizeof(res), 0);
    if(rc < 0){
        printf("send Response fail\n");
        return -1;
    }
    return rc;
}

int recvResponse(int sockfd, Response* pres){
    int rc;
    Response res;

    if(pres == NULL){
        return -1;
    }

    rc = recv(sockfd, &res, sizeof(res), 0);
    if(rc < 0){
        printf("send Response fail\n");
        return -1;
    }

    htonResponse(pres, &res);
    return rc;
}

/******/
/*stat*/
/******/
void printstat(struct stat* st){
    printf("stat->st_size: %ld\n", st->st_size);
    printf("stat->st_mode: %d\n", st->st_mode);
    printf("stat->st_ctime: %ld\n", st->st_ctime);
    printf("stat->st_mtime: %ld\n", st->st_mtime);
    printf("stat->st_nlink: %ld\n", st->st_nlink);
}


void ntohtimespec(struct timespec* dst, struct timespec* frm){
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    dst->tv_nsec = ntoh8(frm->tv_nsec);
    dst->tv_sec = ntoh8(frm->tv_sec);
}

void htontimespec(struct timespec* dst, struct timespec* frm){
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    dst->tv_nsec = hton8(frm->tv_nsec);
    dst->tv_sec = hton8(frm->tv_sec);
}

void ntohstat(struct stat* dst, struct stat* frm){
    int rc;
    
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    ntohtimespec(&dst->st_atim, &frm->st_atim);
    ntohtimespec(&dst->st_ctim, &frm->st_ctim);
    ntohtimespec(&dst->st_mtim, &frm->st_mtim);
    dst->st_blksize = ntoh8(frm->st_blksize);
    dst->st_blocks = ntoh8(frm->st_blocks);
    dst->st_uid = ntoh4(frm->st_uid);
    dst->st_gid = ntoh4(frm->st_gid);
    dst->st_mode = ntoh4(frm->st_mode);
    dst->st_dev = ntoh8(frm->st_dev);
    dst->st_ino = ntoh8(frm->st_ino);
    dst->st_size = ntoh8(frm->st_size);
    dst->st_nlink = ntoh8(frm->st_nlink);
    dst->st_rdev = ntoh8(frm->st_rdev);
}

void htonstat(struct stat* dst, struct stat* frm){
    int rc;
    
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    htontimespec(&dst->st_atim, &frm->st_atim);
    htontimespec(&dst->st_ctim, &frm->st_ctim);
    htontimespec(&dst->st_mtim, &frm->st_mtim);
    dst->st_blksize = hton8(frm->st_blksize);
    dst->st_blocks = hton8(frm->st_blocks);
    dst->st_uid = hton4(frm->st_uid);
    dst->st_gid = hton4(frm->st_gid);
    dst->st_mode = hton4(frm->st_mode);
    dst->st_dev = hton8(frm->st_dev);
    dst->st_ino = hton8(frm->st_ino);
    dst->st_size = hton8(frm->st_size);
    dst->st_nlink = hton8(frm->st_nlink);
    dst->st_rdev = hton8(frm->st_rdev);
}

int sendstat(int sockfd, struct stat* pst){
    int rc;
    struct stat st;

    if(pst == NULL){
        return -1;
    }

    htonstat(&st, pst);
    rc = send(sockfd, &st, sizeof(st), 0);
    if(rc < 0){
        printf("send stat fail\n");
        return -1;
    }
    return rc;
}

int recvstat(int sockfd, struct stat* pst){
    int rc;
    struct stat st;

    if(pst == NULL){
        return -1;
    }

    rc = recv(sockfd, &st, sizeof(st), 0);
    if(rc < 0){
        printf("recv stat fail\n");
        return -1;
    }
    ntohstat(pst, &st);
    return rc;
}

/**********/
/*FileData*/
/**********/
void printFileData(FileData* data){
    data->data[1023] = '\0';
    printf("[FileData]\nindex: %d\n%s\n", data->index, data->data);
}

void htonFileData(FileData* data){
    if(data == NULL){
        return;
    }
    data->index = hton4(data->index);
}

void ntohFileData(FileData* data){
    if(data == NULL){
        return;
    }
    data->index = ntoh4(data->index);
}

int sendFileData(int sockfd, FileData* data){
    int rc;

    //さすがにサイズが大きいのでコピーせずにインデックスだけ変換
    if(data == NULL){
        return -1;
    }

    htonFileData(data);
    rc = send(sockfd, data, sizeof(*data), 0);
    if(rc < 0){
        printf("send FileData fail\n");
        return -1;
    }
    return rc;
}

int recvFileData(int sockfd, FileData* data){
    int rc;

    //さすがにサイズが大きいのでコピーせずにインデックスだけ変換
    if(data == NULL){
        return -1;
    }

    rc = recv(sockfd, data, sizeof(*data), 0);
    if(rc < 0){
        printf("recv FileData fail\n");
        return -1;
    }
    ntohFileData(data);
    return rc;
}

/**********/
/*FileAttr*/
/**********/
void printFileAttr(FileAttr* attr){
    printf("[FileAttr]\npath: %s\n", attr->path);
    printstat(&(attr->st));
}

void htonFileAttr(FileAttr* dst, FileAttr* frm){
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    htonstat(&dst->st, &frm->st);
}

void ntohFileAttr(FileAttr* dst, FileAttr* frm){
    if(dst == NULL | frm == NULL){
        return;
    }
    memcpy(dst, frm, sizeof(*frm));
    ntohstat(&dst->st, &frm->st);
}

int sendFileAttr(int sockfd, FileAttr* attr){
    int rc;
    FileAttr _attr;

    if(attr == NULL){
        return -1;
    }

    htonFileAttr(&_attr, attr);
    rc = send(sockfd, &_attr, sizeof(*attr), 0);
    if(rc < 0){
        printf("send FileAttr fail\n");
        return -1;
    }
    return rc;
}

int recvFileAttr(int sockfd, FileAttr* attr){
    int rc;
    FileAttr _attr;

    if(attr == NULL){
        return -1;
    }

    htonFileAttr(&_attr, attr);
    rc = send(sockfd, &_attr, sizeof(*attr), 0);
    if(rc < 0){
        printf("recv FileAttr fail\n");
        return -1;
    }
    return rc;
}