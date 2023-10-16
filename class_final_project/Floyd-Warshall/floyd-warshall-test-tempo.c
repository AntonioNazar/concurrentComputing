#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#define NumMuitoGrande 10000000
int min(int a,int b){
    if(a<=b){
        return a;
    }
    else{
        return b;
    }
}
// V= tamanho do grafo (linha ou coluna)

void floyd_warshall(int* dist,int V){
    for (int k=0; k < V; k++)
    {
        for (int i=0; i < V; i++)
        {
            for (int j=0; j < V; j++)
            {   if(k!=i){
                    if (*((dist+k)+i*V) != NumMuitoGrande &&  *((dist+j)+k*V) != NumMuitoGrande){
                        *((dist+j)+i*V) = min(*((dist+j)+i*V), *((dist+k)+i*V) + *((dist+j)+k*V));
                    }
                }
            }
        }
    }
}
void adicionarAresta(int tamanho,int* matriz,int v1,int v2, int peso){
    *((matriz+v2)+v1*tamanho) = peso;
}

int main(){
    double inicio, fim, delta;
    int V = 2000;
    int *matriz = (int *)malloc(V * V * sizeof(int));
    for (int i = 0; i < V;i++){
        for (int j = 0; j < V;j++){
            *((matriz+i)+V*j) = NumMuitoGrande;
        }
    }
    int aux = 0;
    for (int i = 0; i < V;i++){
        if(i!=V-1){
            adicionarAresta(V, matriz, i, i + 1, 3);
        }
    }
    adicionarAresta(V, matriz, V - 1, 0, 3);

    for (int i = 0; i < V; i++)
    {
        *((matriz + i) + i * V) = 0;
        }
        GET_TIME(inicio);
        floyd_warshall(matriz, V);
        GET_TIME(fim);
        delta = fim - inicio;

        printf("o tempo de execucao foi: %f", delta);
        return 0;
}