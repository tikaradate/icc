#include <stdio.h>
#include <math.h>

#define INICIAL 3.5
#define ERRO 0.05

// raiz por volta de 3.6
double func(double x){
    x = x/2;
    return((x*x*x) - 6);
}

// phi = third_root(-6)
double deriv(double x){
    return (3*(x*x)/2);
}

int main (){
    int it = 0;
    double x0, x1, erroA;
    x0 = INICIAL;
    do {
        it++;
        x1 = x0 - func(x0)/deriv(x0);
        erroA = fabs((x1-x0)/x1)*100;
        x0 = x1;
    } while(erroA > ERRO);

    printf("raiz = %f, iteracoes = %d\n", x1, it);
    return 0;
}