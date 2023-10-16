#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define N 5 //tamanho do buffer
#define P 4 //qtde de threads produtoras
#define C 4 //qtde de threads consumidoras

int countador_cheio = 0;
sem_t semCheio,semVazio,prod,cons;
int Buffer[N];

//inicializa o buffer
void IniciaBuffer(int n) {
  for(int i=0; i<n; i++){
    Buffer[i] = 0;
  }
}

//imprime o buffer
void ImprimeBuffer(int n) {
  for(int i=0; i< n; i++){
    printf("%d ", Buffer[i]);
  }
  printf("\n");
}

void Insere (int item, int id) {
  static int in = 0;
  sem_wait(&semVazio);
  printf("P[%d] quer inserir\n", id);
  sem_wait(&prod);
  for (int i = 0; i < N;i++){
    Buffer[in] = item;
    in = (in + 1) % N;
  }
  countador_cheio = N;
  printf("P[%d] inseriu\n", id);
  ImprimeBuffer(N);
  sem_post(&prod);
  for (int i = 0; i < N;i++){
    sem_post(&semCheio);
  }
}

int Retira (int id) {
  int item;
  static int out = 0;
  sem_wait(&semCheio);
  printf("C[%d] quer consumir\n", id);
  sem_wait(&cons);
  item = Buffer[out];
  Buffer[out] = 0;
  out = (out + 1)%N;
  countador_cheio--;
  printf("C[%d] consumiu %d\n", id, item);
  ImprimeBuffer(N);
  sem_post(&cons);
  if(countador_cheio==0){
    sem_post(&semVazio);
  }
  return item;
}

//thread consumidora
void * consumidor(void * arg) {
  int *id = (int *) arg;
  printf("Sou a thread consumidora %d\n", *id);
  while(1) {
    //faz o processamento do item
    Retira(*id);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}

void * produtor(void * arg) {
  int *id = (int *) arg;
  printf("Sou a thread produtora %d\n", *id);
  while(1) {
    //produzindo o item
    Insere(*id, *id);
    sleep(1);
  } 
  free(arg);
  pthread_exit(NULL);
}

int main(int argc,char*argv[]){
  
  pthread_t tid[P+C];
  int *id[P+C]; 

  //aloca espaco para os IDs das threads
  for(int i=0; i<P+C;i++) {
    id[i] = malloc(sizeof(int));
    if(id[i] == NULL) exit(-1);
    *id[i] = i+1;
  }

  //inicia os semaforos
  sem_init(&semCheio, 0, 0);
  sem_init(&prod, 0, 1);
  sem_init(&cons, 0, 1);
  sem_init(&semVazio, 0, 1);
  
  // aloca espaco para os IDs das threads
  for(int i=0; i<P+C;i++) {
    id[i] = malloc(sizeof(int));
    if(id[i] == NULL) exit(-1);
    *id[i] = i+1;
  }

  IniciaBuffer(N);
  
  //cria as threads produtoras
  for(int i=0; i<P; i++) {
    if(pthread_create(&tid[i], NULL, produtor, (void *) id[i])) exit(-1);
  } 
  
  //cria as threads consumidoras
  for(int i=0; i<C; i++) {
    if(pthread_create(&tid[i+P], NULL, consumidor, (void *) id[i+P])) exit(-1);
  } 

  pthread_exit(NULL);
  return 1;

}
