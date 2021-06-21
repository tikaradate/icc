#include <stdio.h>
#include <math.h>

#define INICIAL0 6
#define INICIAL1 5
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
    double x0, x1, x2, erroA;
    x0 = INICIAL0;
    x1 = INICIAL1;
    do {
        it++;
        x2 = x1 - ((func(x1)*(x1-x0))/(func(x1)-func(x0)));
        erroA = fabs((x2-x1)/x2)*100;
        x0 = x1;
        x1 = x2;
    } while(erroA > ERRO);

    printf("raiz = %f, iteracoes = %d\n", x2, it);
    return 0;
}