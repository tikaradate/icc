#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "diagonal.h"
#include "utils.h"

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
        td->di[i] = 1 - (h * edo->p(xi) / 2.0);
        td->d[i] = -2 + h * h * edo->q(xi);
        td->ds[i] = 1 + h * edo->p(xi) / 2.0;
        // se for extremidade, é subtraido de B
        if (i == 0)
            td->b[i] -= edo->ya(xi) * td->di[i];
        if (i == n - 1)
            td->b[i] -= edo->yb(xi) * td->ds[i];
    }
    return td;
}

void gaussSeidelEDO(struct tridiagonal *td, double *y, int n)
{
    int it = 0;
    double soma;

    while (it++ < MAXIT)
    {
        for (int i = 0; i < n; i++)
        {
            soma = 0;
            if (i != 0)
                soma += td->di[i] * y[i - 1];
            if (i != n - 1)
                soma += td->ds[i] * y[i + 1];

            y[i] = (td->b[i] - soma) / td->d[i];
        }
    }
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
    printf("inferior | principal | superior | independente\n");
    for (int i = 0; i < td->n; i++)
    {
        if (i == 0)
        {
            printf("%10c %+.7f %+.7f ", ' ', td->d[i], td->ds[i]);
        }
        else if (i == td->n - 1)
        {
            printf("%+.7f %+.7f %10c ", td->di[i], td->d[i], ' ');
        }
        else
        {
            printf("%+.7f %+.7f %+.7f ", td->di[i], td->d[i], td->ds[i]);
        }
        printf("= %+.7f\n", td->b[i]);
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
            pd->d[index] = -2.0 * ((hx2 * edp->d(xi, yi)) + (hy2 * edp->c(xi, yi))) + (hx2 * hy2 * edp->e(xi, yi));
            pd->ds[index] = hy2 * edp->c(xi, yi);
            pd->ds2[index] = hx2 * edp->d(xi, yi);

            // se for extremidade, é subtraído de B
            if (j == 0)
                pd->b[index] -= edp->yay(xi, yi) * pd->di2[index];
            if (i == 0)
                pd->b[index] -= edp->yax(xi, yi) * pd->di[index];
            if (i == n - 1)
                pd->b[index] -= edp->ybx(xi, yi) * pd->ds[index];
            if (j == m - 1)
                pd->b[index] -= edp->yby(xi, yi) * pd->ds2[index];
        }
    }
    return pd;
}

void gaussSeidelEDP(struct pentadiagonal *pd, double **y, int n, int m)
{
    int it = 0, index;
    double soma;

    while (it++ < MAXIT)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                soma = 0;
                index = (i * m + j);
                // checa parte inferior
                if (j != 0)
                    soma += pd->di2[index] * y[i][j - 1];
                // checa borda esquerda
                if (i != 0)
                    soma += pd->di[index] * y[i - 1][j];
                // checa borda direita
                if (i != n - 1)
                    soma += pd->ds[index] * y[i + 1][j];
                // checa parte superior
                if (j != m - 1)
                    soma += pd->ds2[index] * y[i][j + 1];

                y[i][j] = (pd->b[index] - soma) / pd->d[index];
            }
        }
    }
}

double *residuoPenta(struct pentadiagonal *pd, double **y)
{
    double *r;
    int n = pd->n, m = pd->m, index;

    r = calloc(n * m, sizeof(double) * n * m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            index = i * m + j;

            r[index] += (pd->d[index] * y[i][j]);
            if (j != 0)
                r[index] += (pd->di2[index] * y[i][j - 1]);
            if (i != 0)
                r[index] += (pd->di[index] * y[i - 1][j]);
            if (i != n - 1)
                r[index] += (pd->ds[index] * y[i + 1][j]);
            if (j != m - 1)
                r[index] += (pd->ds2[index] * y[i][j + 1]);

            r[index] = pd->b[index] - r[index];
        }
    }
    return r;
}

void imprimePentadiagonal(struct pentadiagonal *pd)
{
    int index, n = pd->n, m = pd->m;

    printf("inferior2 | inferior | principal | superior | superior2 | independente\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            index = i * m + j;

            if (j != 0)
            {
                printf("%+.7f ", pd->di2[index]);
            }
            else
            {
                printf("%10c ", ' ');
            }
            if (i != 0)
            {
                printf("%+.7f ", pd->di[index]);
            }
            else
            {
                printf("%10c ", ' ');
            }

            printf("%+.7f ", pd->d[index]);

            if (i != n - 1)
            {
                printf("%+.7f ", pd->ds[index]);
            }
            else
            {
                printf("%10c ", ' ');
            }
            if (j != m - 1)
            {
                printf("%+.7f ", pd->ds2[index]);
            }
            else
            {
                printf("%10c ", ' ');
            }

            printf(" = %+.7f\n", pd->b[index]);
        }
    }
}

double normaL2Residuo(int n, double *res)
{
    double norma = 0;
    for (int i = 0; i < n; i++)
    {
        norma += (res[i] * res[i]);
    }
    return fabs(sqrt(norma));
}

void imprimeSolucaoEDO(double *y, int n)
{
    printf("Y = ");
    for (int i = 0; i < n; i++)
    {
        printf("%f ", y[i]);
    }
    printf("\n");
}

void imprimeSolucaoEDP(double **y, int n, int m)
{
    printf("Y = ");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            printf("%f ", y[i][j]);
    printf("\n");
}