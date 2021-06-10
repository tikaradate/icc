#ifndef __EDO__
#define __EDO__

// EDO de segunda ordem
// y'' + y'p + yq = r
struct EDO
{
    int n;
    double a, b;
    double ya, yb;
    double (*p)(double), (*q)(double), (*r)(double);
};

void gaussSeidelEDO(struct EDO *edo, double *y);

#endif