#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
int linha1;
int coluna1;
int linha2;
int coluna2;
int tamanho1;
int tamanho2;
float *matriz1;
float *matriz2;
float *saida;
int NTHREADS;

void* multiplica (void* arg){
    int identificador = *(int *)arg;
    if(identificador==(NTHREADS-1)){
        for (int i=identificador*(linha1/NTHREADS); i<linha1;i++)
        {
            for (int j = 0; j < coluna2; j++)
            {
                for (int k = 0; k < linha2;k++)
                {   
                    *(saida + i * coluna2 + j) += *(matriz1 + i * coluna1 + k) * (*(matriz2 + k * coluna2 + j));
                }   
            }
        }
    }
    else{
        for (int i = identificador*(linha1/NTHREADS); i<(identificador+1)*(linha1/NTHREADS);i++)
        {
            for (int j = 0; j < coluna2; j++)
            {
                for (int k = 0; k < linha2;k++)
                {   
                    *(saida + i * coluna2 + j) += *(matriz1 + i * coluna1 + k) * (*(matriz2 + k * coluna2 + j));
                }   
            }
        }
    }
    pthread_exit(NULL);
    return NULL; // evitar warning
}

int main(int argc, char* argv[]){
    double inicio, fiminicializacao,fimprocessamento,fimfinalizacao, delta;
    double tempoTotal = 0;
    FILE *descritorArquivo;
    size_t ret;
    GET_TIME(inicio);
    // Checar se a entrada foi correta 
    if(argc!=5){
        printf("Erro nas entradas do arquivo\n");
        return 2;
    }
    // Abrir arquivos para leitura binaria
    descritorArquivo = fopen(argv[1], "rb");
    if(!descritorArquivo){
        fprintf(stderr, "Erro de abertura do primeiro arquivo\n");
        return 3;
    }
    ret = fread(&linha1, sizeof(int), 1, descritorArquivo);
    if(!ret){
        fprintf(stderr, "Erro na leitura da linha da matriz1\n");
        return 4;
    }
    ret = fread(&coluna1, sizeof(int), 1, descritorArquivo);
    if(!ret){
        fprintf(stderr, "Erro na leitura da coluna da matriz1\n");
        return 4;
    }
    // Cria a matriz 1 no programa
    tamanho1 = linha1 * coluna1;
    matriz1 = (float *)(malloc(sizeof(float) * tamanho1));
    if(!matriz1){
        printf("Erro Alocacao matriz1\n");
        return 1;
    }
    // Le a matriz 1 do arquivo
    ret = fread(matriz1, sizeof(float), tamanho1, descritorArquivo);
    if(ret!=tamanho1){
        fprintf(stderr, "Erro na leitura da matriz\n");
        return 4;
    }
    fclose(descritorArquivo);
    // Abertura do segundo arquivo
    descritorArquivo = fopen(argv[2], "rb");
    if(!descritorArquivo){
        fprintf(stderr, "Erro de abertura do segundo arquivo\n");
        return 3;
    }
    // Pegar parametros para a segunda matriz
    ret = fread(&linha2, sizeof(int), 1, descritorArquivo);
    if(!ret){
        fprintf(stderr, "Erro na leitura da linha da matriz2\n");
        return 4;
    }
    ret = fread(&coluna2, sizeof(int), 1, descritorArquivo);
    if(!ret){
        fprintf(stderr, "Erro na leitura da coluna da matriz2\n");
        return 4;
    }
    // Cria a segunda matriz
    matriz2 = (float *)(malloc(sizeof(float) * linha2 * coluna2));
    if(!matriz2){
        printf("Erro Alocacao matriz2\n");
        return 1;
    }
    tamanho2 = linha2 * coluna2;
    // Le a matriz 2 do arquivo
    ret = fread(matriz2, sizeof(float), tamanho2, descritorArquivo);
    if(ret!=tamanho2){
        fprintf(stderr, "Erro na leitura da matriz\n");
        return 4;
    }
    fclose(descritorArquivo);

    NTHREADS = atoi(argv[4]);
    if(NTHREADS<=0){
        printf("Número de threads precisa ser um número positivo maior que 0\n");
        return 6;
    }
    if(NTHREADS>linha1){
        NTHREADS = linha1;
    }
    pthread_t tid[NTHREADS];
    // Cria a matriz de saida
    saida = (float *)(malloc(sizeof(float) * linha1 * coluna2));
    if(!saida){
        printf("Erro Alocacao saida\n");
        return 1;
    }
    // Inicializa a matriz saida com 0 para garantir que não tenha um valor indesejado armazenado previamente nele
    for (int i = 0; i < linha1; i++)
    {
        for (int j = 0; j < coluna2;j++){
            *(saida + i*coluna2 +j) = 0.0f;
        }
    }

    GET_TIME(fiminicializacao);
    delta = fiminicializacao - inicio;
    tempoTotal += delta;
    printf("Tempo de inicialização %lf segundos\n", delta);
    // Multiplicação das matrizes
    //Fluxos
    for(int i = 0; i < NTHREADS;i++){
        int*p = malloc(sizeof(int));
        *p = i;
        pthread_create((tid+i),NULL,multiplica,(void*)p);
    }
    //Join
    for (int i = 0; i < NTHREADS;i++){
        pthread_join(*(tid + i),NULL);
    }
    GET_TIME(fimprocessamento);
    delta = fimprocessamento - fiminicializacao;
    tempoTotal += delta;
    printf("Tempo de execucao %lf segundos\n", delta);
    // Abre o arquivo para escrita
    descritorArquivo = fopen(argv[3], "wb");
    if(!descritorArquivo){
        fprintf(stderr, "Erro na abertura da saida\n");
        return 3;
    }
    //  Escreve no arquivo de saida as informações
    ret = fwrite(&coluna1, sizeof(int), 1, descritorArquivo);
    ret = fwrite(&linha2, sizeof(int), 1, descritorArquivo);
    ret = fwrite(saida, sizeof(float), coluna1 * linha2, descritorArquivo);
    if(ret!=coluna1*linha2){
        fprintf(stderr, "Erro de escrita no arquivo\n");
        return 5;
    }

    fclose(descritorArquivo);
    free(matriz1);
    free(matriz2);
    free(saida);
    GET_TIME(fimfinalizacao);
    delta = fimfinalizacao - fimprocessamento;
    tempoTotal += delta;
    printf("Tempo parte final %lf segundos\n", delta);
    printf("Tempo total %lf segundos\n", tempoTotal);
    pthread_exit(NULL);
    return 0;
}