#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "operacoes.h"


int unitario(struct intervalo a){
    int ulps = 1;
    float next;

    next = nextafterf(a.min, +1.0/0.0);
    while(next != a.max){
        next = nextafterf(next,+1.0/0.0);
        ulps++;
    }
    return (ulps == 1);
}

int main(){
    char op, str1[10], str2[10];
    int m, n, num1, num2;
    float aux;
    struct intervalo *intervalos;

    scanf("%d %d", &m, &n);

    intervalos = malloc(sizeof(struct intervalo)*(m+n));
    for(int i = 0; i < m; i++){
        scanf("%*s %f", &aux);
        intervalos[i].min = min_intervalo(aux);
        intervalos[i].max = max_intervalo(aux);
    }
    
    for(int i = m; i < m+n; i++){
        scanf("%*s %*c %s %c %s", str1, &op, str2);
        // pula o "x" no índice do número e converte para número
        num1 = atoi(str1+1);
        num2 = atoi(str2+1);
        intervalos[i] = faz_op(intervalos[num1-1], intervalos[num2-1], op);
    }

    for(int i = 0; i < m+n; i++){
        printf("X%d = [\t% 1.8e,\t % 1.8e]\n", i+1, intervalos[i].min, intervalos[i].max);
    }

    printf("\nNão unitários:\n");
    for(int i = m; i < m+n; i++){
        if(!unitario(intervalos[i])){
            printf("X%d = [\t% 1.8e,\t % 1.8e]\n", i+1, intervalos[i].min, intervalos[i].max);
        }
    }
}   
