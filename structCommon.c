#include <stdio.h>
#include <sys/stat.h>
#include "structCommon.h"

void printRequest(Request* req){
    printf("[Request]\ntype: %d\n", req->type);
    printf("path: %s\n", req->path);
    printf("data: %s\n", req->data);
}

void printResponse(Response* res){
    printf("[Response]\ntype: %d\n", res->type);
}

void printFileData(FileData* data){
    data->data[1023] = '\0';
    printf("[FileData]\ndata: %s\n", data->data);
}

void printStat(struct stat* st){
    printf("stat->st_size: %ld", st->st_size);
    printf("stat->st_mode: %d", st->st_mode);
    printf("stat->st_ctime: %ld", st->st_ctime);
    printf("stat->st_mtime: %ld", st->st_mtime);
    printf("stat->st_nlink: %ld", st->st_nlink);
}

void printFileAttr(FileAttr* attr){
    printf("[FileAttr]\npath: %s\n", attr->path);
    printStat(&(attr->st));
}
