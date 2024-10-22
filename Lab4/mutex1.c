#include <pthread.h>
#include <stdio.h>

int g_var1 = 0;
pthread_mutex_t mutexA;

void *inc_gv(void *thread_name) {
  int i, j;

  for (i = 0; i < 10; i++) {
pthread_mutex_lock(&mutexA);
    
    g_var1++;


  
    for (j = 0; j < 5000000; j++) {
};
    printf("%s: %d\n", (char *)thread_name, g_var1);
 pthread_mutex_unlock(&mutexA);
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
