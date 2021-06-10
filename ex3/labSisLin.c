#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"
#include "SistemasLineares.h"

int main()
{
	int it, erro, metodo, sist = 1;
	SistLinear_t *SL, *gauss;
	real_t *x, *res, norma;
	double time;

	while (SL = lerSistLinear())
	{
		printf("\n************** Sistema %d **************\n", sist);
		// a eliminacao de Gauss altera o sistema, portanto precisamos copiar
		gauss = alocaSistLinear(SL->n);
		copiaSL(SL, gauss);
		// vetor de respostas usado em todos métodos
		x = calloc(SL->n, sizeof(real_t) * SL->n);

		erro = eliminacaoGauss(gauss, x, &time);

		printf("\n----> Eliminacao de Gauss - %1.8fms\n", time);
		if (erro < 0)
		{
			fprintf(stderr, "Sistema %d\nEliminação de Gauss: ", sist);
			trataErro(erro);
		}
		prnVetor(x, SL->n);

		res = residuo(SL, x);
		printf("Norma %1.8e\n", normaL2Residuo(SL, x, res));
		free(res);

		zeraVetor(x, SL->n);

		it = gaussJacobi(SL, x, &time);
		printf("\n----> Gauss Jacobi - %1.8fms - %d iteracoes\n", time, it);
		if (it < 0)
		{
			fprintf(stderr, "Sistema %d\nGauss-Jacobi: ", sist);
			trataErro(it);
		}
		prnVetor(x, SL->n);
		res = residuo(SL, x);
		norma = normaL2Residuo(SL, x, res);
		printf("Norma %1.8e\n", norma);
		free(res);

		if (norma > 5.0)
		{
			it = refinamento(SL, x, &time);
			printf("\n<> Refinamento - %1.8fms - %d iteracoes\n", time, it);
			prnVetor(x, SL->n);
			res = residuo(SL, x);
			norma = normaL2Residuo(SL, x, res);
			free(res);
			printf("Norma %1.8e\n", norma);
		}

		zeraVetor(x, SL->n);

		it = gaussSeidel(SL, x, &time);
		printf("\n----> Gauss-Seidel - %1.8fms - %d iteracoes\n", time, it);
		if (it < 0)
		{
			fprintf(stderr, "Sistema %d\nGauss-Seidel: ", sist);
			trataErro(it);
		}
		prnVetor(x, SL->n);
		res = residuo(SL, x);
		norma = normaL2Residuo(SL, x, res);
		printf("Norma %1.8e\n", norma);
		free(res);

		if (norma > 5.0)
		{
			it = refinamento(SL, x, &time);
			printf("\n<> Refinamento - %1.8fms - %d iteracoes\n", time, it);
			prnVetor(x, SL->n);
			res = residuo(SL, x);
			norma = normaL2Residuo(SL, x, res);
			free(res);
			printf("Norma %1.8e\n", norma);
		}
		sist++;
		liberaSistLinear(SL);
		liberaSistLinear(gauss);
		free(x);
	}
	return 0;
}