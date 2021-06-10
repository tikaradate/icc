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

// problema A
double quadratico(double x)
{
    return (6.0 * x - ((x * x) / 2.0));
}
// problema A, B, C, D
double nulo()
{
    return 0.0;
}

// problema A, B, C, D
double idem()
{
    return 1.0;
}
// problema B
double oposto()
{
    return -1.0;
}
// problema B
double senoQuadrado(double x, double y)
{
    return (sin(x) * sin(x));
}
// problema D
double cosDiff(double x, double y)
{
    return (-cos(x+y)-cos(x-y));
}