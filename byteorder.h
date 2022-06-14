#pragma once

int isLittleEndien();
int isBigEndien();

short bswap2(short i);
int bswap4(int i);
long bswap8(long i);

short ntoh2(short s);
short hton2(short s);

int ntoh4(int i);
int hton4(int i);

long ntoh8(long l);
long hton8(long l);
