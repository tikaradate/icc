#include "edp.h"
#include "utils.h"

void gaussSeidelEDP(struct EDP *edp, double **y)
{
    int n = edp->n, m = edp->m, it = 0;
    double hx, hx2, hy, hy2, xi, yi, di2, di, d, ds, ds2, bi;

    hx = (edp->bx - edp->ax) / (n + 1);
    hx2 = hx*hx;
    hy = (edp->bx - edp->ax) / (m + 1);
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

                di2 = hx2 * edp->d(xi, yi);
                di  = hy2 * edp->c(xi, yi);
                d   = -2*(hx2*edp->d(xi, yi) + hy2*edp->c(xi, yi) + hx2*hy2*edp->e(xi, yi));
                ds  = hy2 * edp->c(xi, yi);
                ds2 = hx2 * edp->d(xi, yi);
                // canto inferior esquerdo
                if (i == 0 && j == 0){
                    bi -= (hx2*edp->yay + hy2*edp->yax + ds*y[i+1][j] + ds2*y[i][j+1]);
                // lateral esquerda
                } else if (i == 0) { 
                    bi -= (di2*y[i][j-1] + hy2*edp->yax + ds*y[i+1][j] + ds2*y[i][j+1]);
                // parte inferior
                } else if (j == 0){
                    bi -= (hx2*edp->yay + di*y[i-1][j] + ds*y[i+1][j] + ds2*y[i][j+1]);
                // canto superior direito
                } else if (i == n-1 && j == m-1){
                    bi -= (di2*y[i][j-1] + di*y[i-1][j] + hy2*edp->ybx + hx2*edp->yby);
                // lateral direita
                } else if (i == n-1) {
                    bi -= (di2*y[i][j-1] + di*y[i-1][j] + hy2*edp->ybx + ds2*y[i][j+1]);
                // parte superior
                } else if (j == m-1) {
                    bi -= (di2*y[i][j-1] + di*y[i-1][j] + ds*y[i+1][j] + hx2*edp->yby);
                // no meio da malha
                } else {
                    bi -= (di2*y[i][j-1] + di*y[i-1][j] + ds*y[i+1][j] + ds2*y[i][j+1]);
                }
                y[i][j] = bi / d;
            }
        }
    }
}
