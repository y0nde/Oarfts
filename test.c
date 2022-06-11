#include "connection.h"
#include "fileoperation.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void client(){
    int rc, fd, server;
    char* path1 = "./sample.txt";
    char rbuf[2000000] = {0};
    char* wbuf = "fileoperation test\n";

    //connect
    server = getClientSock("127.0.0.1", 8080);
    puts("connect clear");

    //Open
    if((fd = requestOpen(server, path1, O_RDWR)) < 0){
        puts("open fail");
        exit(EXIT_FAILURE);
    }
    puts("open clear");

    //Read
    if(requestRead(server, fd, rbuf, 0, 2000000) < 0){
        puts("read fail");
        exit(EXIT_FAILURE);
    }
    puts("read clear");
    //printf("[READ]:\n%s", rbuf);

    //Write
    if(requestWrite(server, fd, wbuf, 1000, strlen(wbuf) + 1) < 0){
        puts("write fail");
        exit(EXIT_FAILURE);
    }
    puts("write clear");

    //Close
    if((requestClose(server, fd)) < 0){
        puts("close fail");
        exit(EXIT_FAILURE);
    }
    puts("close clear");

    //Stat
    struct stat stbuf;
    if((requestStat(server, path1, &stbuf)) < 0){
        puts("stat fail");
        exit(EXIT_FAILURE);
    }
    printf("%ld %ld %ld \n", stbuf.st_size, stbuf.st_mtime, stbuf.st_ctime);
    puts("stat clear");

    //readdir
    List* stats;
    struct dirstat dstat;
    char* path2 = ".";
    if((stats = requestReaddir(server, path2)) < 0){
        puts("readdir fail");
        exit(EXIT_FAILURE);
    }
    printList(stats, printdirstat);
    puts("readdir clear");
}

void server(){
    int rc, server, client;
    struct Payload* payload;

    server = getServerSock(8080);
    puts("listen start at 8080");
    while(1){
        //コネクションの受付
        client = acceptSock(server);
        puts("accpet client");
        while(1){
            //リクエスト
            payload = recvPayload(client);
            if(payload == NULL){
                break;
            }

            puts("recv request");

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
}

int main(int argc, char** argv){
    if(argc < 2){
        puts("need args");
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1], "client") == 0){
        client();
    }else if(strcmp(argv[1], "server") == 0){
        server();
    }
    return 0;
}





