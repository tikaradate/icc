#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <sys/time.h>

#define MAXIT 50

/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

double timestamp(void);

void zeraVetor(double *y, int n);

void zeraMatriz(double **y, int n, int m);

// funções e constantes usadas nas equações
double quadratico(double x);

double const0();

double const1();

double constNeg1();

double const20();

double const45();

double const100();

double senoQuadrado(double x, double y);

double negCosx(double x, double y);

double negCosy(double x, double y);

double cosx(double x, double y);

double cosy(double x, double y);

double cosDiff(double x, double y);

#endif // __UTILS_H__
