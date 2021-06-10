#include <stdio.h>
#include <stdlib.h>
#include "edo.h"
#include "edp.h"
#include "diagonal.h"

struct tridiagonal *geraTridiagonal(struct EDO *edo)
{
    struct tridiagonal *td;
    int n = edo->n;
    double h, xi;

    td = malloc(sizeof(struct tridiagonal));
    td->di = malloc(sizeof(double) * n);
    td->d = malloc(sizeof(double) * n);
    td->ds = malloc(sizeof(double) * n);
    td->b = malloc(sizeof(double) * n);
    td->n = n;

    h = (edo->b - edo->a) / (n + 1);
    for (int i = 0; i < n; i++)
    {
        xi = edo->a + (i + 1) * h;
        td->b[i] = h * h * edo->r(xi);
        td->di[i] = 1 - h * edo->p(xi) / 2.0;
        td->d[i] = -2 + h * h * edo->q(xi);
        td->ds[i] = 1 + h * edo->p(xi) / 2.0;
    }
    return td;
}

void imprimeTridiagonal(struct tridiagonal *td)
{
    for (int i = 0; i < td->n; i++)
    {
        if (i == 0)
        {
            printf("%f  %f ", td->d[i], td->ds[i]);
        }
        else if (i == td->n - 1)
        {
            printf("%f  %f ", td->di[i], td->ds[i]);
        }
        else
        {
            printf("%f  %f  %f ", td->di[i], td->d[i], td->ds[i]);
        }
        printf("%f\n", td->b[i]);
    }
}

struct pentadiagonal *geraPentadiagonal(struct EDP *edp)
{
    struct pentadiagonal *pd;
    int n = edp->n, m = edp->m, index;
    double hx, hx2, hy, hy2, xi, yi;

    pd = malloc(sizeof(struct pentadiagonal));
    pd->di2 = malloc(sizeof(double) * n * m);
    pd->di = malloc(sizeof(double) * n * m);
    pd->d = malloc(sizeof(double) * n * m);
    pd->ds = malloc(sizeof(double) * n * m);
    pd->ds2 = malloc(sizeof(double) * n * m);
    pd->b = malloc(sizeof(double) * n * m);
    pd->n = n;
    pd->m = m;

    hx = (edp->bx - edp->ax) / (n + 1);
    hx2 = hx * hx;
    hy = (edp->bx - edp->ax) / (m + 1);
    hy2 = hy * hy;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            index = i * m + j;
            xi = edp->ax + (i + 1) * hx;
            yi = edp->ay + (j + 1) * hy;
            pd->b[index] = hx2 * hy2 * edp->r(xi, yi);

            pd->di2[index] = hx2 * edp->d(xi, yi);
            pd->di[index] = hy2 * edp->c(xi, yi);
            pd->d[index] = -2.0 * (hx2 * edp->d(xi, yi) + hy2 * edp->c(xi, yi) + (hx2 * hy2 * edp->e(xi, yi)));
            pd->ds[index] = hy2 * edp->c(xi, yi);
            pd->ds2[index] = hx2 * edp->d(xi, yi);
        }
    }
    return pd;
}

void imprimePentadiagonal(struct pentadiagonal *pd)
{
    int index;
    for (int i = 0; i < pd->n; i++)
    {
        for(int j = 0; j < pd->m; j++){
            index = i*pd->m + j;
        if (i == 0 && j == 0)
        {
            printf("%f  %f  %f ", pd->d[index], pd->ds[index], pd->ds2[index]);
        }
        else if (i == 0)
        {
            printf("%f %f  %f  %f ", pd->di2[index], pd->d[index], pd->ds[index], pd->ds2[index]);
        }
        else if (j == 0)
        {
            printf("%f  %f  %f  %f ", pd->di[index], pd->d[index], pd->ds[index], pd->ds2[index]);
        }
        else if (i == pd->n - 1 && j == pd->m - 1)
        {
            printf("%f  %f  %f ", pd->di2[index], pd->di[index], pd->d[index]);
        }
        else if (i == pd->n - 1)
        {
            printf("%f  %f  %f  %f ", pd->di2[index], pd->di[index], pd->d[index], pd->ds2[index]);
        }
        else if (j == pd->m - 1)
        {
            printf("%f  %f  %f  %f ", pd->di2[index], pd->di[index], pd->d[index], pd->ds[index]);
        }
        else
        {
            printf("%f  %f  %f  %f  %f ", pd->di2[index], pd->di[index], pd->d[index], pd->ds[index], pd->ds2[index]);
        }
        printf("%f\n", pd->b[index]);
        }
    }
}