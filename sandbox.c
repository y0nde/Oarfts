#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

void printDirent(struct dirent* dir){
    if(dir == NULL){
        return;
    }
    printf("[dirent]\n");
    printf("inode: %ld\n", dir->d_ino);
    printf("type: %d\n", (int)dir->d_type);
    printf("path: %s\n", dir->d_name);
}

void printStat(struct stat* st){
    printf("[stat]\n");
    printf("st_size: %ld\n", st->st_size);
    printf("st_mode: %d\n", st->st_mode);
    printf("st_ctime: %ld\n", st->st_ctime);
    printf("st_mtime: %ld\n", st->st_mtime);
    printf("st_nlink: %ld\n", st->st_nlink);
}

int main(){
    int rc;
    DIR* dir;
    struct dirent* entry;
    struct stat stbuf;

    if(write(44, "aaa", 0) < 0){
        puts("write error");
        return -1;
    }
    dir = opendir(".");
    if(dir == NULL){
        return -1;
    }

    while(1){
        entry = readdir(dir);
        if(entry == NULL){
            break;
        }
        //printDirent(entry);

        rc = stat(entry->d_name, &stbuf);
        if(rc < 0){
            return -1;
        }
        //printStat(&stbuf);
        //puts("");
    }

    closedir(dir);

    //scanf
    char a[32] = {0};
    char b[32] = {0};
    printf(">> ");
    scanf("%s%s", a, b);
    printf("%s\n%s\n", a, b);
    return 0;
}
