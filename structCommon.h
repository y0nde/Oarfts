#pragma once
#include <sys/stat.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef enum req_t {
    NONE,
    OPEN,
    CLOSE,
    READ,
    WRITE,
    STAT,
    READDIR,
} req_t;

typedef enum res_t {
    DENY,
    YES,
    NO,
    DISCONNECT,
    BUSY,
    WAIT
} res_t;

typedef struct Request{
    req_t type;
    char path[256];
    char data[64];
} Request;

typedef struct Response{
    res_t type;
} Response;

typedef struct FileData{
    int index;
    char data[1024];
} FileData;

typedef struct FileAttr{
    char path[256];
    struct stat st;
} FileAttr;

void printRequest(Request* req);
void printResponse(Response* res);
void printstat(struct stat* st);
void printFileData(FileData* data);
void printFileAttr(FileAttr* attr);

void htonRequest(Request* dst, Request* frm);
void ntohRequest(Request* dst, Request* frm);
void htonResponse(Response* dst, Response* frm);
void ntohResponse(Response* dst, Response* frm);
void ntohstat(struct stat* dst, struct stat* frm);
void htonstat(struct stat* dst, struct stat* frm);
void htonFileData(FileData* data);
void ntohFileData(FileData* data);
void htonFileAttr(FileAttr* dst, FileAttr* frm);
void ntohFileAttr(FileAttr* dst, FileAttr* frm);


int sendRequest(int sockfd, Request* preq);
int recvRequest(int sockfd, Request* preq);
int sendResponse(int sockfd, Response* preq);
int recvResponse(int sockfd, Response* preq);
int sendstat(int sockfd, struct stat* pst);
int recvstat(int sockfd, struct stat* pst);
int sendFileData(int sockfd, FileData* data);
int recvFileData(int sockfd, FileData* data);
int sendFileAttr(int sockfd, FileAttr* attr);
int recvFileAttr(int sockfd, FileAttr* attr);
