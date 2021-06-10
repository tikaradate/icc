#ifndef __DIAGONAL__
#define __DIAGONAL__
struct tridiagonal
{
    int n;                    // ordem do SL
    double *di, *d, *ds, *b;  // as diagonais e os termos independentes
};

struct pentadiagonal
{
    int n, m;                               // ordem do SL
    double *di2, *di, *d, *ds, *ds2, *b; // as diagonais e os termos independentes
};

struct tridiagonal *geraTridiagonal(struct EDO *edo);

void imprimeTridiagonal(struct tridiagonal *td);

struct pentadiagonal *geraPentadiagonal(struct EDP *edp);

void imprimePentadiagonal(struct pentadiagonal *pd);
#endif