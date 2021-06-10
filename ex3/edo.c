#include "edo.h"
#include "utils.h"

void gaussSeidelEDO(struct EDO *edo, double *y)
{
    int n = edo->n, it = 0;
    double h, xi, bi, yi, d, di, ds;

    h = (edo->b - edo->a) / (n + 1);
    while (it++ < MAXIT)
    {
        for (int i = 0; i < n; i++)
        {
            xi = edo->a + (i + 1) * h;
            bi = h * h * edo->r(xi);
            di = 1 - h * edo->p(xi) / 2.0;
            d = -2 + h * h * edo->q(xi);
            ds = 1 + h * edo->p(xi) / 2.0;
            if (i == 0)
                bi -= ds * y[i + 1] + edo->ya * (1 - h * edo->p(edo->a + h) / 2.0);
            else if (i == n - 1)
                bi -= di * y[i - 1] + edo->yb * (1 + h * edo->p(edo->b - h) / 2.0);
            else
                bi -= ds * y[i + 1] + di * y[i - 1];

            y[i] = bi / d;
        }
    }
}