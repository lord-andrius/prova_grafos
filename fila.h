#ifndef FILA_H
#define FILA_H
#include <stdlib.h>
#include <stdbool.h>

struct _Elemento
{
    void *elemento;
    struct _Elemento *proximo;
};


struct Fila 
{
    struct _Elemento *primeiro_elemento;
    size_t tamanho_elemento;
    size_t quantidade_elementos;
};

struct Fila *criar_fila(size_t tamanho_elemento);
void deletar_fila(struct Fila **fila);
void iterar(struct Fila *fila, void (*callback) (struct Fila *fila, struct _Elemento *atual, void *data), void *data);
bool adicionar_elemento(struct Fila *fila, void *elemento);
bool remover_elemento(struct Fila *fila, void *removido);

#endif

