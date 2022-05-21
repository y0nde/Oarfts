#pragma once
#include "structCommon.h"

/*Request*/
typedef struct RemoteReadRequest {
    req_t type;
    char path[PATH_MAX];
    int fd;
    int offset;
    int size;
    int index;
    char padding[48];
} RemoteReadRequest;

void printRemoteReadRequest(RemoteReadRequest* rrr);

void ntohRRRequset(RemoteReadRequest* rrr);

void htonRRRequset(RemoteReadRequest* rrr);

/*Response*/
typedef struct RemoteReadResponse{
    req_t req;
    res_t res;
    int fd;
    int chunks;
    char padding[56];
} RemoteReadResponse;

void printRemoteReadResponse(RemoteReadResponse* rrr);

void ntohRemoteReadResponse(RemoteReadResponse *rrr);

void htonRemoteReadResponse(RemoteReadResponse *rrr);

//RemoteOpen
//RemoteWrite
//RemoteClose
//RemoteStat
//RemoteReaddir

int testClient();

int testServer();
