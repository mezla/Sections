#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <semaphore.h>  /* Semaphore */
#define MAX_THREAD 100

typedef struct {
 int start,end;
} param;
sem_t mysem;
void *count(void *arg){
 sem_wait(&mysem);
 int i =0;
 param *p=(param *)arg;
 printf("\nprintfrom %d  to  %d\n",p->start,p->end);
 for(i =p->start ; i< p->end ; i++){
  printf(" i = %d",i);sleep(1);
 }
 
 sem_post(&mysem);
 return (void*)(1);
}

int main(int argc, char* argv[]) {
 
 if ( sem_init(&mysem,0,5) ) {
  perror("init");
 }
 int n,i;
 pthread_t *threads;
 param *p;

     if (argc != 2) {
      printf ("Usage: %s n\n",argv[0]);
      printf ("\twhere n is no. of threads\n");
      exit(1);
     }

 n=atoi(argv[1]);

    if ((n < 1) || (n > MAX_THREAD)) {
      printf ("arg[1] should be 1 - %d.\n",MAX_THREAD);
      exit(1);
    }

 threads=(pthread_t *)malloc(n*sizeof(*threads));

 p=(param *)malloc(sizeof(param)*n);
 /* Assign args to a struct and start thread */
 for (i=0; i<n; i++) {
  p[i].start=i*100;
  p[i].end=(i+1)*100;
  pthread_create(&threads[i],NULL,count,(void *)(p+i));
 }
 printf("\nWait threads\n");
 sleep(1);
 /* Wait for all threads. */
 int *x = malloc(sizeof(int));
 for (i=0; i<n; i++) {
  pthread_join(threads[i],(void*)x);
 }
 free(p);
 exit(0);
}