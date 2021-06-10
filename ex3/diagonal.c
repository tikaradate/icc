#include <stdio.h>
#include <stdlib.h>
#include "diagonal.h"
#include "utils.h"

void gaussSeidelEDO(struct EDO *edo, double *y, struct tridiagonal *td)
{
    int n = edo->n, it = 0;
    double h, xi, bi, yi, d, di, ds, soma;

    h = (edo->b - edo->a) / (n + 1);
    while (it++ < MAXIT)
    {
        for (int i = 0; i < n; i++)
        {
            soma = 0;
            xi = edo->a + (i + 1) * h;
            // bi = h * h * edo->r(xi);
            
            // di = 1 - (h * edo->p(xi) / 2.0);
            // d = -2 + h * h * edo->q(xi);
            // ds = 1 + (h * edo->p(xi)/ 2.0);
            
            // limitante inferior
            // if (i == 0)
            //     di *= edo->ya;
            // else
            //     di *= y[i - 1];

            // limitante superior
            // if (i == n-1)
            //     ds *= edo->yb;
            // else
            //     ds *= y[i + 1];
                
            if(i == 0)
                soma += td->ds[i]*y[i+1];
            else if(i == n-1)
                soma += td->di[i]*y[i-1];
            else
                soma += td->di[i]*y[i-1] + td->ds[i]*y[i+1];

            y[i] = (td->b[i] - soma) / td->d[i];
        }
    }
}


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
        if(i == 0)
            td->b[i] -= edo->ya*td->di[i];
        if(i == n-1)
            td->b[i] -= edo->yb*td->ds[i];
    }
    return td;
}

double *residuoTri(struct tridiagonal *td, double *y)
{
    double *r;
    int n = td->n;

    r = calloc(n, sizeof(double) * n);
    for (int i = 0; i < n; i++)
    {
        r[i] += (td->d[i] * y[i]);

        if (i != 0)
            r[i] += (td->di[i] * y[i - 1]);
        if (i != n - 1)
            r[i] += (td->ds[i] * y[i + 1]);

        r[i] = td->b[i] - r[i];
    }
    return r;
}

void imprimeTridiagonal(struct tridiagonal *td)
{
    for (int i = 0; i < td->n; i++)
    {
        if (i == 0)
        {
            printf("%9c %+f %+f =", ' ', td->d[i], td->ds[i]);
        }
        else if (i == td->n - 1)
        {
            printf("%+f %+f %9c =", td->di[i], td->d[i], ' ');
        }
        else
        {
            printf("%+f %+f %+f =", td->di[i], td->d[i], td->ds[i]);
        }
        printf(" %+f\n", td->b[i]);
    }
}

void gaussSeidelEDP(struct EDP *edp, double **y)
{
    int n = edp->n, m = edp->m, it = 0;
    double hx, hx2, hy, hy2, xi, yi, di2, di, d, ds, ds2, bi;

    hx = (edp->bx - edp->ax) / (n + 1);
    hx2 = hx*hx;
    hy = (edp->by - edp->ay) / (m + 1);
    hy2 = hy*hy;
    while (it++ < MAXIT)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                xi = edp->ax + (i + 1) * hx;
                yi = edp->ay + (j + 1) * hy;
                bi = hx2*hy2 * edp->r(xi, yi);

                di2 = hx2*edp->d(xi, yi);
                di  = hy2*edp->c(xi, yi);
                d = (-2*(hx2*edp->d(xi, yi) + hy2*edp->c(xi, yi)) + hx2*hy2*edp->e(xi, yi));
                ds = hy2*edp->c(xi, yi);
                ds2 = hx2*edp->d(xi, yi);
                
                // checa parte inferior
                if(j == 0){
                    di2 *= edp->yay;
                } else {
                    di2 *= y[i][j-1];
                }
                // checa borda esquerda
                if(i == 0){
                    di *= edp->yax;
                } else {
                    di *= y[i-1][j];
                }
                // checa borda direita
                if(i == n-1){
                    ds *= edp->ybx;
                } else {
                    ds *= y[i+1][j];
                }
                // checa parte superior
                if(j == m-1){
                    ds2 *= edp->yby;
                } else {
                    ds2 *= y[i][j+1];
                }

                bi -= (di2 + di + ds + ds2);
                y[i][j] = bi / d;
            }
        }
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
    hy = (edp->by - edp->ay) / (m + 1);
    hy2 = hy * hy;
    printf("\n\n%f %f\n\n", hx, hy);
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
            pd->d[index] = (-2.0 * (hx2 * edp->d(xi, yi) + hy2 * edp->c(xi, yi))) + ((edp->e(xi, yi)));
            pd->ds[index] = hy2 * edp->c(xi, yi);
            pd->ds2[index] = hx2 * edp->d(xi, yi);

            if(i == 0)
                pd->b[index] -= edp->yax;
            if(j == 0)
                pd->b[index] -= edp->yay;
            if(i == n-1)
                pd->b[index] -= edp->ybx;
            if(j == m-1)
                pd->b[index] -= edp->yby;
        }
    }
    return pd;
}

double *residuoPenta(struct pentadiagonal *pd, double **y)
{
    double *r;
    int n = pd->n, m = pd->m, index;

    r = calloc(n, sizeof(double) * n * m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            index = i * m + j;
            
            r[index] += (pd->d[index] * y[i][j]);
            if(i != 0)
                r[index] += (pd->di[index] * y[i-1][j]);
            if(j != 0)
                r[index] += (pd->di2[index] * y[i][j-1]);
            if(i != n-1)
                r[index] += (pd->ds[index] * y[i+1][j]);
            if(j != m-1)
                r[index] += (pd->ds2[index] * y[i][j+1]);

            r[index] = pd->b[index] - r[index];
        }
    }
    return r;
}

void imprimePentadiagonal(struct pentadiagonal *pd)
{
    int index;
    for (int i = 0; i < pd->n; i++)
    {
        for (int j = 0; j < pd->m; j++)
        {
            index = i * pd->m + j;
            if (i == 0 && j == 0)
            {
                printf("%9c %9c %+f %+f %+f =", ' ', ' ', pd->d[index], pd->ds[index], pd->ds2[index]);
            }
            else if (i == 0)
            {
                printf("%+f %9c %+f %+f %+f =", pd->di2[index], ' ', pd->d[index], pd->ds[index], pd->ds2[index]);
            }
            else if (j == 0)
            {
                printf("%9c %+f %+f %+f %+f =", ' ', pd->di[index], pd->d[index], pd->ds[index], pd->ds2[index]);
            }
            else if (i == pd->n - 1 && j == pd->m - 1)
            {
                printf("%+f %+f %+f %9c %9c =", pd->di2[index], pd->di[index], pd->d[index], ' ', ' ');
            }
            else if (i == pd->n - 1)
            {
                printf("%+f %+f %+f %9c %+f =", pd->di2[index], pd->di[index], pd->d[index], ' ', pd->ds2[index]);
            }
            else if (j == pd->m - 1)
            {
                printf("%+f %+f %+f %+f %9c =", pd->di2[index], pd->di[index], pd->d[index], pd->ds[index], ' ');
            }
            else
            {
                printf("%+f %+f %+f %+f %+f =", pd->di2[index], pd->di[index], pd->d[index], pd->ds[index], pd->ds2[index]);
            }
            printf(" %+f\n", pd->b[index]);
        }
    }
}
