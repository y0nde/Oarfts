#include "connection.h"
#include "fileoperation.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

static pthread_mutex_t mutex;

void* client(){
    int rc, fd, server;
    char* path1 = "./sample.txt";
    char rbuf[2000] = {0};
    char* wbuf = "fileoperation test\n";

    //connect
    puts("connect");
    server = getClientSock("127.0.0.1", 8080);
    if(server < 0){ return NULL; }
    pthread_mutex_lock(&mutex);
    puts("connect clear");
    pthread_mutex_unlock(&mutex);

    //Open
    puts("Open");
    if((fd = requestOpen(server, path1, O_RDWR)) < 0){
        pthread_mutex_lock(&mutex);
        puts("open fail");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_lock(&mutex);
    puts("open clear");
    pthread_mutex_unlock(&mutex);

    //Read
    puts("Read");
    if(requestRead(server, fd, rbuf, 0, 2000) < 0){
        pthread_mutex_lock(&mutex);
        puts("read fail");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_lock(&mutex);
    puts("read clear");
    //printf("[READ]:\n%s", rbuf);
    pthread_mutex_unlock(&mutex);

    puts("0 size Read");
    //0 size Read
    if(requestRead(server, fd, rbuf, 0, 0) < 0){
        pthread_mutex_lock(&mutex);
        puts("read fail");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_lock(&mutex);
    puts("0 size read clear");
    pthread_mutex_unlock(&mutex);

    //over 0 size size Read
    puts("over 0 size Read");
    if(requestRead(server, fd, rbuf, 80, 0) < 0){
        pthread_mutex_lock(&mutex);
        puts("read fail");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_lock(&mutex);
    puts("over 0 size read clear");
    pthread_mutex_unlock(&mutex);

    //Write
    puts("Write");
    if(requestWrite(server, fd, wbuf, 1000, strlen(wbuf) + 1) < 0){
        pthread_mutex_lock(&mutex);
        puts("write fail");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    pthread_mutex_lock(&mutex);
    puts("write clear");
    pthread_mutex_unlock(&mutex);

    //Close
    puts("Close");
    if((requestClose(server, fd)) < 0){
        pthread_mutex_lock(&mutex);
        puts("close fail");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_lock(&mutex);
    puts("close clear");
    pthread_mutex_unlock(&mutex);

    //Stat
    puts("Stat");
    struct stat stbuf;
    if((requestStat(server, path1, &stbuf)) < 0){
        pthread_mutex_lock(&mutex);
        puts("stat fail");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_lock(&mutex);
    printf("%ld %ld %ld \n", stbuf.st_size, stbuf.st_mtime, stbuf.st_ctime);
    pthread_mutex_unlock(&mutex);
    puts("stat clear");

    //readdir
    puts("Readir");
    List* stats;
    struct Attribute dstat;
    char* path2 = ".";
    if((stats = requestReaddir(server, path2)) < 0){
        pthread_mutex_lock(&mutex);
        puts("readdir fail");
        pthread_mutex_unlock(&mutex);
        return NULL;
    }
    pthread_mutex_lock(&mutex);
    printList(stats, printAttr);
    puts("readdir clear");
    pthread_mutex_unlock(&mutex);
    close(server);
    return NULL;
}

int main(int argc, char** argv){
    pthread_t th1, th2;

    pthread_mutex_init(&mutex, NULL);
    pthread_create(&th1, NULL, client, NULL);
    pthread_create(&th2, NULL, client, NULL);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}





