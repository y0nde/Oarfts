#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include "structCommon.h"

int main(void)
{ 
  __SYSCALL_SLONG_TYPE t;
  size_t s1;
  ssize_t s2;
  pid_t pid;
  int a;
  long b;
  short c;
  FileData data;
  
  printf("FileData %ld\n", sizeof(FileData));
  printf("int %ld\n", sizeof(int));
  printf("long %ld\n", sizeof(long));
  printf("short %ld\n", sizeof(short));
  printf("uint32_t %ld\n", sizeof(uint32_t));
  printf("uint16_t %ld\n", sizeof(uint16_t));
  printf("__SYSCALL_SLONG_TYPE %ld\n", sizeof(__SYSCALL_SLONG_TYPE));
  printf("time_t %ld\n", sizeof(time_t));
  printf("__blksize_t %ld\n", sizeof(__blksize_t));
  printf("__blkcnt_t %ld\n", sizeof(__blkcnt_t));
  printf("dev_t %ld\n", sizeof(dev_t));
  printf("gid_t %ld\n", sizeof(gid_t));
  printf("mode_t %ld\n", sizeof(mode_t));
  printf("__nlink_t %ld\n", sizeof(__nlink_t));
  printf("__ino_t %ld\n", sizeof(__ino_t));
  printf("__dev_t %ld\n", sizeof(__dev_t));
  printf("__off_t %ld\n", sizeof(__off_t));
  printf("__uid_t %ld\n", sizeof(__uid_t));
  return 0;
}
