#include "hello.h"
#include <stdio.h>
#include <unistd.h>

int main(void) {
  printf("Hello world from main!\n");
  sleep(0x5fffff);
  helloprint();
  hellocount();
  printf("Bye!\n");
  return 0;
}
