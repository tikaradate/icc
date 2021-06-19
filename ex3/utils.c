#include <math.h>
#include <stdio.h>

#include "utils.h"

/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

double timestamp(void)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)(tp.tv_sec * 1000.0 + tp.tv_usec / 1000.0));
}

void zeraVetor(double *y, int n)
{
    for (int i = 0; i < n; i++)
    {
        y[i] = 0.0;
    }
}

void zeraMatriz(double **y, int n, int m)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            y[i][j] = 0.0;
}

// problema a b c d
double const0()
{
    return 0.0;
}
// problema a b c d
double const1()
{
    return 1.0;
}
// problema b
double constNeg1()
{
    return -1.0;
}
// problema b
double const20()
{
    return 20.0;
}
// problema b
double const45()
{
    return 45.0;
}
// problema b
double const100()
{
    return 100.0;
}
// problema d
double negCosx(double x, double y)
{
    return (-cos(y));
}
// problema d
double negCosy(double x, double y)
{
    return (-cos(y));
}
// problema d
double cosx(double x, double y)
{
    return (cos(x));
}
// problema d
double cosy(double x, double y)
{
    return (cos(y));
}

// problema a
double quadratico(double x)
{
    return (6.0 * x - ((x * x) / 2.0));
}
// problema b
double senoQuadrado(double x, double y)
{
    return (sin(x) * sin(x));
}
// problema d
double cosDiff(double x, double y)
{
    return (-cos(x + y) - cos(x - y));
}