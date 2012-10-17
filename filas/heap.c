#include <stdlib.h>
#include <string.h>
#include "heap.h"

#define RAIZ 			(0)
#define PAI(x) 			((x-1)/2)
#define FILHO_ESQ(x) 	((x*2)+1)
#define FILHO_DIR(x) 	((x*2)+2)

int menor_que(elemento * e1, elemento * e2);

heap* heap_cria(int tamanho_maximo)
{
	heap * h = (heap *) malloc(sizeof(heap));
	if (!h)
		return NULL;
	h->tamanho_actual = 0;
	h->tamanho_maximo = tamanho_maximo;
	h->elementos = (elemento **) calloc(tamanho_maximo, sizeof(elemento *));
	if (!h->elementos) {
		free(h);
		return NULL;
	}
	return h;
}

void heap_apaga(heap *h)
{
	if(!h)
		return;

	free(h->elementos);
	free(h);
}

elemento * elemento_cria(int prioridade, const char * valor)
{
	elemento * elem = (elemento *) malloc(sizeof(elemento));
	if (!elem)
		return NULL;
	elem->valor = strdup(valor);
	elem->prioridade = prioridade;
	return elem;
}

int heap_insere(heap * h, char * texto, int prioridade)
{
	elemento * aux, * elem;
	int i;

	if (h->tamanho_actual >= h->tamanho_maximo)
		return 0;

	elem = elemento_cria(prioridade, texto);

	i = h->tamanho_actual;
	h->elementos[i] = elem;
	h->tamanho_actual++;
	while (i != RAIZ && menor_que(h->elementos[i], h->elementos[PAI(i)])) {
		aux = h->elementos[PAI(i)];
		h->elementos[PAI(i)] = h->elementos[i];
		h->elementos[i] = aux;
		i = PAI(i);
	}
	return 1;
}


char * heap_remove(heap * h)
{
	int i, filho_maior;
	elemento * aux;
	char * ret;

	if (!h || h->tamanho_actual <= 0)
		return NULL;

	ret = h->elementos[RAIZ]->valor;
	free(h->elementos[RAIZ]);

	h->tamanho_actual--;
	h->elementos[RAIZ] = h->elementos[h->tamanho_actual];
	h->elementos[h->tamanho_actual] = NULL;

	i = RAIZ;

	while(FILHO_ESQ(i) < h->tamanho_actual) {
		filho_maior = FILHO_ESQ(i);
		if (FILHO_DIR(i) < h->tamanho_actual && menor_que(h->elementos[FILHO_DIR(i)], h->elementos[FILHO_ESQ(i)]))
			filho_maior = FILHO_DIR(i);

		if (menor_que(h->elementos[filho_maior], h->elementos[i])) {
			aux = h->elementos[filho_maior];
			h->elementos[filho_maior] = h->elementos[i];
			h->elementos[i] = aux;
			i = filho_maior;
		}
		else
			break;
	}
	return ret;
}


int menor_que(elemento * e1, elemento * e2)
{
	if (e1 == NULL || e2 == NULL) 
	{
		return 0;
	}
	return e1->prioridade < e2->prioridade;
}