// AlgoritmoPrim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct VERTICE
{
	char nome[50];
	int referencia;
	char status; //V - encontrado, F - não econtrado
}VERTICE;

typedef struct ARESTA
{
	int origem;
	int destino;
	float capacidade;
	int melhoria;
}ARESTA;

//========================PILHA==========================
struct Node{
	int num;
	struct Node *prox;
};
typedef struct Node node;

int menu(void);
void opcao(int op);
void inicia();
int vazia();
node *aloca(int num);
void insere(int num);
int retira();
void exibe();
void libera();


//==========================================ASSINATURA DAS FUNÇÕES =====================================================
void lerArquivoMatriz();
void lerArquivoVertice();;
void imprimirVertice();
void imprimirMatriz();
void realizarCopia();
int  fordFulkerson();
void desperdicio();
void imprimirGrafoResidual();
void arestasFinal();

//VAR GLOBAIS PILHA
int tam;
node *FILA = (node *)malloc(sizeof(node));

//VAR GLOBAIS
VERTICE *vertices;
ARESTA *arestas;
int *percurso;
int *visitados;
float  **matriz;
float **grafoResidual;
int dimensaoMatriz = 0;
int inicio = 0, destino = 0;
int g = 0;//indice de arestas

int vetor[1000];//vetor que armazena os indices dos valores ja marcados

//========================================== MAIN =====================================================
int main()
{
	int i = 0;

	lerArquivoVertice();

	lerArquivoMatriz();

	grafoResidual = (float**)malloc((dimensaoMatriz)*sizeof(float*));
	for (i = 0; i < dimensaoMatriz; i++)
	{
		grafoResidual[i] = (float*)malloc((dimensaoMatriz)*sizeof(float));
	}

	percurso = (int *)malloc(sizeof(int)*dimensaoMatriz);

	visitados = (int *)malloc(sizeof(int)*dimensaoMatriz);


	arestas = (ARESTA*)malloc(sizeof(ARESTA)*(dimensaoMatriz * 2));

	for (i = 0; i < dimensaoMatriz * 2; i++)
	{
		arestas[i].capacidade = 9999;
		arestas[i].melhoria = 0;
	}

	inicia();

	//verificarDistancia();
	imprimirMatriz();
	//imprimirVertice();

	inicio = 0; destino = 4;

	int fluxoMaximo = fordFulkerson();

	imprimirGrafoResidual();
	

	
	printf("\nFLUXO MAXIMO: %d", fluxoMaximo);

	printf("\n\n");

	arestasFinal();

	printf("\n\n");

	desperdicio();

	printf("\n\n");

	//possivelMelhoria();

	

	

	libera();
	printf("\n\n\n");
	system("PAUSE");
	return 0;
}

//========================================== LER ARQUIVO MATRIZ ===========================================

void lerArquivoMatriz()
{
	float distancia;
	FILE *fp;
	char caractere;
	char linha[70];
	int i = 0, j = 0;

	matriz = (float**)malloc((dimensaoMatriz)*sizeof(float*));
	for (i = 0; i < dimensaoMatriz; i++)
	{
		matriz[i] = (float*)malloc((dimensaoMatriz)*sizeof(float));
	}


	fp = fopen("avaliacaoMatriz.txt", "r");

	for (i = 0; i < dimensaoMatriz; i++)
	{
		for (j = 0; j < dimensaoMatriz; j++)
		{
			fscanf(fp, "%f", &distancia);
			/*if (distancia == 0)
				distancia = 999.00;*/
			matriz[i][j] = distancia;
			fgetc(fp);
		}
	}

	fclose(fp);
}

//========================================== IMPRIMIR MATRIZ =====================================================

void imprimirMatriz(){
	int i = 0, j = 0;

	for (i; i < dimensaoMatriz; i++){
		for (j; j < dimensaoMatriz; j++){
			printf("%.2f ", matriz[i][j]); 
		}
		j = 0;
		printf("\n");
	}
}

//========================================== LER ARQUIVO VERTICE=====================================================

void lerArquivoVertice(){

	FILE *fp;
	char nome[50];
	int indice = 0;
	int quantLinha = 0;

	fp = fopen("avaliacaoVertices.txt", "r");

	while (!feof(fp))
	{
		fgets(nome, 50, fp);
		quantLinha++;
	}

	fclose(fp);

	dimensaoMatriz = quantLinha;

	vertices = (VERTICE*)malloc(sizeof(VERTICE)*(dimensaoMatriz));

	fp = fopen("avaliacaoVertices.txt", "r");

	while (!feof(fp))
	{
		fgets(nome, 50, fp);

		//retirando o '\n'
		for (int c = 0; c < strlen(nome); c++){
			if (nome[c] == '\n')
				nome[c] = '\0';
		}

		strcpy(vertices[indice].nome, nome);
		vertices[indice].referencia = indice;
		vertices[indice].status = 'F';
		indice++;
	}

	fclose(fp);

}

//========================================= IMPRIMIR VERTICE =====================================================

void imprimirVertice(){
	int i = 0;

	for (i; i < dimensaoMatriz; i++)
	{
		if (i % 3 == 0){
			printf("\n");
			printf("%d - %s ", i, vertices[i].nome);
		}
		else
			printf("                       %d - %s ", i, vertices[i].nome);

	}
}



//faz uma copia da matriz original para um grafo residual
void realizarCopia(){
	int fluxoMaximo = 0;

	//Faz uma cópia da matriz do grafo no grafo residual
	for (int i = 0; i < dimensaoMatriz; i++){

		for (int j = 0; j < dimensaoMatriz; j++){

			grafoResidual[i][j] = matriz[i][j];

		}
	}
}


int buscaCaminho()
{
	int existeCaminho = 0;

	//"reseto" as variáveis
	for (int v = 0; v < dimensaoMatriz; v++){
		percurso[v] = -1;
		visitados[v] = 0;
	}

	insere(inicio); //Adiciono o vértice inicial na fila (FIFO)
	
	visitados[inicio] = 1;

	
	while (vazia() == 0){ //Enquanto a fila não estiver vazia...

		int vertice = retira(); //tira o primeiro vértice adicionado
		

		for (int i = 0; i < dimensaoMatriz; i++){
			//Se existir uma ligação entre os vértices, a capacidade for maior que zero e ainda não foi visitado
			if (grafoResidual[vertice][i] != 999 && grafoResidual[vertice][i] > 0 && visitados[i] == 0){
				percurso[i] = vertice; //Adiciono ele no vetor que guarda o caminho
				insere(i); //Adiciono o vértice no final da fila
				visitados[i] = 1; //Marco ele como visitado
			}

		}

	}
	if (visitados[destino] == 1){ //Verifica se um caminho até o destino foi formado
		existeCaminho = 1;
		g++;//indice do caminho(usado para saber a qual caminho pertence a aresta que pode ser otimitizada)

	}

	return existeCaminho;

}

//================================FORD FULKERSON ======================================================

int fordFulkerson(){
	int fluxoMaximo = 0;

	realizarCopia();
	printf("\n\n");
	//Busca um caminho até que todos os caminhos sejam percorridos
	int y = 0;
	int melhoria = 0, s = 0, d = 0;
	while (buscaCaminho())
	{
		int fluxo = 999; //Seta um valor suficiente grande para iniciar a variável
		int i = 0;

		//Percorre o caminho e salva a aresta de menor capacidade para definir o valor do fluxo
		printf("======================================= CAMINHO %d ====================================\n", y);
		melhoria = 0;
		for (int v = destino; v != inicio; v = percurso[v])
		{
			i = percurso[v];
			printf("\n%s <-----------> %s", vertices[i].nome, vertices[v].nome);
			if (grafoResidual[i][v] < fluxo){
				fluxo = grafoResidual[i][v];
			}

		}
		

		printf("\nfluxo: %d\n", fluxo);
		y++;

		//Atualiza os valores no grafo residual
		for (int v = destino; v != inicio; v = percurso[v])
		{
			i = percurso[v];
			grafoResidual[i][v] -= fluxo;
			//grafoResidual[v][i] += fluxo;

			if (grafoResidual[i][v] == 0){
				s = i; d = v;
				melhoria++;

			}
		}
		fluxoMaximo += fluxo;

		if (melhoria == 1){
			arestas[g].origem = s;
			arestas[g].destino = d;
			arestas[g].capacidade = grafoResidual[s][d];
			arestas[g].melhoria = 1;
		}		/*else{
			arestas[g].origem = s;
			arestas[g].destino = d;
			arestas[g].capacidade = grafoResidual[s][d];
			arestas[g].melhoria = 0;
		}*/
	}

	
	return fluxoMaximo;
	
}

void arestasFinal(){
	int i = 0;
	printf("\nPOSSIVEL MELHORIA");
	for (i = 0; i < dimensaoMatriz*2; i++){
		if (arestas[i].melhoria == 1){
			printf("\n%s <--> %s %.2f: ", vertices[arestas[i].origem].nome, vertices[arestas[i].destino].nome, arestas[i].capacidade);
		}
	}
}

void desperdicio(){
	int i = 0, j = 0;
	int d = 0;
		
	for (i = 0; i < dimensaoMatriz; i++){
		for (j = 0; j < dimensaoMatriz; j++){
			if (i == 0 && j == 0) printf("DESPERDICIO NAS SEGUINTES ARESTAS:");
			if (grafoResidual[i][j] != 0){
				printf("\n%s<-->%s desperdicio = %.2f", vertices[i].nome, vertices[j].nome, grafoResidual[i][j]);
				d++;
			}
		}
	}

	if (d == 0){
		printf("Fluxo sem desperdicio");
	}
}


void imprimirGrafoResidual(){
	int i = 0, j = 0;
	
	printf("\n\n");
	printf("GRAFO RESIDUAL\n");
	for (i; i < dimensaoMatriz; i++){
		for (j = 0; j < dimensaoMatriz; j++){
			printf("%.2f ", grafoResidual[i][j]);
		}
		j = 0;
		printf("\n");
	}
}


//=========================FUNÇÕES DA PILHA==========================================


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




















