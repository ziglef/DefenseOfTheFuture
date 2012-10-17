#ifndef HEAP_H_
#define HEAP_H_

typedef struct {
	int prioridade;
	char * valor;
} elemento;

typedef struct {
	/* este campo diz o número de elementos correntemente no vector */
	int tamanho_actual;
	/* este campo indica o tamanho maximo do vector */
	int tamanho_maximo;

	/* vector de apontadores para elementos */
	elemento ** elementos;
} heap;

/* cria nova heap */
heap* heap_cria(int tamanho_maximo);

/* apaga heap */
void heap_apaga(heap *h);

/* insere elemento na heap com uma determinada prioridade */
int heap_insere(heap *h, char * texto, int prioridade);

/* remove elemento na raiz da heap */
char * heap_remove(heap *h);

#endif /* HEAP_H_ */