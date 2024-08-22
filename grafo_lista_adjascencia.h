#include "fila.h"
#ifndef GRAFO_LISTA_ADJASCENCIA
#define GRAFO_LISTA_ADJASCENCIA

enum Cor {
	BRANCO,
	CINZA,
	PRETO
};

struct Vertice {
	int dado;
	enum Cor cor;
	int qtd_adjascentes;
	struct Vertice **adjascentes;
};

struct Grafo {
	int numero_vertices;
	struct Vertice **vertices;
};

struct Grafo *cria_grafo(void);
void destroi_grafo(struct Grafo **);

struct Vertice *procura_vertice(struct Grafo *grafo,int dado);
struct Vertice *cria_vertice(struct Grafo *grafo, int dado);
int adiciona_conecao(struct Vertice *a, struct Vertice *b);
	
int adiciona_ou_modifica_grafo(struct Grafo *grafo, int dado, struct Vertice **conexoes, int qtd_conexoes);

void printa_grafo(struct Grafo *grafo);

int deletar_vertice(struct Grafo *grafo, int dado);

void buscar_por_conexoes(struct Vertice *vertice);

void mostrar_conexoes(struct Grafo *grafo);

void limpar_resultado_da_busca_de_conexoes(struct Grafo *grafo);

void menor_caminho(struct Fila *caminho, struct Vertice *origem, struct Vertice *destino);

#endif
