#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "list.h"
#include "connection.h"
#include "byteorder.h"
#include "filetransfer.h"
#include "session.h"

typedef struct FileServer {
    List* sessions;
} FileServer;

int handleRequest(ServerSession* session, Request* req){
    int rc;
    req_t type;

    type = ntoh4(req->type);
    switch(type){
        case OPEN:
            ntohOpenRequest((OpenRequest*)req);
            rc = handleOpenRequest(session, (OpenRequest*)req);
            break;
        case CLOSE:
            ntohCloseRequest((CloseRequest*)req);
            rc = handleCloseRequest(session, (CloseRequest*)req);
            break;
        case READ:
            ntohReadRequest((ReadRequest*)req);
            rc = handleReadRequest(session, (ReadRequest*)req);
            break;
        case WRITE:
            ntohWriteRequest((WriteRequest*)req);
            rc = handleWriteRequest(session, (WriteRequest*)req);
            break;
        case STAT:
            ntohStatRequest((StatRequest*)req);
            rc = handleStatRequest(session, (StatRequest*)req);
            break;
        case READDIR:
            ntohReaddirRequest((ReaddirRequest*)req);
            rc = handleReaddirRequest(session, (ReaddirRequest*)req);
            break;
        default:
            rc = 0;
            printf("%d\n", type);
            printf("NONE\n");
    }
    return rc;
}

void* serverWork(void* _session){
    int rc;
    ServerSession* session = _session;
    Request req = {0};

    while(1){
        rc = recvRequest(session->clientfd, &req);
        if(rc < 0 ){
            freeServerSession(session);
            break;
        }

        rc = handleRequest(session, &req);
        if(rc < 0){
            freeServerSession(session);
            break;
        }
    }
    printf("disconnect\n");
    return NULL;
}

int startServer(short port){
    FileServer server;
    ServerSession* session;

    int rc, serverfd, clientfd;

    server.sessions = newList();

    serverfd = getServerSock(port);
    if(serverfd < 0){
        return -1;
    }
    
    while(1){
        clientfd = acceptSock(serverfd);
        if(clientfd < 0){
            continue;
        }
        printf("accept\n");
        session = newServerSession(clientfd);
        push_front(server.sessions, &server, sizeof(ServerSession));

        pthread_create(&session->thread, NULL, serverWork, session);
    }
}

int main(){
    int rc;

    rc = startServer(8080);

    return rc;
}
