#ifndef __EDO_EDP__
#define __EDO_EDP__
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

#endif