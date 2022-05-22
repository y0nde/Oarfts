#pragma once
#include "structCommon.h"

/*Request*/
typedef struct ReadRequest {
    req_t type;
    char path[PATH_MAX];
    int fd;
    int offset;
    int size;
    char padding[52];
} ReadRequest;
void printReadRequest(ReadRequest* req);
void ntohReadRequest(ReadRequest* req);
void htonReadRequest(ReadRequest* req);

/*Response*/
typedef struct ReadResponse{
    req_t req;
    res_t res;
    int fd;
    int chunks;
    char padding[56];
} ReadResponse;
void printReadResponse(ReadResponse *res);
void ntohReadResponse(ReadResponse *res);
void htonReadResponse(ReadResponse *res);


//Open
/*Request*/
typedef struct OpenRequest {
    req_t type;
    char path[PATH_MAX];
    int mode;
    char padding[60];
} OpenRequest;
void printOpenRequest(OpenRequest *req);
void ntohOpenRequest(OpenRequest* req);
void htonOpenRequest(OpenRequest* req);

/*Response*/
typedef struct OpenResponse{
    req_t req;
    res_t res;
    int fd;
    char padding[56];
} OpenResponse;
void printOpenResponse(OpenResponse *res);
void ntohOpenResponse(OpenResponse *res);
void htonOpenResponse(OpenResponse *res);


//Write
/*Request*/
typedef struct WriteRequest {
    req_t type;
    char path[PATH_MAX];
    int fd;
    int offset;
    int size;
    char padding[52];
} WriteRequest;
void printWriteRequest(WriteRequest *req);
void ntohWriteRequest(WriteRequest* req);
void htonWriteRequest(WriteRequest* req);

/*Response*/
typedef struct WriteResponse{
    req_t req;
    res_t res;
    int fd;
    int chunks;
    char padding[56];
} WriteResponse;
void printWriteResponse(WriteResponse *res);
void ntohWriteResponse(WriteResponse *res);
void htonWriteResponse(WriteResponse *res);


//Close
/*Request*/
typedef struct CloseRequest {
    req_t type;
    char path[PATH_MAX];
    int fd;
    char padding[60];
} CloseRequest;
void printCloseRequest(CloseRequest *req);
void ntohCloseRequest(CloseRequest* req);
void htonCloseRequest(CloseRequest* req);

/*Response*/
typedef struct CloseResponse{
    req_t req;
    res_t res;
    char padding[64];
} CloseResponse;
void printCloseResponse(CloseResponse *res);
void ntohCloseResponse(CloseResponse *res);
void htonCloseResponse(CloseResponse *res);


//Stat
/*Request*/
typedef struct StatRequest {
    req_t type;
    char path[PATH_MAX];
    char padding[64];
} StatRequest;
void printStatRequest(StatRequest *req);
void ntohStatRequest(StatRequest* req);
void htonStatRequest(StatRequest* req);

/*Response*/
typedef struct StatResponse{
    req_t req;
    res_t res;
    char padding[64];
} StatResponse;
void printStatResponse(StatResponse *res);
void ntohStatResponse(StatResponse *res);
void htonStatResponse(StatResponse *res);


//Readdir
/*Request*/
typedef struct ReaddirRequest {
    req_t type;
    char path[PATH_MAX];
    char padding[64];
} ReaddirRequest;
void printReaddirRequest(ReaddirRequest *req);
void ntohReaddirRequest(ReaddirRequest* req);
void htonReaddirRequest(ReaddirRequest* req);

/*Response*/
typedef struct ReaddirResponse{
    req_t req;
    res_t res;
    char padding[64];
} ReaddirResponse;
void printReaddirResponse(ReaddirResponse *res);
void ntohReaddirResponse(ReaddirResponse *res);
void htonReaddirResponse(ReaddirResponse *res);

int testClient();

int testServer();
