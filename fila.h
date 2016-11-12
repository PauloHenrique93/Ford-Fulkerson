#include <stdafx.h>
#include <stdio.h>
#include <stdlib.h>

struct Node{
	int num;
	struct Node *prox;
};
typedef struct Node node;

int tam;

int menu(void);
void opcao(int op);
void inicia();
int vazia();
node *aloca(int num);
void insere(int num);
int retira();
void exibe();
void libera();

node *FILA = (node *)malloc(sizeof(node));

int main(void)
{

	if (!FILA){
		printf("Sem memoria disponivel!\n");
		exit(1);
	}
	else{
		inicia();
		int opt;

		do{
			opt = menu();
			opcao(opt);
		} while (opt);

		free(FILA);
		return 0;
	}
}


int menu(void)
{
	int opt;

	printf("Escolha a opcao\n");
	printf("0. Sair\n");
	printf("1. Zerar fila\n");
	printf("2. Exibir fila\n");
	printf("3. Adicionar Elemento na Fila\n");
	printf("4. Retirar Elemento da Fila\n");
	printf("Opcao: "); scanf("%d", &opt);

	return opt;
}

void opcao(int op)
{
	int valor;
	switch (op){
	case 0:
		libera();
		break;

	case 1:
		libera();
		inicia();
		break;

	case 2:
		exibe();
		break;

	case 3:
		for (int t = 5; t < 10; t++)
			insere(t);
		break;

	case 4:
		valor = retira();
		printf("Retirado: %3d\n\n", valor);
		break;

	default:
		printf("Comando invalido\n\n");
	}
}

void inicia()
{
	FILA->prox = NULL;
	tam = 0;
}

int vazia()
{
	if (FILA->prox == NULL)
		return 1;
	else
		return 0;
}

node *aloca(int num)
{
	node *novo = (node *)malloc(sizeof(node));

	novo->num = num;

	return novo;

}

void insere(int num)
{
	node *novo = aloca(num);
	novo->prox = NULL;

	if (vazia())
		FILA->prox = novo;
	else{
		node *tmp = FILA->prox;

		while (tmp->prox != NULL)
			tmp = tmp->prox;

		tmp->prox = novo;
	}
	tam++;
}


int retira()
{
	if (FILA->prox == NULL){
		printf("Fila ja esta vazia\n");
		return NULL;
	}
	else{
		node *tmp = FILA->prox;
		FILA->prox = tmp->prox;
		tmp->prox = NULL;
		free(tmp->prox);
		tam--;

		return tmp->num;
	}

}


void exibe()
{
	if (vazia()){
		printf("Fila vazia!\n\n");
		return;
	}

	node *tmp;
	tmp = FILA->prox;
	printf("Fila :");
	while (tmp != NULL){
		printf("%5d", tmp->num);
		tmp = tmp->prox;
	}
	printf("\n        ");
	int count;
	for (count = 0; count < tam; count++)
		printf("  ^  ");
	printf("\nOrdem:");
	for (count = 0; count < tam; count++)
		printf("%5d", count + 1);


	printf("\n\n");
}

void libera()
{
	if (!vazia()){
		node *proxNode,
			*atual;

		atual = FILA->prox;
		while (atual != NULL){
			proxNode = atual->prox;
			free(atual);
			atual = proxNode;
		}
	}
}