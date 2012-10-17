
/*****************************************************************/
/* Parte do trabalho pratico 3 - Filas | PROG2 | MIEEC | 2010/11 */      
/*****************************************************************/

#include <stdlib.h>
#include <string.h>
#include "fila.h"

fila* fila_cria(int capacidade)
{
	fila *f = (fila *) malloc(sizeof(fila));
	if (f == NULL)
		return NULL;
	f->capacidade = capacidade;
	f->cabeca = f->cauda = NULL;

	return f;
}

int	fila_vazia(fila *f)
{
	if(f == NULL)
		return FILA_ERRO;

	return f->cabeca == NULL;
}

int	fila_cheia(fila *f)
{
	if(f == NULL)
		return FILA_ERRO;

	if (f->capacidade == FILA_ILIMITADA)
		return 0;

	return fila_comprimento(f) >= f->capacidade;
}

int	fila_push(fila *f, const char *string)
{
	filaItem *novo, *ult = f->cauda;

	if(f == NULL || string == NULL)
		return FILA_ERRO;

	if (fila_cheia(f))
		return FILA_CHEIA;

	novo = (filaItem *) malloc(sizeof(filaItem));
	if(novo == NULL)
		return FILA_ERRO;

	novo->string = (char*) calloc(strlen(string)+1, sizeof(char));
	if(novo->string == NULL)
	{
		free(novo);
		return FILA_ERRO;
	}

	strcpy(novo->string, string);
	novo->proximo = NULL;

	if (ult == NULL)
		f->cabeca = f->cauda = novo;
	else
	{
		f->cauda->proximo = novo;
		f->cauda = novo;
	}

	return FILA_OK;
}

char* fila_pop(fila *f)
{
	filaItem *cabeca;
	char *ret;

	if (f == NULL || fila_vazia(f))
		return NULL;

	cabeca = f->cabeca;
	f->cabeca = cabeca->proximo;
	ret = cabeca->string;

	if (f->cabeca == NULL)
		f->cauda = NULL;
	
	free(cabeca);
	return ret;
}

int	fila_comprimento(fila *f)
{
	int count = 0;
	filaItem *cur;

	if(f == NULL)
		return FILA_ERRO;

	cur = f->cabeca;
	while(cur != NULL)
	{
		count++;
		cur = cur->proximo;
	}
	return count;
}

char* fila_cabeca(fila *f)
{
	if (f == NULL || fila_vazia(f))
		return NULL;
	return f->cabeca->string;
}

char* fila_cauda(fila *f)
{
	if (f != NULL && f->cauda != NULL)
		return f->cauda->string;
	else
		return NULL;
}

void fila_apaga(fila **f_ref)
{
	filaItem * cur, * next;
	fila *f = *f_ref;

	if(f == NULL)
		return;

	if (f->cabeca != NULL)
	{
		cur = f->cabeca;
		while (cur->proximo != NULL)
		{
			free(cur->string);
			next = cur->proximo;
			free(cur);
			cur = next;
		}
	}

	free(f);
	*f_ref = NULL;
}

