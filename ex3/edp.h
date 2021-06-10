#ifndef __EDP__
#define __EDP__

// EDP de formato Laplaciano (c*Uxx + d*Uyy + e*U = r)
struct EDP
{
    int n, m;
    double ax, bx, ay, by;
    double yax, ybx, yay, yby;
    double (*c)(double, double), (*d)(double, double), 
           (*e)(double, double), (*r)(double, double);
};

void gaussSeidelEDP(struct EDP *edp, double **y);
#endif