#include "entry.h"
#include <stdio.h>

void printAttr(void* _dstat){
    if(_dstat == NULL){
        return;
    }
    struct Attribute* dstat = _dstat;
    printf("%s %ld %ld %ld \n", dstat->path, dstat->st.st_size, dstat->st.st_mtime, dstat->st.st_ctime);
}



