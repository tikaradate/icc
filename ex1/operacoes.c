#include <math.h>
#include "operacoes.h"

struct intervalo faz_op(struct intervalo a, struct intervalo b, char op){
    switch(op){
        case '+':
            return soma(a, b);
            break;
        case '-':
            return subtracao(a, b);
            break;
        case '*':
            return multiplicacao(a, b);
            break; 
        case '/':
            return divisao(a, b);
            break;
    }   
}

float min_intervalo(float a){
    return nextafterf(a, a-1);
}
float max_intervalo(float a){
    return nextafterf(a, a+1);
}

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
