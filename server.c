#include "connection.h"
#include "fileoperation.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

char* req_t_str[] = {"NONE", "OPEN", "CLOSE", "READ", "WRITE", "STAT", "READDIR", "HEALTH"};

void serverwork(int client){
    int rc;
    struct Payload* payload;

    while(1){
        //リクエスト
        payload = recvPayload(client);
        if(payload == NULL){
            break;
        }

        if(payload->header.type < 8 & payload->header.type >= 0){
            printf("recv %s from client[%d]\n", req_t_str[payload->header.type], client);
        }
        //リクエスト処理
        switch(payload->header.type){
            case OPEN:
                rc = responseOpen(client, *payload);
                break;
            case CLOSE:
                rc = responseClose(client, *payload);
                break;
            case READ:
                rc = responseRead(client, *payload);
                break;
            case WRITE:
                rc = responseWrite(client, *payload);
                break;
            case STAT:
                rc = responseStat(client, *payload);
                break;
            case READDIR:
                rc = responseReaddir(client, *payload);
                break;
            case HEALTH:
            rc = responseHealth(client, *payload);
            break;
            default:
                rc = -1;
                break;
        }

        if(rc < 0){
            break;
        }

        freePayload(payload);
    }
    close(client);
}

void startServer(){ 
    int rc, client, server;

    server = getServerSock(8080);
    if(server < 0){ return ;}

    while(1){
        client = acceptSock(server);
        if(client < 0){ continue; }

        //serverwork(client);
        if((rc = fork()) == 0){
            close(server);
            serverwork(client);
            close(client);
            exit(0);
        }else{
            close(client);
        }
    }
}

int main(int argc, char** argv){
    startServer(); 
    return 0;
}





