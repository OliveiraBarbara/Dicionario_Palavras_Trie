/***************************************************
Nome: Bárbara Beatriz Bueno de Oliveira
RGA: 2019.0743.014-8
Implementacao 5
Disciplina: Estruturas de Dados e Programacao I
Professor: Ronaldo Fiorilo
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define alfa_size 26 /*tamanho do alfabeto*/

/*Struct da lista para armazenar os sinonimos*/
typedef struct lista_sinonimo{
	char sinonimo[MAX];
	struct lista_sinonimo *prox_sin; 
}lista_sinonimo;

/*Struct da arvore para armazenar as palavras*/
typedef struct arvore{
	char palavra[MAX];
	char idioma[3];
	int folha; /*1=folha*/
	struct lista_sinonimo *primeiro_conceito;
	struct arvore *filho[alfa_size];
}arvore;

/*Escopo de todas as funções utilzadas para essa implementacao*/
lista_sinonimo *aloca_sinonimo(char *s);
arvore* aloca();

void insere(arvore* trie, char* str, char *id, char *sin);

arvore* busca(arvore* trie, char* str);
void imprime_busca(arvore* trie, char* str);

void remover(arvore *trie, char *str);
void remove_sinonimo(arvore *trie, char *str1, char *str2);

void lista_prefixo(arvore *trie, char *id, char *pre);
void imprime_lista(arvore *trie, char *id);

int filhos(arvore *trie);

void libera_sinonimo(lista_sinonimo *lst);
void libera(arvore* trie);
