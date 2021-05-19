struct intervalo{
    float min;
    float max;
};

struct intervalo soma(struct intervalo a, struct intervalo b);

struct intervalo subtracao(struct intervalo a, struct intervalo b);

struct intervalo multiplicacao(struct intervalo a, struct intervalo b);

struct intervalo divisao(struct intervalo a, struct intervalo b);

struct intervalo faz_op(struct intervalo a, struct intervalo b, char op);

float min_intervalo(float a);

float max_intervalo(float a);

int unitario(struct intervalo a);

int intervalo_valido(struct intervalo a);