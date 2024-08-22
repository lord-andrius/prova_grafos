#include "fila.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static struct _Elemento *cria_elemento(size_t tamanho_elemento);
static void delete_elemento(struct _Elemento *elemento);

struct _Elemento *cria_elemento(size_t tamanho_elemento)
{
    struct _Elemento *elemento;
    if (!(elemento = malloc(sizeof(struct _Elemento)))) return NULL;
    if (!(elemento->elemento = malloc(tamanho_elemento))) 
    {
        free(elemento);
        return NULL;
    }   
    elemento->proximo = NULL;
    return elemento;
}

static void delete_elemento(struct _Elemento *elemento)
{
    free(elemento->elemento);
    free(elemento);
}

struct Fila *criar_fila(size_t tamanho_elemento)
{
    struct Fila *fila = malloc(sizeof(struct Fila));
    if (fila != NULL)
    {
        fila->primeiro_elemento = NULL;
        fila->tamanho_elemento = tamanho_elemento;
        fila->quantidade_elementos = 0;
    }
    return fila;
}

void deletar_fila(struct Fila **fila)
{
    struct _Elemento *proximo = NULL;
    for(struct _Elemento *atual = (*fila)->primeiro_elemento; atual != NULL; atual = proximo)
    {
        proximo = atual->proximo;
        delete_elemento(atual);
    }
    free(*fila);
    *fila = NULL;
}

void iterar(struct Fila *fila, void (*callback) (struct Fila *fila, struct _Elemento *atual, void *data), void *data)
{
   struct _Elemento *proximo = NULL;
   for (struct _Elemento *atual = fila->primeiro_elemento; atual != NULL; atual = proximo)
   {
        proximo = atual->proximo;
        callback(fila, atual, data);
   } 
}

static void adicionar_elemento_callback(struct Fila *fila, struct _Elemento *atual, void *data)
{
    if(!atual->proximo) atual->proximo = data;
}


bool adicionar_elemento(struct Fila *fila, void *elemento)
{
    struct _Elemento *novo_elemento = NULL;
    if (!(novo_elemento = cria_elemento(fila->tamanho_elemento))) return false;
    memcpy(novo_elemento->elemento, elemento, fila->tamanho_elemento);
    if (fila->quantidade_elementos == 0)
    {
        fila->primeiro_elemento = novo_elemento;
    }
    else
    {
        iterar(fila, adicionar_elemento_callback, novo_elemento);
    }
    fila->quantidade_elementos++;
    return true;
}

bool remover_elemento(struct Fila *fila, void *removido)
{
    if (fila->quantidade_elementos == 0)
    {
        return false;
    }

    memcpy(removido, fila->primeiro_elemento->elemento,fila->tamanho_elemento);

    struct _Elemento *elemento_a_ser_removido = fila->primeiro_elemento;

    fila->primeiro_elemento = fila->primeiro_elemento->proximo;

    delete_elemento(elemento_a_ser_removido);

    fila->quantidade_elementos--;
    
    return true;

}



