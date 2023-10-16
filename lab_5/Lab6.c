#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define T1 4
#define T2 5
#define T3 3
int x = 0;
int escritor = 0;
int leitor = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_leitor;
pthread_cond_t cond_escritor;

void EntraLeitura(int id){
    pthread_mutex_lock(&mutex);
    printf("L[%d] quer ler\n", id);
    while(escritor > 0) {
      printf("L[%d] bloqueou\n", id);
      pthread_cond_wait(&cond_leitor, &mutex);
      printf("L[%d] desbloqueou\n", id);
    }
    leitor++;
    pthread_mutex_unlock(&mutex);
}

void SaiLeitura (int id) {
   pthread_mutex_lock(&mutex);
   printf("L[%d] terminou de ler\n", id);
   leitor=leitor-1;
   if(leitor==0){
       printf("Enviando sinal para escritor\n");
       pthread_cond_signal(&cond_escritor);
   }
   pthread_mutex_unlock(&mutex);
}
void EntraEscrita (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] quer escrever\n", id);
   while((leitor>0) || (escritor>0)) {
     printf("E[%d] bloqueou\n", id);
     pthread_cond_wait(&cond_escritor, &mutex);
     printf("E[%d] desbloqueou\n", id);
   }
   escritor++;
   pthread_mutex_unlock(&mutex);
}
void SaiEscrita (int id) {
   pthread_mutex_lock(&mutex);
   printf("E[%d] terminou de escrever\n", id);
   escritor--;
   printf("Enviado sinal escritor\n");
   pthread_cond_signal(&cond_escritor);
   pthread_cond_broadcast(&cond_leitor);
   pthread_mutex_unlock(&mutex);
}

void* tarefaT1(void* arg){
    int *id = (int *)arg;
    EntraEscrita(*id);
    x = x + 1;
    SaiEscrita(*id);
    sleep(1);
    pthread_exit(NULL);
}
void* tarefaT2(void* arg){
    int *id = (int *)arg;
    EntraLeitura(*id);
    if(x%2==0){
        printf("o valor %d e par\n", x);
    }
    else{
        printf("o valor %d e impar\n", x);
    }
    SaiLeitura(*id);
    sleep(1);
    pthread_exit(NULL);
}
void* tarefaT3(void* arg){
    int* id = (int *)arg;
    EntraLeitura(*id);
    printf("o valor e %d\n",x);
    SaiLeitura(*id);
    sleep(1);
    int bobo1 = 0;
    int bobo2 = 1000;
    while(bobo1<bobo2){
        bobo1++;
    }
    EntraEscrita(*id);
    x = *id;
    SaiEscrita(*id);
    sleep(1);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    pthread_t tid[T1 + T2 + T3];
    int id[T1 + T2 + T3];
    for (int i = 0; i < T1 + T2 + T3;i++){
        id[i] = i + 1;
    }
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leitor, NULL);
    pthread_cond_init(&cond_escritor, NULL);
    for (int i = 0; i < T1;i++){
        if(pthread_create(&tid[i], NULL, tarefaT1, (void *) &id[i])){
            return 1;
        }
    }
    for (int i = 0; i < T2;i++){
        if(pthread_create(&tid[i+T1], NULL, tarefaT2, (void *) &id[i+T1])){
            return 2;
        }
    }
    for (int i = 0; i < T3;i++){
        if(pthread_create(&tid[i+T1+T2], NULL, tarefaT3, (void *) &id[i+T1+T2])){
            return 3;
        }
    }

        pthread_exit(NULL);
    return 0;
}