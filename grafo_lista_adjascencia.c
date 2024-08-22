#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <time.h>
#include "grafo_lista_adjascencia.h"
#include "fila.h"




struct Grafo *cria_grafo(void) {
	struct Grafo *grafo = calloc(1, sizeof(struct Grafo));
	return grafo;	
}

void destroi_grafo(struct Grafo **grafo) {
	if(*grafo == NULL) return;

	for(int i = (*grafo)->numero_vertices - 1; i >= 0; i--){
		if((*grafo)->vertices[i] != NULL) {
			if((*grafo)->vertices[i]->adjascentes != NULL) {
				free((*grafo)->vertices[i]->adjascentes);
			}
			free((*grafo)->vertices[i]);
		}
	}

	free((*grafo)->vertices);
	free(*grafo);
	*grafo = NULL;
}

struct Vertice *procura_vertice(struct Grafo *grafo,int dado) {
	struct Vertice *vertice = NULL;	
	for(int i = 0; i < grafo->numero_vertices; i++) {
		if(grafo->vertices[i]->dado == dado) {
			vertice = grafo->vertices[i];
			break;
		}
	}

	return vertice;
}

struct Vertice *cria_vertice(struct Grafo *grafo, int dado) {
	struct Vertice *vertice = NULL;
	int pode_criar = 1;
	for(int i = 0; i < grafo->numero_vertices; i++) {
		if(grafo->vertices[i]->dado == dado) {
			pode_criar = 0;
			break;
		}
	}
	
	if(pode_criar == 0) return NULL;

	vertice = calloc(1, sizeof(struct Vertice));
	if (vertice == NULL) return NULL;
	vertice->dado = dado;
	vertice->cor = BRANCO;

	return vertice;
}

void buscar_por_conexoes(struct Vertice *vertice) {
	if(vertice->cor != BRANCO) return;

	vertice->cor = CINZA;
	for(int i = 0; i < vertice->qtd_adjascentes; i++) {
		buscar_por_conexoes(vertice->adjascentes[i]);
	}
	vertice->cor = PRETO;
}

void limpar_resultado_da_busca_de_conexoes(struct Grafo *grafo) {
	for(int i = 0; i < grafo->numero_vertices; i++) {
		grafo->vertices[i]->cor = BRANCO;
	}
}


void mostrar_conexoes(struct Grafo *grafo) {
	for(int i = 0; i < grafo->numero_vertices; i++) {
		if(grafo->vertices[i]->cor == PRETO) {
			printf("%d, ", grafo->vertices[i]->dado);
		}
	}
}




int adiciona_conecao(struct Vertice *a, struct Vertice *b) {
	assert(a != NULL);
	assert(b != NULL);
	int pode_adicionar = 1;
	for(int i = 0; i < a->qtd_adjascentes; i++) {
		if(a->adjascentes[i]->dado == b->dado) {
			pode_adicionar = 0;
			break;
		}
	}

	// Se der algum bug por favor verifique se o a está em b :)
	
	if(pode_adicionar) {
		// melhor lidar com o realloc. No momento se houver uma falha na alocação vamos ter um vazamento de memória(Melhor o seu pc ter RAM!).
		a->adjascentes = realloc(a->adjascentes,sizeof(struct Vertice **) * (a->qtd_adjascentes + 1));
		if(a->adjascentes == NULL) return 1;
		b->adjascentes = realloc(b->adjascentes,sizeof(struct Vertice **) * (b->qtd_adjascentes + 1));
		if(b->adjascentes == NULL) return 1;
		a->adjascentes[a->qtd_adjascentes++] = b;
		b->adjascentes[b->qtd_adjascentes++] = a;
		return 0;
	} else {
		return 1;
	}
}

int adiciona_ou_modifica_grafo(struct Grafo *grafo, int dado, struct Vertice **conexoes, int qtd_conexoes) {
	if(!grafo) return 1;

	struct Vertice *vertice = procura_vertice(grafo, dado);

	int deve_add = vertice == NULL;

	if(deve_add) {
		if((vertice = cria_vertice(grafo,dado)) == NULL) return 1;
		struct Vertice **novos_vertices = realloc(grafo->vertices,sizeof(struct Vertice **) * (grafo->numero_vertices + 1));
		if(novos_vertices == NULL) {
			free(vertice);
			return 1;
		}
		grafo->vertices = novos_vertices;
		grafo->vertices[grafo->numero_vertices++] = vertice;
	}


	for(int i = 0; i < qtd_conexoes; i++) {
		if(conexoes[i] == NULL) continue;
		if(adiciona_conecao(vertice, conexoes[i])) return 1; // caso dê erro em alguma conexão
	}

	return 0;
	
}

void printa_grafo(struct Grafo *grafo) {
	for(int i = 0; i < grafo->numero_vertices; i++) {
		printf("%d: ", grafo->vertices[i]->dado);
		for(int i2 = 0; i2 < grafo->vertices[i]->qtd_adjascentes; i2++) {
			printf("%d ", grafo->vertices[i]->adjascentes[i2]->dado);
		}
		puts("");
	}
}

int deletar_vertice(struct Grafo *grafo, int dado) {
	assert(grafo != NULL);

	for(int i = 0; i < grafo->numero_vertices; i++){
		for(int i2 = 0; i2 < grafo->vertices[i]->qtd_adjascentes; i2++) {
			if(grafo->vertices[i]->adjascentes[i2]->dado == dado) {
				struct Vertice *tmp = grafo->vertices[i]->adjascentes[i2];


				grafo->vertices[i]->adjascentes[i2] = grafo->vertices[i]->adjascentes[grafo->vertices[i]->qtd_adjascentes - 1];

				grafo->vertices[i]->adjascentes[grafo->vertices[i]->qtd_adjascentes - 1] = tmp;
				grafo->vertices[i]->qtd_adjascentes -= 1;
				struct Vertice **novos_adjascentes = realloc(grafo->vertices[i]->adjascentes, grafo->vertices[i]->qtd_adjascentes * sizeof(struct Vertice **));
				if(!novos_adjascentes) return 1;
				grafo->vertices[i]->adjascentes = novos_adjascentes;
				if(grafo->vertices[i]->qtd_adjascentes == 0) grafo->vertices[i]->adjascentes = NULL;
			}

		}
		if(grafo->vertices[i]->dado == dado) {
			struct Vertice *tmp = grafo->vertices[i];
			grafo->vertices[i] = grafo->vertices[grafo->numero_vertices - 1];
			grafo->vertices[grafo->numero_vertices - 1] = tmp;
			grafo->numero_vertices--;
			struct Vertice **novos_vertices = realloc(grafo->vertices, grafo->numero_vertices * sizeof(struct Vertice **));
			if(!novos_vertices) return 1;
			grafo->vertices = novos_vertices;
			if(grafo->numero_vertices == 0) grafo->vertices = NULL;
		}
	} 

	return 0;

}

// se não houver um caminho essa função retorna um caminho contendo apenas a
// origem.
void menor_caminho(struct Fila *caminho, struct Vertice *origem, struct Vertice *destino) {
	if(origem->cor == CINZA) return;
	adicionar_elemento(caminho, &origem->dado);
	origem->cor = CINZA;

	if(origem->dado == destino->dado || origem->qtd_adjascentes == 0) {
		origem->cor = BRANCO;
		return;
	}

	struct Fila **possiveis_caminhos = malloc(sizeof(caminho) * origem->qtd_adjascentes);
	if(possiveis_caminhos == NULL) {
		printf("Erro de alocação de memória!\n");
		exit(1);
	}

	for(int i = 0; i < origem->qtd_adjascentes; i++) {
		possiveis_caminhos[i] = criar_fila(sizeof(origem->dado));
		if(possiveis_caminhos == NULL) {
			printf("Erro de alocação de memória!\n");
			exit(1);
		}
		menor_caminho(possiveis_caminhos[i], origem->adjascentes[i], destino);
	}
	struct Fila *menor_caminho = NULL;
	for(int i = 0; i < origem->qtd_adjascentes; i++) {
		int chegou_ao_destino = 0;

		struct _Elemento *proximo = NULL;
		for (struct _Elemento *atual = possiveis_caminhos[i]->primeiro_elemento; atual != NULL; atual = proximo)
		{
			proximo = atual->proximo;
			int dado_do_vertice_atual = *(int *)atual->elemento;
			if(destino->dado == dado_do_vertice_atual) {
				chegou_ao_destino = 1;
				break;
			}
		}

		if(chegou_ao_destino) {
			if(menor_caminho == NULL) {
				menor_caminho =  possiveis_caminhos[i];
			} else {
				if(menor_caminho->quantidade_elementos > possiveis_caminhos[i]->quantidade_elementos) {
					menor_caminho =  possiveis_caminhos[i];
				}
			}
		}
	}

	// caso haja um menor caminho vamos adiciona-lo no caminho
	if (menor_caminho != NULL) {
		struct _Elemento* proximo = NULL;
		for (struct _Elemento* atual = menor_caminho->primeiro_elemento; atual != NULL; atual = proximo)
		{
			proximo = atual->proximo;
			int dado_do_vertice_atual = *(int*)atual->elemento;
			adicionar_elemento(caminho, &dado_do_vertice_atual);
		}
	}

	

	for(int i = 0; i < origem->qtd_adjascentes; i++) {
		deletar_fila(possiveis_caminhos + i);
	}
	free(possiveis_caminhos); 

	origem->cor = BRANCO;
	return;
}

bool inicializar_grafo_com_20_vertices(struct Grafo *grafo) {
	if (grafo == NULL) return false;
	for (int i = 0; i < 20; i++) {
		if (adiciona_ou_modifica_grafo(grafo, i, NULL, 0)) {
			for (int j = grafo->numero_vertices; j >= 0; j--) {
				free(grafo->vertices[j]->adjascentes);
				free(grafo->vertices[j]);
			}
			destroi_grafo(&grafo);
			return false;
		}
	}
	return true;
}

// Só deve ser chamado quando não há conexões feitas manualmente
void fazer_conexoes_aleatorias(struct Grafo* grafo) {
	int qtd_conexoes = 0;
	while (qtd_conexoes < grafo->numero_vertices)
	{
		int a = rand() % (grafo->numero_vertices - 1);
		int b = rand() % (grafo->numero_vertices - 1);
		if (a == b) continue;
		if (adiciona_ou_modifica_grafo(grafo, grafo->vertices[a]->dado, (struct Vertice* []) { procura_vertice(grafo, grafo->vertices[b]->dado) }, 1) == 0) {
			qtd_conexoes++;
		}
	}
}

int main(void) {
	setlocale(LC_ALL, "");
	srand(time(NULL));
	struct Grafo* grafo = cria_grafo();
	if (grafo == NULL) {
		fprintf(stderr, "Não foi possível criar a rede de computadores!\n");
		exit(1);
	}

	puts("INIZIALIZANDO A REDE DE COMPUTADORES....");

	if (!inicializar_grafo_com_20_vertices(grafo)) {
		fprintf(stderr, "Não foi possível inicializar a rede de computadores!\n");
	};

	puts("Esse são os computadores que estão na rede(cada número é um computador):");
	for (int i = 0; i < grafo->numero_vertices; i++) printf("%d ", grafo->vertices[i]->dado);
	puts("");

	puts("GERANDO CONEXÕES ALEATÓRIAS");
	fazer_conexoes_aleatorias(grafo);
	puts("Esses são os computadores da rede com suas respectivas conexões: ");
	printa_grafo(grafo);
	
	puts("CALCULANDO MENOR ROTA ENTRE COMPUTADORES!");

	int partida = 0;
	int destino = 0;
	while (partida == destino) {
		printf("Digite o número do computador de partida: ");
		scanf("%d", &partida);
		printf("Digite o número do computador de destino: ");
		scanf("%d", &destino);
		if (procura_vertice(grafo, partida) == NULL || procura_vertice(grafo, destino) == NULL) {
			puts("Você digitou número de computadores inválidos. Tente novamente!");
			partida = 0;
			destino = 0;
			continue;
		}
		if (destino == partida) {
			puts("Por favor seleciona uma partida e um destino diferente!");
		}
	}

	struct Fila* caminho_mais_curto = criar_fila(sizeof(int));
	if (caminho_mais_curto == NULL) {
		fprintf(stderr, "Não foi possível calcular o caminho mais curto por conta de um erro de memória!\n");
		exit(1);
	}
	menor_caminho(caminho_mais_curto, procura_vertice(grafo,partida), procura_vertice(grafo, destino));

	if (caminho_mais_curto->quantidade_elementos != 1) {
		int contador = 1;
		for (struct _Elemento* elemento = caminho_mais_curto->primeiro_elemento; elemento != NULL; elemento = elemento->proximo) {
			if (contador == caminho_mais_curto->quantidade_elementos) {
				printf("%d\n", *(int*)elemento->elemento);
			}
			else {
				printf("%d -> ", *(int*)elemento->elemento);
			}
			contador++;
		}
	}
	else {
		printf("Não há um caminho que ligue o computador %d  ao computador %d\n", partida, destino);
	}

	puts("FIM...");

	deletar_fila(&caminho_mais_curto);
	destroi_grafo(&grafo);
	return 0;
}
