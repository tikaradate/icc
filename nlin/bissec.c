#include <stdio.h>
#include <math.h>

#define XL 2.5
#define XU 5.0
#define ERRO 0.0005

// raiz por volta de 3.6
double func(double x){
    x = x/2;
    return((x*x*x) - 6);
}

int main (){
    int it = 0;
    double xm0, xm1, xl, xu, erroA;
    xl = XL;
    xu = XU;
    do {
        it++;
        xm0 = (xl+xu)/2;
        // passo 3
        if(func(xl)*func(xm0) < 0){
            xu = xm0;
        } else if(func(xl)*func(xm0) > 0){
            xl = xm0;
        } else {
            printf("%f\n",xm0);
            return 0;
        }
        xm1 = (xl + xu)/2;
        erroA = fabs((xm1-xm0)/xm1)*100;
    } while(erroA > ERRO);

    printf("raiz = %f, iteracoes = %d\n", xm0, it);
    return 0;
}