#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "diagonal.h"
#include "utils.h"

#define L 6
#define W 8
#define PI 3.14159265358979323846
#define PI_2 1.57079632679489661923

int main()
{
    struct EDO edo;
    struct EDP edp;
    struct tridiagonal *td;
    struct pentadiagonal *pd;
    int n, m;
    double *yEDO, **yEDP, *r, norma, tempo;

    scanf("%d %d", &n, &m);

    yEDO = calloc(n, sizeof(double) * n);
    yEDP = malloc(sizeof(double *) * n);
    for (int i = 0; i < n; i++)
    {
        yEDP[i] = calloc(m, sizeof(double) * m);
    }

    // problema A
    printf("---- problema a ----\n");
    edo.n = n;
    edo.a = 0;
    edo.b = 12;
    edo.ya = (&const0);
    edo.yb = (&const0);
    edo.p = (&const0);
    edo.q = (&const0);
    edo.r = (&quadratico);

    tempo = timestamp();
    td = geraTridiagonal(&edo);
    gaussSeidelEDO(td, yEDO, n);
    r = residuoTri(td, yEDO);
    norma = normaL2Residuo(n, r);
    tempo = timestamp() - tempo;

    imprimeTridiagonal(td);
    imprimeSolucaoEDO(yEDO, n);
    printf("norma L2 = %.6e, tempo = %.6f\n", norma, tempo);
    zeraVetor(yEDO, n);
    free(td);
    free(r);

    // problema B
    printf("---- problema b ----\n");
    edp.n = n;
    edp.m = m;
    edp.ax = 0;
    edp.bx = L;
    edp.ay = 0;
    edp.by = W;
    edp.yax = (&const20);
    edp.ybx = (&const45);
    edp.yay = (&const0);
    edp.yby = (&const100);
    edp.c = (&const1);
    edp.d = (&const1);
    edp.e = (&constNeg1);
    edp.r = (&senoQuadrado);

    tempo = timestamp();
    pd = geraPentadiagonal(&edp);
    gaussSeidelEDP(pd, yEDP, n, m);
    r = residuoPenta(pd, yEDP);
    norma = normaL2Residuo(n * m, r);
    tempo = timestamp() - tempo;

    imprimePentadiagonal(pd);
    imprimeSolucaoEDP(yEDP, n, m);
    printf("norma L2 = %.6e, tempo = %.6f\n", norma, tempo);
    free(r);

    // problema C
    printf("---- problema c ----\n");
    edo.n = n;
    edo.a = 0.0;
    edo.b = 1.0;
    edo.ya = (&const0);
    edo.yb = (&const1);
    edo.p = (&const0);
    edo.q = (&const1);
    edo.r = (&const0);

    tempo = timestamp();
    td = geraTridiagonal(&edo);
    gaussSeidelEDO(td, yEDO, n);
    r = residuoTri(td, yEDO);
    norma = normaL2Residuo(n, r);
    tempo = timestamp() - tempo;

    imprimeTridiagonal(td);
    imprimeSolucaoEDO(yEDO, n);
    printf("norma L2 = %.6e, tempo = %.6f\n", norma, tempo);
    free(td);
    free(r);

    // problema D
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
    edp.yby = (&const0);
    edp.c = (&const1);
    edp.d = (&const1);
    edp.e = (&const0);
    edp.r = (&cosDiff);

    tempo = timestamp();
    pd = geraPentadiagonal(&edp);
    gaussSeidelEDP(pd, yEDP, n, m);
    r = residuoPenta(pd, yEDP);
    norma = normaL2Residuo(n * m, r);
    tempo = timestamp() - tempo;

    imprimePentadiagonal(pd);
    imprimeSolucaoEDP(yEDP, n, m);
    printf("norma L2 = %.6e, tempo = %.6f\n", norma, tempo);
    printf("\n");
    free(r);
}
