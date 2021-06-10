#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"
#include "SistemasLineares.h"

/*!
  \brief Essa função calcula a norma L2 do resíduo de um sistema linear 

  \param SL Ponteiro para o sistema linear
  \param x Solução do sistema linear
  \param res Valor do resíduo

  \return Norma L2 do resíduo.
*/
real_t normaL2Residuo(SistLinear_t *SL, real_t *x, real_t *res)
{
	real_t norma = 0;
	for (int i = 0; i < SL->n; i++)
	{
		norma += (res[i] * res[i]);
	}
	return realAbs(sqrt(norma));
}

/*!
  \brief Método da Eliminação de Gauss

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução
  \param tTriangulariza tempo gasto na triangularização
  \param tRetroSubst tempo gasto na retrosubstituição

  \return código de erro. 0 em caso de sucesso.
*/
int eliminacaoGauss(SistLinear_t *SL, real_t *x, double *tTotal)
{
	int retValue = 0;
	*tTotal = timestamp();
	for (int i = 0; i < SL->n; ++i)
	{
		int iPivo = encontraMax(SL, i);
		if (i != iPivo)
		{
			trocaLinha(SL, i, iPivo);
		}

		for (int j = i + 1; j < SL->n; j++)
		{
			double m = SL->A[j][i] / SL->A[i][i];
			if (isinf(m))
				retValue = -3;

			SL->A[j][i] = 0.0;
			for (int k = i + 1; k < SL->n; k++)
				SL->A[j][k] -= SL->A[i][k] * m;

			SL->b[j] -= SL->b[i] * m;
		}
	}
	retrossubs(SL, x);
	*tTotal = timestamp() - *tTotal;
	return retValue;
}

/*!
  \brief Método de Gauss-Jacobi

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução. Ao iniciar função contém
            valor inicial
  \param erro menor erro aproximado para encerrar as iterações
  \param tTotal tempo gasto pelo método
  \param tIteração tempo gasto em cada iteração

  \return código de erro. Um nr positivo indica sucesso e o nr
          de iterações realizadas. Um nr. negativo indica um erro:
          -1 (não converge) -2 (sem solução)
*/
int gaussJacobi(SistLinear_t *SL, real_t *x, double *tTotal)
{
	int it = 0, retValue = 0;
	real_t diff, soma, max;
	real_t *x_it = malloc(sizeof(real_t) * SL->n);

	*tTotal = timestamp();
	// checa se converge
	max = 0;
	for (int i = 0; i < SL->n; i++)
	{
		soma = 0;
		for (int j = 0; j < SL->n; j++)
		{
			if (i != j)
			{
				soma += SL->A[i][j];
			}
		}
		max = realMax(max, soma / SL->A[i][i]);
	}
	if (max > 1)
	{
		retValue = -1;
	}

	while (it < MAXIT)
	{
		for (int i = 0; i < SL->n; i++)
		{
			soma = 0;
			for (int j = 0; j < SL->n; j++)
			{
				if (i != j)
				{
					soma += (SL->A[i][j] * x[j]);
				}
			}
			x_it[i] = (SL->b[i] - soma) / SL->A[i][i];
			if (isinf(x_it[i]) && retValue == 0)
				retValue = -3;
		}
		it++;
		diff = 0;
		for (int i = 0; i < SL->n; i++)
		{
			diff = realMax(diff, realAbs(x[i] - x_it[i]));
		}
		for (int i = 0; i < SL->n; i++)
		{
			x[i] = x_it[i];
		}
		if (diff < SL->erro)
			break;
	}
	*tTotal = timestamp() - *tTotal;
	free(x_it);
	if (retValue == 0)
		return it;
	else
		return retValue;
}

/*!
  \brief Método de Gauss-Seidel

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução. Ao iniciar função contém
            valor inicial
  \param erro menor erro aproximado para encerrar as iterações
  \param tTotal tempo gasto pelo método
  \param tIteração tempo gasto em cada iteração

  \return código de erro. Um nr positivo indica sucesso e o nr
          de iterações realizadas. Um nr. negativo indica um erro:
          -1 (não converge) -2 (sem solução)
  */
int gaussSeidel(SistLinear_t *SL, real_t *x, double *tTotal)
{
	int it = 0, retValue = 0;
	real_t *beta, diff, soma, soma2, prev, max;

	beta = malloc(sizeof(real_t) * SL->n);
	*tTotal = timestamp();
	// checa se converge pelo critério de Sassenfeld
	max = 0;
	for (int i = 0; i < SL->n; i++)
	{
		soma = 0;
		soma2 = 0;
		for (int j = 0; j < i - 1; j++)
		{
			soma += SL->A[i][j] * beta[j];
		}
		for (int j = i + 1; j < SL->n; j++)
		{
			soma2 += SL->A[i][j];
		}
		beta[i] = (soma + soma2) / SL->A[i][i];
		max = realMax(max, beta[i]);
	}
	if (max > 1)
	{
		retValue = -1;
	}

	while (it < MAXIT)
	{
		diff = 0;
		for (int i = 0; i < SL->n; i++)
		{
			soma = 0;
			for (int j = 0; j < SL->n; j++)
			{
				if (i != j)
				{
					soma += (SL->A[i][j] * x[j]);
				}
			}
			prev = x[i];
			x[i] = (SL->b[i] - soma) / SL->A[i][i];
			if (isinf(x[i]) && retValue == 0)
				retValue = -3;
			diff = realMax(diff, realAbs(x[i] - prev));
		}
		it++;

		if (diff < SL->erro)
		{
			break;
		}
	}
	*tTotal = timestamp() - *tTotal;
	if (retValue == 0)
		return it;
	else
		return retValue;
}

/*!
  \brief Método de Refinamento

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução. Ao iniciar função contém
            valor inicial para início do refinamento
  \param erro menor erro aproximado para encerrar as iterações

  \return código de erro. Um nr positivo indica sucesso e o nr
          de iterações realizadas. Um nr. negativo indica um erro:
          -1 (não converge) -2 (sem solução)
  */
int refinamento(SistLinear_t *SL, real_t *x, double *tTotal)
{
	int it = 0;
	double tGauss;
	real_t diff;
	real_t *w = calloc(SL->n, sizeof(real_t) * SL->n);
	real_t *x_it = malloc(sizeof(real_t) * SL->n);
	SistLinear_t *aux;
	aux = alocaSistLinear(SL->n);
	*tTotal = timestamp();
	while (it < MAXIT)
	{
		// eliminação de gauss altera o sistema,
		// por isso precisamos de cópia
		copiaSL(SL, aux);
		// não precisamos da cópia do b, então liberamos
		free(aux->b);
		aux->b = residuo(SL, x);
		eliminacaoGauss(aux, w, &tGauss);
		diff = 0;
		for (int i = 0; i < SL->n; i++)
		{
			// xn = xn-1 + w
			x_it[i] = x[i] + w[i];
			// faz uma das diferenças para a norma máxima
			diff = realMax(diff, realAbs(x[i] - x_it[i]));
			// atualiza o conteúdo do vetor com a iteração mais recente
			x[i] = x_it[i];
		}
		it++;
		if (diff < SL->erro)
			break;
	}
	*tTotal = timestamp() - *tTotal + tGauss;
	liberaSistLinear(aux);
	free(w);
	free(x_it);
	return it;
}

/*!
  \brief Alocaçao de memória 

  \param n tamanho do SL

  \return ponteiro para SL. NULL se houve erro de alocação
  */
SistLinear_t *alocaSistLinear(unsigned int n)
{
	SistLinear_t *SL;

	SL = malloc(sizeof(SistLinear_t));
	SL->A = malloc(sizeof(real_t *) * n);
	for (int i = 0; i < n; i++)
	{
		SL->A[i] = malloc(sizeof(real_t) * n);
	}
	SL->b = malloc(sizeof(real_t) * n);
	SL->erro = 0;
	SL->n = n;

	return SL;
}

/*!
  \brief Liberaçao de memória 

  \param sistema linear SL
  */
void liberaSistLinear(SistLinear_t *SL)
{
	for (int i = 0; i < SL->n; i++)
	{
		free(SL->A[i]);
	}
	free(SL->A);
	free(SL->b);
	free(SL);
}

/*!
  \brief Leitura de SL a partir de Entrada padrão (stdin).

  \return sistema linear SL. NULL se houve erro (leitura ou alocação)
  */
SistLinear_t *lerSistLinear()
{
	unsigned int n;
	SistLinear_t *SL;

	if (scanf("%d", &n) == EOF)
		return NULL;

	SL = alocaSistLinear(n);
	SL->n = n;

	scanf("%f", &SL->erro);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			scanf("%f", &SL->A[i][j]);
		}
	}
	for (int i = 0; i < n; i++)
	{
		scanf("%f", &SL->b[i]);
	}
	return SL;
}

// Exibe SL na saída padrão
void prnSistLinear(SistLinear_t *SL)
{
	for (int i = 0; i < SL->n; i++)
	{
		for (int j = 0; j < SL->n; j++)
		{
			printf("%f ", SL->A[i][j]);
		}
		printf("%f ", SL->b[i]);
		printf("\n");
	}
}

// Exibe um vetor na saída padrão
void prnVetor(real_t *v, unsigned int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("%1.8e ", v[i]);
	}
	printf("\n");
}

// Funções próprias

// encontra o máximo de determinada coluna
int encontraMax(SistLinear_t *SL, int l)
{
	float max = 0;
	int max_l = l;
	for (int i = 0; i < l; i++)
	{
		if (max < SL->A[l][i])
		{
			max = SL->A[l][i];
			max_l = i;
		}
	}
	return max_l;
}

void trocaLinha(SistLinear_t *SL, int atual, int pivo)
{
	int aux;
	for (int i = 0; i < SL->n; i++)
	{
		aux = SL->A[atual][i];
		SL->A[atual][i] = SL->A[pivo][i];
		SL->A[pivo][i] = aux;
	}
	aux = SL->b[atual];
	SL->b[atual] = SL->b[pivo];
	SL->b[pivo] = aux;
}

void retrossubs(SistLinear_t *SL, real_t *x)
{
	for (int i = SL->n - 1; i >= 0; i--)
	{
		x[i] = SL->b[i];
		for (int j = i + 1; j < SL->n; j++)
		{
			x[i] -= SL->A[i][j] * x[j];
		}
		x[i] /= SL->A[i][i];
	}
}

void copiaSL(SistLinear_t *src, SistLinear_t *dest)
{
	dest->n = src->n;
	dest->erro = src->erro;
	for (int i = 0; i < dest->n; i++)
	{
		for (int j = 0; j < dest->n; j++)
		{
			dest->A[i][j] = src->A[i][j];
		}
		dest->b[i] = src->b[i];
	}
}

real_t *residuo(SistLinear_t *SL, real_t *x)
{
	real_t *r;
	r = calloc(SL->n, sizeof(real_t) * SL->n);

	for (int i = 0; i < SL->n; i++)
	{
		for (int j = 0; j < SL->n; j++)
		{
			r[i] += (SL->A[i][j] * x[j]);
		}
		// r tem Ax_n após o for de cima
		// subtraímos de b para então termos o resíduo

		r[i] = SL->b[i] - r[i];
	}
	return r;
}
