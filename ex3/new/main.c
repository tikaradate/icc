#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct edp{
    int n, m;
    double ax, ay, bx, by;
    double yax, ybx, yay, yby;
    double (*c)(double, double), (*d)(double, double), 
           (*e)(double, double), (*r)(double, double);
};

struct pd{
    int n, m;
    double *di2, *di, *d, *ds, *ds2, *b;
};

double const1(){
    return 1.0;
}

double constNeg1(){
    return -1.0;
}

double senoQuad(double x, double y){
    return (sin(x)* sin(x));
}

struct pentadiagonal *geraPentadiagonal(struct edp *edp)
{
    struct pd *pd;
    int n = edp->n, m = edp->m;
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {

        }
    }
    return pd;
}

int main(){
    int n, m;
    struct edp edp;
    struct pd *pd;
    double **y;

    scanf("%d %d", &n, &m);
    edp.n = n;
    edp.m = m;
    edp.ax = 0;
    edp.bx = 6;
    edp.yax = 20;
    edp.ybx = 45;
    edp.ay = 0;
    edp.bx = 8;
    edp.yay = 0;
    edp.yby = 100;
    edp.c = (&const1);
    edp.d = (&const1);
    edp.e = (&constNeg1);
    edp.r = (&senoQuad);
    y = malloc(sizeof(double)*n);
    for(int i = 0; i < n; i++){
        y[i] = malloc(sizeof(double)*m);
    }

}