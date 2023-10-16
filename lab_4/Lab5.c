#include <pthread.h>
#include <stdio.h>
#define NTHREADS 5
/* Variaveis globais */
int cond1 = 0;
int cond2 = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_1, cond_2;
/* Arquivos */
void* VolteSempre(void* arg){
   // printf("Thread 4 comecou\n");
    pthread_mutex_lock(&mutex);
      while (cond1 != 1){   
       // printf("Thread 4 se bloqueou por condicao 1\n");
        pthread_cond_wait(&cond_1,&mutex);
       // printf("Thread 4 desbloqueada por condicao 1\n");
    }

     while(cond2!=2){   
      //  printf("Thread 4 se bloqueou por condicao 2\n");
        pthread_cond_wait(&cond_2,&mutex);
       // printf("Thread 4 desbloqueada por condicao 2\n");
    }
    pthread_mutex_unlock(&mutex);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}
void* FiqueAVontade(void* arg){
   // printf("Thread 2 comecou\n");
    pthread_mutex_lock(&mutex);
    while (cond1 != 1)
    {
        //printf("Thread 2 se bloqueou\n");
        pthread_cond_wait(&cond_1,&mutex);
        //printf("Thread 2 desbloqueada\n");
    }
    cond2++;
    if(cond2==2){
        //printf("Condicao 2 sinalizada\n");
        pthread_cond_signal(&cond_2);
    }
    pthread_mutex_unlock(&mutex);
    printf("Fique a vontade\n");
    pthread_exit(NULL);
}
void* Sente_sePorFavor(void* arg){
    //printf("Thread 3 comecou\n");
    pthread_mutex_lock(&mutex);
    while(cond1!=1){
       // printf("Thread 3 se bloqueou\n");
        pthread_cond_wait(&cond_1,&mutex);
        //printf("Thread 3 desbloqueada\n");
    }
    cond2++;
    if(cond2==2){
       // printf("Condicao 2 sinalizada\n");
        pthread_cond_signal(&cond_2);
    }
    pthread_mutex_unlock(&mutex);
    printf("Sente-se por favor\n");
    pthread_exit(NULL);
}
void* SejaBemVindo(void* arg){
    //printf("Thread 1 comecou\n");
    printf("Seja Bem Vindo!\n");
    pthread_mutex_lock(&mutex);
    cond1++;
   // printf("Condicao 1 sinalizada\n");
    pthread_cond_broadcast(&cond_1);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}
/* Main */
int main(int argc,char* argv[]){
    pthread_t threads[NTHREADS];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_1, NULL);
    pthread_cond_init(&cond_2, NULL);
    pthread_create(&threads[0], NULL, VolteSempre, NULL);
    pthread_create(&threads[1], NULL, FiqueAVontade, NULL);
    pthread_create(&threads[2], NULL, Sente_sePorFavor, NULL);
    pthread_create(&threads[3], NULL, SejaBemVindo, NULL);
    for (int i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_1);
    pthread_cond_destroy(&cond_2);
    pthread_exit(NULL);
    return 0;
}