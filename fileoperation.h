#include "transfer.h"
#include "list.h"
#include <sys/stat.h>

#define PATH_MAZ 256
#define CHUNK_SIZE 4048

typedef enum {
    NONE,
    OPEN,
    CLOSE,
    READ,
    WRITE,
    STAT,
    READDIR,
} req_t;

typedef enum {
    DENY,
    YES,
    NO,
    DISCONNECT,
    BUSY,
    WAIT
} res_t;

int requestOpen(int sockfd, char* path, int mode);

int responseOpen(int sockfd, struct Payload request);

int requestClose(int sockfd, int fd);

int responseClose(int sockfd, struct Payload request);

int requestRead(int sockfd, int fd, char* buf, int offset, int size);

int responseRead(int sockfd, struct Payload request);

int requestWrite(int sockfd, int fd, char* buf, int offset, int size);

int responseWrite(int sockfd, struct Payload request);

void swapStat(struct stat* stbuf);

int requestStat(int sockfd, char* path, struct stat* stbuf);

int responseStat(int sockfd, struct Payload request);

struct dirstat {
    char path[256];
    struct stat st;
};

void printdirstat(void* dstat);

List* requestReaddir(int sockfd, char* path);

int responseReaddir(int sockfd, struct Payload request);
