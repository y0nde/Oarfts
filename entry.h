#pragma once

#include <sys/stat.h>
#include <stdlib.h>

typedef struct Attribute
{
    char path[256];
    struct stat st;
} Attribute;

void printAttr(void* _dstat);

