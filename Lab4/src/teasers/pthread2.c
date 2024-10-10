#include <pthread.h>
#include <stdio.h>

// The below code is given by the lab manual
// A better way to show race conditions would probably be by
// removing the delay, and in the incrementing loop, using a higher upper bound.

// void *inc_gv(void *thread_name) {
//   int i, j;
//   for (i = 0; i < 100000; i++) {
//     g_var1++;
//     printf("%s: %d\n", (char *)thread_name, g_var1);
//     fflush(stdout);
//   }
//   return NULL;
// }

int g_var1 = 0;

void *inc_gv(void *thread_name) {
  int i, j;
  for (i = 0; i < 10; i++) {
    g_var1++;
    for (j = 0; j < 5000000; j++) {
    };
    printf("%s: %d\n", (char *)thread_name, g_var1);
    fflush(stdout);
  }
  return NULL;
}

int main() {
  pthread_t TA, TB;
  int TAret, TBret;

  TAret = pthread_create(&TA, NULL, inc_gv, "ThreadA");
  TBret = pthread_create(&TB, NULL, inc_gv, "ThreadB");

  pthread_join(TA, NULL);
  pthread_join(TB, NULL);

  printf("\npthread2 completed\n");
  printf("TAret: %d\n", TAret);
  printf("TBret: %d\n", TBret);
}