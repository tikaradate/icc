#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "edo.h"
#include "edp.h"
#include "diagonal.h"

#define L 6
#define W 8


// problema A

double quadratico(double x)
{
    return (6.0 * x - ((x * x) / 2.0));
}

double nulo()
{
    return 0.0;
}

// problema B

double idem()
{
    return 1.0;
}

double oposto()
{
    return -1.0;
}

double senoQuadrado(double x, double y)
{
    return (sin(x) * sin(x));
}

// problema C

// problema D

double cosFunc(double x, double y)
{
    return 1;
}

int main()
{
    struct EDO edo;
    struct EDP edp;
    struct tridiagonal *td;
    struct pentadiagonal *pd;
    int n, m;
    double *yEDO, **yEDP;
    scanf("%d %d", &n, &m);
    // problema A $ y^{ii} = 6x - 0.5x^2, x \in (0,12), y(0) = 0 e y(12) = 0 $
    // y^{ii} + 0*y^{i} + 0*y{i}  = 6x - 0.5x^2, p = 0, q = 0, r = 6x-0.5x^2

    edo.n = n;
    edo.a = 0;
    edo.b = 12;
    edo.ya = 0;
    edo.yb = 0;
    edo.p = (&nulo);
    edo.q = (&nulo);
    edo.r = (&quadratico);
    yEDO = malloc(sizeof(double) * edo.n);
    td = geraTridiagonal(&edo);
    imprimeTridiagonal(td);
    gaussSeidelEDO(&edo, yEDO);
    // for(int i = 0; i < n; i++){
    //     printf("%f ", y[i]);
    // }
    // printf("\n");
    free(yEDO);
    // problema B $ T_{xx} + T_{yy} - T = sin^2(x), (x,y) \in \Omega = (0,L) \times (0,W)
    //                                            , T(0,y) = 20, T(L,y) = 45, T(x,0) = 0, T(x,W) = 100$
    edp.n = n;
    edp.m = m;
    edp.ax = 0;
    edp.bx = L;
    edp.ay = 0;
    edp.by = W;
    edp.yax = 0;
    edp.ybx = 100;
    edp.yay = 20;
    edp.yby = 45;
    edp.c = (&idem);
    edp.d = (&idem);
    edp.e = (&oposto);
    edp.r = (&senoQuadrado);
    yEDP = malloc(sizeof(double*)*n);
    for(int i = 0; i < n; i++){
        yEDP[i] = malloc(sizeof(double)*n);
    }
    pd = geraPentadiagonal(&edp);
    imprimePentadiagonal(pd);
    gaussSeidelEDP(&edp, yEDP);
    // problema C $ y^{ii} + y = 0, x \in (0,1), y(0) = 0 e y(1) = 1 $
    // edo.a = 0;
    // edo.b = 1;
    // edo.ya = 0;
    // edo.yb = 1;
    // edo.p = (&nulo);
    // edo.q = (&idem);
    // edo.r = (&nulo);

    // problema D $ u_{xx} + u_{yy} = -cos(x-y), (x,y) \in (0,\pi) \crossproduct (0,\frac{\pi,2})
    //                                            , u(0,y) = cos(y), u(\pi,y) = -cos(y), u(x,0) = cos(x), u(x,\frac{\pi,2}) = 0$
    // edp.n = n;
    // edp.m = m;
    // edp.ax = 0;
    // edp.bx = M_PI;
    // edp.ay = 0;
    // edp.by = M_PI_2;
    // edp.yax = 0;
    // edp.ybx = 100;
    // edp.yay = 20;
    // edp.yby = 0;
    // edp.c = (&idem);
    // edp.d = (&idem);
    // edp.e = (&oposto);
    // edp.r = (&senoQuadrado);
    // y = malloc(sizeof(double)*n*m);
    // gaussSeidelEDP(&edp);
    // free(y);
}
