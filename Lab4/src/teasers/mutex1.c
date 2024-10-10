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

// Using the above code, we realise that pthread2 executes to reach some number
// below 200000, (maybe something like 199985). This is due to the race
// condition between TA and TB. After using a mutex to ensure synchornized
// access, we now have final count as 200000, which is expected.

int g_var1 = 0;
pthread_mutex_t mutexA;

void *inc_gv(void *thread_name) {
  int i, j;
  for (i = 0; i < 100000; i++) {
    pthread_mutex_lock(&mutexA);
    g_var1++;
    pthread_mutex_unlock(&mutexA);
    // for (j = 0; j < 5000000; j++) {
    // };
    printf("%s: %d\n", (char *)thread_name, g_var1);
    fflush(stdout);
  }
  return NULL;
}

int main() {
  pthread_t TA, TB;
  int TAret, TBret;

  pthread_mutex_init(&mutexA, NULL);

  TAret = pthread_create(&TA, NULL, inc_gv, "ThreadA");
  TBret = pthread_create(&TB, NULL, inc_gv, "ThreadB");

  pthread_join(TA, NULL);
  pthread_join(TB, NULL);

  printf("\npthread2 completed\n");
  printf("TAret: %d\n", TAret);
  printf("TBret: %d\n", TBret);
}