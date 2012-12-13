/*****************************************************************/
/* Trabalho pratico 4 - Tab. Dispersao | PROG2 | MIEEC | 2010/11 */
/*****************************************************************/

#ifndef TABDISPERSAO_H
#define TABDISPERSAO_H

#define TABDISPERSAO_OK   		(0)
#define TABDISPERSAO_INVALIDA   (-1)
#define TABDISPERSAO_ERRO       (-2)
#define TABDISPERSAO_NAOEXISTE  (-3)

typedef unsigned long hash_func(const char, int);

/* tabela de dispersão baseada em encadeamento */
typedef struct elem
{
	unsigned long color;
	struct elem * proximo;
} elemento;

/*
 * \brief A estrutura tabela_dispersao para armazenar dados relativos a uma tabela de dispersao
 */
typedef struct
{
	hash_func *hfunc;		/* apontador para a funcao a ser usada (hash_djb, hash_kr, ...) */
	elemento **elementos;	/* vector de listas de elementos */
	int tamanho;			/* tamanho do vector de listas de elementos */
} tabela_dispersao;

/**
 * \brief cria uma tabela de dispersao
 * \param tamanho tamanho da tabela de dispersao
 * \param hash_func apontador para funcao de dispersao a ser usada nesta tabela
 * \return uma tabela de dispersao vazia que usa a funcao de dispersao escolhida
 */
tabela_dispersao* tabela_cria(int tamanho, hash_func *hfunc);

/**
 * \brief elimina uma lista, libertando toda a memoria ocupada
 * \param td tabela de dispersao a ser apagada da memoria
 * \remark apagar _todos_ os elementos anteriormente alocados na memoria
 */
void tabela_apaga(tabela_dispersao *td);

/**
 * \brief adiciona um novo valor 'a tabela
 * \param td tabela onde adicionar o elemento
 * \param valor valor a ser adicionado 
 * \return se o valor for adicionado correctamente, a funcao retorna TABDISPERSAO_OK
 * \remark a tabela nao deve ficar com valores repetidos.
 * \remark valor deve ser copiado para outro local em memoria.
 */
int tabela_adiciona(tabela_dispersao *td, const unsigned long valor, const char key);

/**
 * \bried remove um valor da tabela
 * \param td Tabela de onde remover o valor
 * \param valor valor a ser removido da tabela
 * \return se o valor passado como argumento nao existir na tabela, a funcao retorna TABDISPERSAO_NAOEXISTE.
 * \return se o valor foi adicionado correctamente a funcao retorna TABDISPERSAO_OK
 */
int tabela_remove(tabela_dispersao *td, const unsigned long valor, const char key);

/**
 * \brief verifica se um valor existe na tabela
 * \param td tabela onde procurar o valor
 * \param valor valor a ser procurado
 * \return 0 se o valor nao existir ou != 0 se o valor existir
 */
int tabela_existe(tabela_dispersao *td, const unsigned long valor, const char key);


/**
 * \brief apaga todos os valores correntemente na tabela, ficando a tabela vazia
 * \param td tabela a ser esvaziada
 * \return TABDISPERSAO_OK ou TABDISPERSAO_INVALIDA de acordo com o resultado
 */
int tabela_esvazia(tabela_dispersao * td);

/**
 * \brief determina o numero de elementos na tabela de dispersao
 * \param td tabela de dispersao
 * \return numero de elementos na tabela
 */
int tabela_numelementos(tabela_dispersao * td);

unsigned long hash_lcom(const char key, int tamanho);

/*
 * Esta funcao imprime o estado actual da tabela para o ecra
 * \param td Tabela a ser mostrada no ecra
 */
void mostraTabela(tabela_dispersao * td);

#endif
