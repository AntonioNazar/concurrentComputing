#include<stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include <time.h>   
#define NTHREADS 2
float vetor[10000];
void * aumento (void * arg) {
   float* pos = (float *) arg;
   for (float* i = pos; i < (pos + 5000);i++){
       *(i) = *(i) * 1.1;
   }
       pthread_exit(NULL);
}

int main(void) {
    srand(time(NULL));
    for (int i = 0; i < 10000;i++){
        vetor[i]=((rand()%1000)+1);
        printf("%f\n", vetor[i]);
    }

    pthread_t tid[NTHREADS];

    if (pthread_create(&tid[0], NULL, aumento, (void *)&vetor[0])){
        printf("ERRO -- pthread_create\n");
    }
    if (pthread_create(&tid[1], NULL, aumento, (void *)&vetor[5000])){
        printf("ERRO -- pthread_create\n");
    }
    for(int i=0; i<NTHREADS; i++) {
        if (pthread_join(tid[i], NULL)) 
            printf("ERRO -- pthread_join\n");
    }
    
    for (int i = 0; i < 10000;i++){
        printf("%f\n", vetor[i]);
    }

    pthread_exit(NULL);
    return 0;
}