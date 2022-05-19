#include <stdio.h>

int isLittleEndien(){
    int i = 1;

    if(*(char*)&i == 1){
        return 1;
    }else{
        return 0;
    }
}

int isBigEndien(){
    int i = 1;
    
    if(*(char*)&i == 1){
        return 0;
    }else{
        return 1;
    }
}

short bswap2(short i){
    char in[2];
    char out[2];

    *(short*)in = i;
    out[0] = in[1];
    out[1] = in[0];
    return *(short*)out;
}

int bswap4(int i){
    char in[4];
    char out[4];

    *(int*)in = i;
    out[0] = in[3];
    out[1] = in[2];
    out[2] = in[1];
    out[3] = in[0];
    return *(int*)out;
}

long bswap8(long i){
    char in[8];
    char out[8];

    *(long*)in = i;
    out[0] = in[7];
    out[1] = in[6];
    out[2] = in[5];
    out[3] = in[4];
    out[4] = in[3];
    out[5] = in[2];
    out[6] = in[1];
    out[7] = in[0];
    return *(long*)out;
}

short ntoh2(short s){
    if(isLittleEndien()){
        return bswap2(s);
    }else{
        return s;
    }
}

short hton2(short s){
    if(isLittleEndien()){
        return bswap2(s);
    }else{
        return s;
    }
}

int ntoh4(int i){
    if(isLittleEndien()){
        return bswap4(i);
    }else{
        return i;
    }
}

int hton4(int i){
    if(isLittleEndien()){
        return bswap4(i);
    }else{
        return i;
    }
}

long ntoh8(long l){
    if(isLittleEndien()){
        return bswap8(l);
    }else{
        return l;
    }
}

long hton8(long l){
    if(isLittleEndien()){
        return bswap8(l);
    }else{
        return l;
    }
}

void byteorder_test(){
    printf("%d\n", isBigEndien());
    printf("%d\n", bswap2(1));
    printf("%d\n", bswap2(bswap2(1)));
    printf("%d\n", bswap4(1));
    printf("%d\n", bswap4(bswap4(1)));
    printf("%ld\n", bswap8(1));
    printf("%ld\n", bswap8(bswap8(1)));
    printf("%d\n", ntoh2(1));
    printf("%d\n", ntoh4(1));
    printf("%ld\n", ntoh8(1));
}
