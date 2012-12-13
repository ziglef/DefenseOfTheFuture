/*****************************************************************/
/* Trabalho pratico 4 - Tab. Dispersao | PROG2 | MIEEC | 2010/11 */
/*****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tabdispersao.h"

tabela_dispersao* tabela_cria(int tamanho, hash_func *hfunc)
{
	tabela_dispersao * t = (tabela_dispersao *) malloc(sizeof(tabela_dispersao));
	if(t == NULL || hfunc == NULL)
		return NULL;

	t->tamanho = tamanho;
	t->elementos = (elemento **) calloc(tamanho, sizeof(elemento*));	
	
	if(t->elementos == NULL)
	{
		free(t);
		return NULL;
	}

	t->hfunc = hfunc;
	return t;
}

void tabela_apaga(tabela_dispersao *td)
{
	int i;
	elemento * elem, * aux;

	for (i = 0; i < td->tamanho; i++)
	{
		if (td->elementos[i])
		{
			elem = td->elementos[i];
			while (elem)
			{
				aux = elem->proximo;
				free(elem->string);
				free(elem);
				elem = aux;
			}
		}
	}
	free(td->elementos);
	free(td);
}

int tabela_adiciona(tabela_dispersao *td, const unsigned long valor, const char key)
{
	int index;
	elemento * elem;

	if (!td) return TABDISPERSAO_ERRO;

	if (tabela_existe(td, valor, key))
		return TABDISPERSAO_OK;

	index = td->hfunc(key, td->tamanho);
	elem = (elemento *) malloc(sizeof(elemento));
	if(elem == NULL)
		return TABDISPERSAO_ERRO;

	elem->color = valor;
	elem->proximo = td->elementos[index];
	td->elementos[index] = elem;
	
	return TABDISPERSAO_OK;
}

int tabela_remove(tabela_dispersao *td, const unsigned long valor, const char key)
{
	int index;
	elemento * elem, * aux;

	if (!td) return TABDISPERSAO_ERRO;

	index = td->hfunc(key, td->tamanho);
	elem = td->elementos[index];
	aux = NULL;

	while(elem != NULL)
	{
		if (valor == elem->color)
		{
			if (aux != NULL)
				aux->proximo = elem->proximo;
			else
				td->elementos[index] = elem->proximo;

			free(elem);

			return TABDISPERSAO_OK;
		}

		aux = elem;
		elem = elem->proximo;
	}
	return TABDISPERSAO_NAOEXISTE;
}

int tabela_existe(tabela_dispersao *td, const unsigned long valor, const char key)
{
	int index;
	elemento * elem;

	if (!td) return TABDISPERSAO_ERRO;

	index = td->hfunc(key, td->tamanho);

	elem = td->elementos[index];
	while(elem != NULL)
	{
		if (elem->color == valor)
			return 1;
		elem = elem->proximo;
	}
	return 0;
}

int tabela_esvazia(tabela_dispersao * td)
{
	int i;
	elemento * elem, * aux;

	if (!td) return TABDISPERSAO_ERRO;

	for (i = 0; i < td->tamanho; i++)
	{
		elem = td->elementos[i];
		while(elem != NULL)
		{
			aux = elem->proximo;	
			free(elem);
			elem = aux;			
		}
		td->elementos[i] = NULL;
	}
	return TABDISPERSAO_OK;
}

int tabela_numelementos(tabela_dispersao * td)
{
	int i, count;
	elemento * elem;
	count = 0;
	if (!td) 
		return TABDISPERSAO_ERRO;
	for (i = 0; i < td->tamanho; i++)
	{
		elem = td->elementos[i];
		while(elem != NULL)
		{
			elem = elem->proximo;
			count++;
		}
	}
	return count;
}

unsigned long hash_lcom(const char key, int tamanho)
{
	return (int)key;
}

void mostraTabela(tabela_dispersao * td)
{
	int i;
	elemento * elem;
	printf("TABELA DE DISPERSAO (%d elementos)\n", tabela_numelementos(td));
	for (i = 0; i < td->tamanho; i++)
	{
		if (td->elementos[i])
		{
			printf("\t[%2d]", i);
			elem = td->elementos[i];
			while (elem)
			{
				printf(" : \"%ul\"", elem->color);
				elem = elem->proximo;
			}
			printf("\n");
		}
	}
	printf("\n");
}
