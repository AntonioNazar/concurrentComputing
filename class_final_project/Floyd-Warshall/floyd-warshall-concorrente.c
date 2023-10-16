#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#define NTHREADS 7
#define NumMuitoGrande 10000000
int bloq = 0;
int id = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;
struct arg_struct{
    int tamanho;
    int *matriz;
    int identificador;
} ;

void barreira() {
    pthread_mutex_lock(&x_mutex);
    if (bloq == (NTHREADS-1)) { 
      pthread_cond_broadcast(&x_cond);
      bloq=0;
    } else {
      bloq++;
      pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); 
}

int min(int a,int b){
    if(a<=b){
        return a;
    }
    else{
        return b;
    }
}

void *floyd_warshall(void* arg){
    struct arg_struct aux = *(struct arg_struct *)arg;
    int* dist = aux.matriz;
    pthread_mutex_lock(&x_mutex);
    int identificador = id++;
     pthread_mutex_unlock(&x_mutex); 
    int V = aux.tamanho;
    for (int k=0; k < V; k++){
        for (int i=0; i < V; i++){
            for (int j=identificador; j < V; j+=NTHREADS){ 
                *((dist+j)+i*V) = min(*((dist+j)+i*V), *((dist+k)+i*V) + *((dist+j)+k*V));
            }
        }
         barreira();
    }
    pthread_exit(NULL);
}
void adicionarAresta(int tamanho,int* matriz,int v1,int v2, int peso){
    *((matriz+v2)+v1*tamanho) = peso;
}

int main(){
    double inicio, fim, delta;
    pthread_t tid[NTHREADS];
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);
    int V = 10;
    int *matriz = (int *)malloc(V * V * sizeof(int));
    for (int i = 0; i < V;i++){
        for (int j = 0; j < V;j++){
            *((matriz+i)+V*j) = NumMuitoGrande;
        }
    }

    adicionarAresta(V,matriz, 0, 1, 1);
    adicionarAresta(V,matriz, 0, 3, 2);
    adicionarAresta(V,matriz, 0, 5, 3);
    adicionarAresta(V,matriz, 1, 2, 2);
    adicionarAresta(V,matriz, 2, 3, 3);
    adicionarAresta(V,matriz, 3, 6, 1);
    adicionarAresta(V,matriz, 3, 5, -1);
    adicionarAresta(V,matriz, 3, 7, 3);
    adicionarAresta(V,matriz, 3, 8, -2);
    adicionarAresta(V,matriz, 4, 0, 1);
    adicionarAresta(V,matriz, 5, 4, 1);
    adicionarAresta(V,matriz, 6, 4, 2);
    adicionarAresta(V,matriz, 6, 5, 4);
    adicionarAresta(V,matriz, 6, 9, 2);
    adicionarAresta(V,matriz, 7, 5, 7);
    adicionarAresta(V,matriz, 8, 7, 8);
    adicionarAresta(V,matriz, 9, 8, 3);
    
    for (int i=0; i < V; i++){
        *((matriz+i)+i*V) = 0;
    }
    struct arg_struct aux;
    aux.tamanho = V;
    aux.matriz = matriz;
    GET_TIME(inicio);
    for (int i = 0; i < NTHREADS;i++){
        if(pthread_create(&tid[i], NULL, floyd_warshall, (void*)&aux)){
            printf("ERRO -- pthread_create\n");
        }
    }
    for(int i=0; i<NTHREADS; i++) {
        if (pthread_join(tid[i], NULL)) 
            printf("ERRO -- pthread_join\n");
    }
    GET_TIME(fim);
    delta = fim - inicio;
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V;j++){
            printf("O caminho minimo de %d para %d e: %d \n", i, j, *((aux.matriz+j)+V*i));
        }
    }
    printf("o tempo de execucao foi: %f", delta);
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;
}
