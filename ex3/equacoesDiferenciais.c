#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "diagonal.h"
#include "utils.h"

#define L    6
#define W    8
#define PI   3.14159265358979323846
#define PI_2 1.57079632679489661923

double normaL2Residuo(int n, double *res)
{
	double norma = 0;
	for (int i = 0; i < n; i++)
	{
		norma += (res[i] * res[i]);
	}
	return fabs(sqrt(norma));
}

int main()
{
    struct EDO edo;
    struct EDP edp;
    struct tridiagonal *td;
    struct pentadiagonal *pd;
    int n, m;
    double *yEDO, **yEDP, *r, norma;
    scanf("%d %d", &n, &m);

    yEDO = calloc(n, sizeof(double) *n);
    yEDP = malloc(sizeof(double*)*n);
    for(int i = 0; i < n; i++){
        yEDP[i] = calloc(m, sizeof(double)*m);
    }

    // problema A $ y^{ii} = 6x - 0.5x^2, x \in (0,12), y(0) = 0 e y(12) = 0 $
    // y^{ii} + 0*y^{i} + 0*y{i}  = 6x - 0.5x^2, p = 0, q = 0, r = 6x-0.5x^2
    printf("---- problema a ----\n");
    edo.n = n;
    edo.a = 0;
    edo.b = 12;
    edo.ya = (&nulo);
    edo.yb = (&nulo);
    edo.p = (&nulo);
    edo.q = (&nulo);
    edo.r = (&quadratico);
    td = geraTridiagonal(&edo);
    imprimeTridiagonal(td);
    gaussSeidelEDO(td, yEDO, n);
    printf("Y = ");
    for(int i = 0; i < n; i++){
        printf("%f ", yEDO[i]);
    }
    printf("\n");
    r = residuoTri(td, yEDO);
    norma = normaL2Residuo(n, r);
    printf("norma L2 = %f\n", norma);
    free(td);
    free(r);

    // problema B $ T_{xx} + T_{yy} - T = sin^2(x), (x,y) \in \Omega = (0,L) \times (0,W)
    //                                            , T(0,y) = 20, T(L,y) = 45, T(x,0) = 0, T(x,W) = 100$
    printf("---- problema b ----\n");
    edp.n = n;
    edp.m = m;
    edp.ax = 0;
    edp.bx = L;
    edp.ay = 0;
    edp.by = W;
    edp.yax = (&const20);
    edp.ybx = (&const45);
    edp.yay = (&nulo);
    edp.yby = (&const100);
    edp.c = (&idem);
    edp.d = (&idem);
    edp.e = (&oposto);
    edp.r = (&senoQuadrado);
    pd = geraPentadiagonal(&edp);
    imprimePentadiagonal(pd);
    gaussSeidelEDP(pd, yEDP, n, m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%+f ", yEDP[i][j]);
        }
    }
    printf("\n");
    r = residuoPenta(pd, yEDP);
    norma = normaL2Residuo(n*m, r);
    printf("norma L2 = %f\n", norma);
    printf("\n");

    // problema C $ y^{ii} + y = 0, x \in (0,1), y(0) = 0 e y(1) = 1 $
    printf("---- problema c ----\n");
    edo.a = 0;
    edo.b = 1;
    edo.ya = (&nulo);
    edo.yb = (&idem);
    edo.p = (&nulo);
    edo.q = (&idem);
    edo.r = (&nulo);
    td = geraTridiagonal(&edo);
    imprimeTridiagonal(td);
    gaussSeidelEDO(td, yEDO, n);
    for(int i = 0; i < n; i++){
        printf("%f ", yEDO[i]);
    }
    printf("\n");
    r = residuoTri(td, yEDO);
    norma = normaL2Residuo(n, r);
    printf("%f\n", norma);
    free(td);


    // problema D $ u_{xx} + u_{yy} = -cos(x-y), (x,y) \in (0,\pi) \crossproduct (0,\frac{\pi,2})
    //                                            , u(0,y) = cos(y), u(\pi,y) = -cos(y), u(x,0) = cos(x), u(x,\frac{\pi,2}) = 0$
    printf("---- problema d ----\n");
    edp.n = n;
    edp.m = m;
    edp.ax = 0;
    edp.bx = PI;
    edp.ay = 0;
    edp.by = PI_2;
    edp.yax = (&cosy);
    edp.ybx = (&negCosy);
    edp.yay = (&cosx);
    edp.yby = (&nulo);
    edp.c = (&idem);
    edp.d = (&idem);
    edp.e = (&nulo);
    edp.r = (&cosDiff);
    pd = geraPentadiagonal(&edp);
    imprimePentadiagonal(pd);
    gaussSeidelEDP(pd, yEDP, n, m);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%+f ", yEDP[i][j]);
        }
    }
    printf("\n");
    r = residuoPenta(pd, yEDP);
    norma = normaL2Residuo(n*m, r);
    printf("norma L2 = %f\n", norma);
    printf("\n");
}
