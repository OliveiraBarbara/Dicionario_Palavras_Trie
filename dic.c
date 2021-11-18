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
			nova->filho[i] = NULL; /*inicializo todas as posicoes do noh com nulo*/
		nova->folha = 0; /*nenhum noh eh folha quando inicializado*/
	}else /*se a alocacao nao deu certo, informo na tela*/
		printf("Erro ao alocar memória\n");
	
	return nova;
}

void insere(arvore* trie, char* str, char *id, char *sin){
	int i, ascii;
	lista_sinonimo *aux, *nova;
	
	for (i=0; str[i] != '\0'; i++) { /*enquanto não chegar ao final da palavra*/
		ascii = str[i] - 97; /*calculo o valor da letra e faco menos o valor da letra 'a' para achar a posicao que sera inserido*/
		if (trie->filho[ascii] == NULL) /*se a posicao tiver nula, eu inicializo*/
			trie->filho[ascii] = aloca(); 
		trie = trie->filho[ascii]; /*a arvore recebe o noh*/
	}
	trie->folha = 1; /*quando chegar ao final da palavra eu marco o noh como folha*/
	strcpy(trie->palavra, str); /*insiro a palavra*/
	strcpy(trie->idioma, id); /*insiro o idioma*/
	
	if(trie->primeiro_conceito == NULL){ /*faco a alocacao do sinonimo ja em ordem alfabetica*/
		aux = aloca_sinonimo(sin);
		aux->prox_sin = trie->primeiro_conceito;
		trie->primeiro_conceito = aux;
	}else{
		if(strcmp(sin, (trie)->primeiro_conceito->sinonimo) < 0){ /*se o primeiro sinonimo for menor lexicograficamente menor que o sinonimo ja inserido, insiro o novo na primeira posicao*/
			nova = aloca_sinonimo(sin);
			nova->prox_sin = trie->primeiro_conceito;
			trie->primeiro_conceito = nova;
		}else{ /*se nao procuro o lugar que ele deve ser inserido*/
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
	
	for(i=0; str[i] != '\0'; i++){ /*enquanto não chegar ao final da palavra*/
		ascii = str[i] - 97; /*calculo o valor da letra e faco menos o valor da letra 'a' para achar a posicao que sera inserido*/
		if (aux->filho[ascii] == NULL) /*se a posicao tiver nula, eu retorno nulo*/
			return NULL;
		aux = aux->filho[ascii]; /*a arvore atualiza a posicao recebendo o filho*/
	}
	if (aux != NULL && aux->folha == 1) /*se a posicao da arvore for uma folha, eu retorno o noh, se nao retorno nulo*/
		return aux;
	return NULL;
}

void imprime_busca(arvore* trie, char* str){ /*funcao que imprime a resposta da busca*/
    arvore *aux;
    lista_sinonimo *q;
    
    aux = busca(trie, str); /*recebe a busca*/
    if (aux != NULL){ /*se nao for nula eu imprimo os sinonimos*/
        for(q = aux->primeiro_conceito; q != NULL; q = q->prox_sin)
			printf("%s\n", q->sinonimo);
    }else /*se nao imprimo a mensagem*/
        printf("hein?\n");
}

void remover(arvore *trie, char *str){
	arvore *aux = trie;
	
	if(trie != NULL){ 
		if(*str != '\0'){ /*se a palavra nao tiver acabado*/
			if(aux->filho[*str-97] != NULL){ /*e a posicao do filho for diferente de nulo*/
				if(filhos(aux) != 0){ /*verifico se posicao tem filhos*/
					if(aux->folha == 0) /*se tiver filhos, verifico se eh ou nao uma folha, se for chamo a recursao*/
						remover(aux->filho[*str-97], str+1);
				}else{
					free(aux); /*se a posicao nao tiver filhos, removo o noh*/
					aux = NULL;
				}
			}
		}else{ /*se chegou ao final da palavra*/
			if(aux->folha == 1){ /*verifico se o noh eh folha*/
				if(filhos(aux) == 0){ /*se o noh nao tiver filhos*/
					aux->folha = 0; 
					libera_sinonimo(aux->primeiro_conceito); /*excluo a lista de sinonimo*/
					aux->primeiro_conceito = NULL; /*faco o inicio da lista de sinonimos ser nula*/
					free(aux->filho[*str-97]); /*apago a posicao*/
					aux->filho[*str-97] = NULL;
				}else{ /*se tiver filhos*/
					aux->folha = 0;  /*faco o no nao ser folha*/
					strcpy(aux->palavra, ""); /*apago a palavra e o idioma*/
					strcpy(aux->idioma, "");
					libera_sinonimo(aux->primeiro_conceito); /*excluo a lista de sinonimos*/
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
		aux = busca(trie, str1); /*primeiro busco o noh que quero excluir o sinonimo*/
		
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
		if(q != NULL && q->primeiro_conceito == NULL) /*verifico se a palavra ficou sem sinonimo, se ficou eu chamo a funcao para remover ela*/
			remover(trie, str1);
	}
}

void lista_prefixo(arvore *trie, char *id, char *pre){ /*funcao para procurar as palavras com determinado prefixo*/
	int i, tam;
	arvore *aux;
	
	if(trie != NULL){
		for(i=0; i<alfa_size; i++){ /*percorro todo o alfabeto*/
			aux = trie->filho[i]; /*faco o no receber o filho*/
			if(aux != NULL){ /*se for diferente de nulo*/
				tam = strlen(pre); /*verifico o tamanho do prefixo*/
				if(aux->folha == 1 && strncmp(aux->palavra, pre, tam) == 0 && strcmp(id, aux->idioma) == 0){ /*se o noh for folha, tiver o prefixo e for do idioma requerido, eu imprimo*/
					imprime_lista(aux, id);
				}
				lista_prefixo(aux, id, pre); /*chamo a recursao para procurar mais palavras*/
			}
		}
	}
}

void imprime_lista(arvore *trie, char *id){ /*funcao para imprimir a palavra com seu sinonimo*/
	lista_sinonimo *q;
	
	printf("%s : ", trie->palavra); /*imprimo a palavra*/
	printf("%s", trie->primeiro_conceito->sinonimo); /*e toda a lista de sinonimos*/
	for(q = trie->primeiro_conceito->prox_sin; q != NULL; q = q->prox_sin)
		printf(", %s", q->sinonimo);
	printf("\n");
}

int filhos(arvore *trie){ /*funcao para verificar se o noh possui filhos*/
	int i;
	
	for(i=0; i < alfa_size; i++){
		if(trie->filho[i] != NULL)
			return 1; /*se tiver filho, retorno 1*/
	}
	return 0; /*ou entao retorno 0, caso nao tenha filhos*/
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

void libera(arvore* trie){ /*funcao para desalocar a arvore*/
	int i;   
	for(i=0; i<alfa_size; i++){ /*acontece de maneira recursiva, indo ate os filhos dos nos e depois excluindo*/
		if (trie->filho[i] != NULL)
			libera(trie->filho[i]);
	}
	free(trie);
}
