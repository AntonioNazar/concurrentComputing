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

void floyd_warshall(int* dist,int V)
{
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
    int V = 10; // V= tamanho do grafo (linha ou coluna)
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

    GET_TIME(inicio);
    floyd_warshall(matriz, V);
    GET_TIME(fim);
    delta = fim - inicio;
    for (int i = 0; i < V; i++){
        for (int j = 0; j < V;j++){
            printf("O caminho minimo de %d para %d e: %d \n", i, j, *((matriz+j)+V*i));
        }
        
    }
    printf("o tempo de execucao foi: %f", delta);
    return 0;
}