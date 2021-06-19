#ifndef __DIAGONAL__
#define __DIAGONAL__

struct tridiagonal
{
    int n;                   // ordem do SL
    double *di, *d, *ds, *b; // as diagonais e os termos independentes
};

struct pentadiagonal
{
    int n, m;                            // ordem do SL
    double *di2, *di, *d, *ds, *ds2, *b; // as diagonais e os termos independentes
};

// EDO de segunda ordem
// y'' + y'p + yq = r
struct EDO
{
    int n;
    double a, b;
    double (*ya)(double), (*yb)(double);
    double (*p)(double), (*q)(double), (*r)(double);
};

// EDP de formato c*Uxx + d*Uyy + e*U = r
struct EDP
{
    int n, m;
    double ax, bx, ay, by;
    // valores dos extremos
    double (*yax)(double, double), (*ybx)(double, double),
        (*yay)(double, double), (*yby)(double, double);
    // funções das variáveis independentes
    double (*c)(double, double), (*d)(double, double),
        (*e)(double, double), (*r)(double, double);
};

void gaussSeidelEDO(struct tridiagonal *td, double *y, int n);

struct tridiagonal *geraTridiagonal(struct EDO *edo);

void imprimeTridiagonal(struct tridiagonal *td);

double *residuoTri(struct tridiagonal *td, double *y);

void gaussSeidelEDP(struct pentadiagonal *td, double **y, int n, int m);

struct pentadiagonal *geraPentadiagonal(struct EDP *edp);

double *residuoPenta(struct pentadiagonal *pd, double **y);

void imprimePentadiagonal(struct pentadiagonal *pd);

double normaL2Residuo(int n, double *res);

void imprimeSolucaoEDO(double *y, int n);

void imprimeSolucaoEDP(double **y, int n, int m);
#endif