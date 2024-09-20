#include "hello.h"
#include <stdio.h>
#include <unistd.h>

void hellocount(void) {
  printf("Hello world from funct2!\n");
  sleep(0x7fffffff);
    // int count = 0;
    // int i;
    // char c;
    // while (1) {
    //   i = fread(&c, 1, 1, stdin);
    //   if (i > 0) {
    //     printf("You have printed this %d times\n", count);
    //     count++;
    //   }
    // }
}
