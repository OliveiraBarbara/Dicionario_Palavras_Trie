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
#include "dic.h"

lista_sinonimo *aloca_sinonimo(char *s){
	lista_sinonimo *nova;
	
	nova = (lista_sinonimo *) malloc (sizeof (lista_sinonimo)); /*aloco um espaco na memoria*/
	
	if(nova != NULL){
		strcpy(nova->sinonimo, s); /*na posição da lista na variavel do sinonimo eu atribuo o sinonimo correto*/
		nova->prox_sin = NULL;
	}else /*se a alocacao nao deu certo, informo na tela*/
		printf("Erro ao alocar memória\n");
		
	return nova;
}

arvore* aloca(){
	int i;
	arvore* nova = (arvore *) malloc (sizeof (arvore)); /*aloco um espaço na memória*/
	if(nova != NULL){
		for (i=0; i<alfa_size; i++)
			nova->filho[i] = NULL;
		nova->folha = 0;
	}else /*se a alocacao nao deu certo, informo na tela*/
		printf("Erro ao alocar memória\n");
	
	return nova;
}

void insere(arvore* trie, char* str, char *id, char *sin){
	int i, ascii;
	lista_sinonimo *aux, *nova;
	
	for (i=0; str[i] != '\0'; i++) {
		ascii = str[i] - 97;
		if (trie->filho[ascii] == NULL)
			trie->filho[ascii] = aloca();
		trie = trie->filho[ascii];
	}
	trie->folha = 1;
	strcpy(trie->palavra, str);
	strcpy(trie->idioma, id);
	
	if(trie->primeiro_conceito == NULL){
		aux = aloca_sinonimo(sin);
		aux->prox_sin = trie->primeiro_conceito;
		trie->primeiro_conceito = aux;
	}else{
		if(strcmp(sin, (trie)->primeiro_conceito->sinonimo) < 0){ 
			nova = aloca_sinonimo(sin);
			nova->prox_sin = trie->primeiro_conceito;
			trie->primeiro_conceito = nova;
		}else{ 
			aux = trie->primeiro_conceito;
			
			while(aux->prox_sin != NULL && (strcmp(sin, trie->primeiro_conceito->sinonimo) > 0))
				aux = aux->prox_sin;
			
			nova = aloca_sinonimo(sin);
			nova->prox_sin = aux->prox_sin;
			aux->prox_sin = nova;
		}
	}
}

arvore* busca(arvore* trie, char* str){
	arvore* aux = trie;
	int i, ascii;
	
	for(i=0; str[i] != '\0'; i++){
		ascii = str[i] - 97;
		if (aux->filho[ascii] == NULL)
			return NULL;
		aux = aux->filho[ascii];
	}
	if (aux != NULL && aux->folha == 1)
		return aux;
	return NULL;
}

void imprime_busca(arvore* trie, char* str){
    arvore *aux;
    lista_sinonimo *q;
    
    aux = busca(trie, str);
    if (aux != NULL){
        for(q = aux->primeiro_conceito; q != NULL; q = q->prox_sin)
			printf("%s\n", q->sinonimo);
    }else
        printf("hein?\n");
}

void remover(arvore *trie, char *str){
	arvore *aux = trie;
	
	if(trie != NULL){
		if(*str != '\0'){
			if(aux->filho[*str-97] != NULL){
				if(filhos(aux) != 0){
					if(aux->folha == 0){
						remover(aux->filho[*str-97], str+1);
					}else{
						aux->filho[*str-97] = NULL;
					}
				}else{
					free(aux);
					aux = NULL;
				}
			}
		}else{
			if(aux->folha == 1){
				if(filhos(aux) == 0){
					aux->folha = 0;
					libera_sinonimo(aux->primeiro_conceito);
					aux->primeiro_conceito = NULL;
					free(aux->filho[*str-97]);
					aux->filho[*str-97] = NULL;
				}else{
					aux->folha = 0;
					strcpy(aux->palavra, "");
					strcpy(aux->idioma, "");
					libera_sinonimo(aux->primeiro_conceito);
				}
			}
		}
	}
}

void remove_sinonimo(arvore *trie, char *str1, char *str2){
	arvore *q, *aux;
	lista_sinonimo *p, *s;
	s = NULL;	
	
	if(trie != NULL){
		aux = busca(trie, str1);
		
		p = aux->primeiro_conceito;
		q = aux;
		while (p != NULL && (strcmp(p->sinonimo, str2) != 0)){ /*procuro o sinonimo na lista de sinonimo*/
			s = p;
			p = p->prox_sin;
		}
		if (p != NULL){ /*se ele estiver na lista eu verifico se ele é o primeiro sinonimo, se for o primeiro eu faco a primeira posicao da lista de sinonimo receber o sinonimo seguinte do que eu vou remover, e se ele nao for o primeiro, faco o sinonimo anterior receber o sinonimo seguinte do que vou remover*/
			if(s != NULL)
				s->prox_sin = p->prox_sin;
			else
				aux->primeiro_conceito = p->prox_sin;
				
			free(p); /*removo o sinonimo*/
		}
		if(q != NULL && q->primeiro_conceito == NULL){ /*verifico se a palavra ficou sem sinonimo, se ficou eu chamo a funcao para remover ela*/
			remover(trie, str1);
			printf("aqui\n");
		}
	}
}

void lista_prefixo(arvore *trie, char *id, char *pre){
	int i, tam;
	arvore *aux;
	
	if(trie != NULL){
		for(i=0; i<alfa_size; i++){
			aux = trie->filho[i];
			if(aux != NULL){
				tam = strlen(pre);
				if(aux->folha == 1 && strncmp(aux->palavra, pre, tam) == 0 && strcmp(id, aux->idioma) == 0){
					imprime_lista(aux, id);
				}
				lista_prefixo(aux, id, pre);
			}
		}
	}
}

void imprime_lista(arvore *trie, char *id){
	lista_sinonimo *q;
	
	printf("%s : ", trie->palavra);
	printf("%s", trie->primeiro_conceito->sinonimo);
	for(q = trie->primeiro_conceito->prox_sin; q != NULL; q = q->prox_sin)
		printf(", %s", q->sinonimo);
	printf("\n");
}

int filhos(arvore *trie){
	int i;
	
	for(i=0; i < alfa_size; i++){
		if(trie->filho[i] != NULL)
			return 1;
	}
	return 0;
}

void libera_sinonimo(lista_sinonimo *lst){
	lista_sinonimo *p = lst;
	lista_sinonimo *t;
	
	while (p != NULL){ /*enquanto a lista não estiver vazia eu faço:*/
		t = p->prox_sin; /*salvo o proximo elemento da lista*/
		free(p); /*libero o espaço do primeiro elemento*/
		p = t; /*vou para o proximo elemento*/
	}
}

void libera(arvore* trie){
	int i;   
	for(i=0; i<alfa_size; i++){
		if (trie->filho[i] != NULL)
			libera(trie->filho[i]);
	}
	free(trie);
}
