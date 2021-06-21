#include <stdio.h>
#include <math.h>

#define INICIAL 6
#define ERRO 0.0005

// raiz por volta de 2
double func(double x){
    return((x*x) + x - 6);
}

double phi(double x){
    return (sqrt(6-x));
}

int main (){
    int it = 0;
    double x0, x1, erroA;
    x0 = INICIAL;
    do {
        it++;
        x1 = phi(x0);
        erroA = fabs((x1-x0)/x1)*100;
        x0 = x1;
    } while(erroA > ERRO);

    printf("appr. raiz = %f, iteracoes = %d\n", x1, it);
    return 0;
}