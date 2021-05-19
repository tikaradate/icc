#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "operacoes.h"

int main(){
    char op, str1[10], str2[10];
    int m, n, num1, num2;
    float aux;
    struct intervalo *intervalos;

    scanf("%d %d", &m, &n);

    intervalos = malloc(sizeof(struct intervalo)*(m+n));
    // Lê os m primeiros números
    for(int i = 0; i < m; i++){
        scanf("%*s %f", &aux);
        intervalos[i].min = min_intervalo(aux);
        intervalos[i].max = max_intervalo(aux);
        intervalo_valido(intervalos[i]);
    }
    
    // Lê as n operações restantes
    for(int i = m; i < m+n; i++){
        scanf("%*s %*c %s %c %s", str1, &op, str2);
        // pula o "x" no índice do número e converte o restante para integer
        num1 = atoi(str1+1);
        num2 = atoi(str2+1);
        intervalos[i] = faz_op(intervalos[num1-1], intervalos[num2-1], op);
        intervalo_valido(intervalos[i]);
    }
    // Escreve os intervalos lidos
    for(int i = 0; i < m+n; i++){
        printf("X%d = [% 1.8e,\t % 1.8e]\n", i+1, intervalos[i].min, intervalos[i].max);
    }
    // Escreve os intervalos não unitários
    printf("\nNão unitários:\n");
    for(int i = m; i < m+n; i++){
        if(!unitario(intervalos[i])){
            printf("X%d = [% 1.8e,\t% 1.8e]\n", i+1, intervalos[i].min, intervalos[i].max);
        }
    }
}   
