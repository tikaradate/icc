#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fenv.h>
#include "operacoes.h"

struct intervalo soma(struct intervalo a, struct intervalo b){
    struct intervalo resultado;
    resultado.min = min_intervalo(a.min + b.min);
    resultado.max = max_intervalo(a.max + b.max);
    return resultado;
}

struct intervalo subtracao(struct intervalo a, struct intervalo b){
    struct intervalo resultado;
    resultado.min = min_intervalo(a.min - b.max);
    resultado.max = max_intervalo(a.max - b.min);
    return resultado;
}   

struct intervalo multiplicacao(struct intervalo a, struct intervalo b){
    struct intervalo resultado;
    resultado.min = fminf(fminf(a.min*b.min,a.min*b.max),fminf(a.max*b.min,a.max*b.max));
    resultado.max = fmaxf(fmaxf(a.min*b.min,a.min*b.max),fmaxf(a.max*b.min,a.max*b.max));
    resultado.min = min_intervalo(resultado.min);
    resultado.max = max_intervalo(resultado.max);
    return resultado;
}   

struct intervalo divisao(struct intervalo a, struct intervalo b){
    struct intervalo resultado;
    resultado.min = fminf(fminf(a.min/b.min,a.min/b.max),fminf(a.max/b.min,a.max/b.max));
    resultado.max = fmaxf(fmaxf(a.min/b.min,a.min/b.max),fmaxf(a.max/b.min,a.max/b.max));
    resultado.min = min_intervalo(resultado.min);
    resultado.max = max_intervalo(resultado.max);
    return resultado;
}

struct intervalo faz_op(struct intervalo a, struct intervalo b, char op){
    struct intervalo resultado;
    feclearexcept(FE_ALL_EXCEPT);
    switch(op){
        case '+':
            resultado = soma(a, b);
            break;
        case '-':
            resultado = subtracao(a, b);
            break;
        case '*':
            resultado = multiplicacao(a, b);
            break; 
        case '/':
            resultado = divisao(a, b);
            break;
    } 
    // testa o underflow e overflow usando a biblioteca fenv.h
    if(fetestexcept(FE_UNDERFLOW)){
        fprintf(stderr, "Underflow :\n [% 1.8e, % 1.8e] %c [% 1.8e, % 1.8e]\nEncerrando o programa\n", a.min, a.max, op, b.min, b.max);
        exit(-1);
    }
    if(fetestexcept(FE_OVERFLOW)){
        fprintf(stderr, "Overflow :\n [% 1.8e, % 1.8e] %c [% 1.8e, % 1.8e]\nEncerrando o programa\n", a.min, a.max, op, b.min, b.max);
        exit(-1);   
    }
    return resultado;  
}
// utilizando os macros de infinito, pega o próximo número na determinada direção
float min_intervalo(float a){
    return nextafterf(a, -INFINITY);
}
float max_intervalo(float a){
    return nextafterf(a, +INFINITY);
}
// conta quantos ulps tem entre o mínimo e o máximo
int unitario(struct intervalo a){
    int ulps;
    float next;

    next = nextafterf(a.min, +1.0/0.0);
    ulps = 1;
    while(next != a.max){
        next = nextafterf(next,+1.0/0.0);
        ulps++;
        if(ulps > 2){
            break;
        }
    }
    return (ulps == 1);
}

int intervalo_valido(struct intervalo a){
    if ((isgreater(a.min, a.max)) || isnan(a.min) || isnan(a.max)){
        return 0;
    }
    return 1;
}
