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

int main(void) {
    int i, j, k; /*Variáveis contadoras usadas na separação da string para vetor de palavras.*/
	char acao[MAX], string[MAX], str[3][MAX], str1[MAX], str2[MAX], id1[3], id2[3], pre[MAX];/*Acao: Variavel utilizada para ler qual operacao realizarei no processo // String: utilizada para ler as palavras que vai remover ou listar // Vetor Str: utilizada para separa a string de palavras com espaço, cada uma em uma posição do vetor // Str1 e Str2: usadas para ler as palavras que vou inserir ou buscar // Id1 e Id2: usadas para ler o idioma. // Vetor id: usado para armazenar o idioma e as letras no comando listar // Pre: variavel que le qual o prefixo que quero procurar as palavras*/
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
			
			lista_prefixo(trie, id1, pre);
		}
			
		scanf("%s", acao); /*Leio novamente a acao para definir qual a proxima operacao a ser realizada*/
	}

    libera(trie);
    return 0;
}
