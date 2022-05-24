#pragma once
#include <sys/stat.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PATH_MAZ 256
#define CHUNK_SIZE 1024

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
    char path[PATH_MAZ];
    char data[64];
} Request;

typedef struct Response{
    req_t req;
    res_t res;
    char data[64];
} Response;

typedef struct FileChunk{
    int index;
    int size;
    char data[CHUNK_SIZE];
} FileChunk;

typedef struct FileAttr{
    int errno;
    int index;
    struct stat st;
    char path[PATH_MAZ];
} FileAttr;

void printRequest(Request* req);
void printResponse(Response* res);
void printstat(struct stat* st);
void printFileChunk(FileChunk* data);
void printFileAttr(void* attr);

void htonRequest(Request* req);
void ntohRequest(Request* req);
void htonResponse(Response* res);
void ntohResponse(Response* res);
void ntohstat(struct stat* dst, struct stat* frm);
void htonstat(struct stat* dst, struct stat* frm);
void htonFileChunk(FileChunk* data);
void ntohFileChunk(FileChunk* data);
void htonFileAttr(FileAttr* dst, FileAttr* frm);
void ntohFileAttr(FileAttr* dst, FileAttr* frm);


int sendRequest(int sockfd, Request* preq);
int recvRequest(int sockfd, Request* preq);
int sendResponse(int sockfd, Response* pres);
int sendErrorResponse(int sockfd, Response* pres);
int recvResponse(int sockfd, Response* pres);
int sendstat(int sockfd, struct stat* pst);
int recvstat(int sockfd, struct stat* pst);
int sendFileChunk(int sockfd, FileChunk* data);
int recvFileChunk(int sockfd, FileChunk* data);
int sendFileAttr(int sockfd, FileAttr* attr);
int recvFileAttr(int sockfd, FileAttr* attr);
