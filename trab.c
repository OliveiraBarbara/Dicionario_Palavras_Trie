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
#define alfa_size 26

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

arvore* aloca() {
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

void insere(arvore* trie, char* str, char *id, char *sin) {
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

void imprime_busca(arvore* trie, char* str) {
    arvore *aux;
    lista_sinonimo *q;
    
    aux = busca(trie, str);
    if (aux != NULL){
        for(q = aux->primeiro_conceito; q != NULL; q = q->prox_sin)
			printf("%s\n", q->sinonimo);
    }else
        printf("hein?\n");
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

void libera(arvore* trie) {
	int i;   
	for(i=0; i<alfa_size; i++){
		if (trie->filho[i] != NULL)
			libera(trie->filho[i]);
	}
	free(trie);
}

void remover(arvore *trie, char *str){
	arvore *aux = trie;
	
	if(trie != NULL){
		if(*str != '\0'){
			if(aux->filho[*str-97] != NULL){
				if(filhos(aux) != 0){
					if(aux->folha == 0){
						remover(aux->filho[*str-97], str+1);
						printf("aqui2\n");
					}else{
						aux->filho[*str-97] = NULL;
						printf("aqui1\n");
					}
				}else{
					free(aux);
					aux = NULL;
					printf("aqui4\n");
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
					printf("aqui\n");
				}else{
					aux->folha = 0;
					strcpy(aux->palavra, "");
					strcpy(aux->idioma, "");
					libera_sinonimo(aux->primeiro_conceito);
					printf("aqui3\n");
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
			printf("entrei\n");
		}
	}
}

/*void lista_pre(arvore *trie, char *id, char *pre){
	arvore* aux = trie;
	lista_sinonimo *q;
	int i, ascii;
	
	for(i=0; pre[i] != '\0'; i++){
		ascii = pre[i] - 97;
		if(aux->filho[ascii] == NULL)
			printf("\n");
		aux = aux->filho[ascii];
	}
	
	if(aux != NULL && filhos(aux) == 1){
		for(i=0; i<alfa_size; i++){
			aux = aux->filho[i];
			if(filhos(aux->filho[i]) == 1){
				if(strcmp(id, aux->idioma) == 0){
					printf("%s : ", aux->palavra);
					printf("%s", aux->primeiro_conceito->sinonimo);
					for(q = aux->primeiro_conceito->prox_sin; q != NULL; q = q->prox_sin)
						printf(", %s", q->sinonimo);
					printf("\n");
				}
			}
		}
	}
}*/

int main(void) {
    int i, j, k; /*Variáveis contadoras usadas na separação da string para vetor de palavras.*/
	char acao[MAX], string[MAX], str[3][MAX], str1[MAX], str2[MAX], id1[3], id2[3], pre[MAX];/*Acao: Variavel utilizada para ler qual operacao realizarei no processo // String: utilizada para ler as palavras que vai remover ou listar // Vetor Str: utilizada para separa a string de palavras com espaço, cada uma em uma posição do vetor // Str1 e Str2: usadas para ler as palavras que vou inserir ou buscar // Id1 e Id2: usadas para ler o idioma. // Vetor id: usado para armazenar o idioma e as letras no comando listar*/
    arvore* trie = aloca();
    
    scanf("%s", acao); /*leio o qual opção devo realizar*/

	/*While para continuar fazendo processos enquanto acao for diferente de fim*/
	while(strcmp("fim", acao)!= 0){
	
		/*Se for o comando insere, eu leio as palavras e  idioma e chamo a função para inserir na arvore*/
		if(strcmp("insere", acao) == 0){
			scanf("%s", id1);
			scanf("%s", str1);
			scanf("%s", id2);
			scanf("%s", str2);
			
			insere(trie, str1, id1, str2);
			insere(trie, str2, id2, str1);
		}
		
		/*Se for o comando busca eu leio a palavra que devo buscar e chamo a função.*/
		if(strcmp("busca", acao) == 0){
			scanf("%s", str1);
			imprime_busca(trie, str1);
		}
		
		/*Se for o comando remove eu leio a/as palavras que devo remover e chamo a função.*/
		if(strcmp("remove", acao) == 0){
			scanf("%[^\n]s", string);
			j=0, k=0;
			
			/*Neste trecho de código eu separo as palavras e armazeno em um vetor de string, ou seja, leio letra a letra e passo para o vetor, até eu encontrar o espaço em branco, quando encontro mudo a posição do vetor*/
			for(i = 0; i < strlen(string); i++){
				if(string[i] != ' '){
					str[j][k] = string[i];
					k++;
				}else{
					str[j][k] = '\0';
					j++;
					k = 0;
				}
			}
			
			/*Verifico se o vetor tem 1 ou 2 palavras e chamo o remover correspondente*/
			if(j == 1){
				str[1][k] = '\0'; /*insiro o caracter nulo para saber onde a palavra acaba*/
				remover(trie, str[1]);
			}else{
				str[2][k] = '\0'; /*insiro o caracter nulo para saber onde a palavra acaba*/
				remove_sinonimo(trie, str[1], str[2]);
				remove_sinonimo(trie, str[2], str[1]);
			}
		}
		
		/*Se for o comando lista eu leio o idioma (e letras do intervalo).*/
		if(strcmp("lista", acao) == 0){
			scanf("%s", id1);
			scanf("%s", pre);
			
			/*lista_pre(trie, id1, pre);*/
		}
			
		scanf("%s", acao); /*Leio novamente a acao para definir qual a proxima operacao a ser realizada*/
	}

    libera(trie);
    return 0;
}
