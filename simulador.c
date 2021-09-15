#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>

struct InstructNode{
	int opcode;
	int op1;
	int op2;
	int contador;
	int linha;
	struct InstructNode *proximo;
};

struct SimbolNode{
	int valor;
	int contador;
	struct SimbolNode *proximo;
};

struct InstructNode* criarinstruct(struct InstructNode* instructnodeHeader, int opcode, int op1, int op2, int contador, int linha){
	struct InstructNode *percorre;
	percorre = instructnodeHeader;
	if(percorre->proximo == NULL && percorre->opcode == -1){
		percorre->opcode = opcode;
		percorre->op1 = op1;
		percorre->op2 = op2;
		percorre->contador = contador;
		percorre->linha = linha;
		percorre->proximo = NULL;
	}
	else{
		while(percorre->proximo != NULL){
			percorre = percorre->proximo;
		}
		percorre->proximo = (struct InstructNode*)malloc(sizeof(struct InstructNode));
		percorre = percorre->proximo;
		percorre->opcode = opcode;
		percorre->op1 = op1;
		percorre->op2 = op2;
		percorre->contador = contador;
		percorre->linha = linha;
		percorre->proximo = NULL;
	}
	return percorre;
}

struct SimbolNode* criasimbol(struct SimbolNode* simbolnodeHeader, int valor, int contador){
	struct SimbolNode *percorre;

	percorre = simbolnodeHeader;
	if(percorre->proximo == NULL && percorre->valor == -1){
		percorre->valor = valor;
		percorre->contador = contador;
		percorre->proximo = NULL;
	}
	else{
		while(percorre->proximo != NULL){
			percorre = percorre->proximo;
		}
		percorre->proximo = (struct SimbolNode*)malloc(sizeof(struct SimbolNode));
		percorre = percorre->proximo;
		percorre->valor = valor;
		percorre->contador = contador;
		percorre->proximo = NULL;
	}
	return percorre;
}

int main(int argc, char const *argv[]){
	FILE *entrada;
	struct InstructNode *instructnodeHeader = (struct InstructNode*)malloc(sizeof(struct InstructNode)), *percorrei;
	struct SimbolNode *simbolnodeHeader = (struct SimbolNode*)malloc(sizeof(struct SimbolNode)), *percorres;
	int opcode=0, op1=0, op2=0, contador=0, tamanho = 0, i = 0, acc=0, linha = 2;


	instructnodeHeader->proximo = NULL;
	instructnodeHeader->opcode = -1;
	simbolnodeHeader->proximo = NULL;
	simbolnodeHeader->valor = -1;

	entrada = fopen(argv[1], "r");
	while(opcode != 14){
		fscanf(entrada, "%d", &opcode);
		switch(opcode){
			case 1:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador, linha);
				contador = contador+2;
				linha++;
				break;

			case 2:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador, linha);
				contador = contador+2;
				linha++;
				break;
				
			case 3:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador, linha);
				contador = contador+2;	
				linha++;
				break;
				
			case 4:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador, linha);
				contador = contador+2;
				linha++;
				break;
				
			case 5:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador, linha);
				contador = contador+2;
				linha++;
				break;
				
			case 6:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador, linha);
				contador = contador+2;
				linha++;
				break;
					
			case 7:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador, linha);
				contador = contador+2;
				linha++;
				break;

			case 8:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador, linha);
				contador = contador+2;
				linha++;
				break;
				
			case 9:
				fscanf(entrada, "%d", &op1);
				fscanf(entrada, "%d", &op2);
				criarinstruct(instructnodeHeader, opcode, op1, op2, contador,linha);
				contador = contador+3;
				linha++;
				break;

			case 10:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador,linha);
				contador = contador+2;
				linha++;
				break;
				
			case 11:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador,linha);
				contador = contador+2;
				linha++;
				break;
				
			case 12:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador,linha);
				contador = contador+2;
				linha++;
				break;
				
			case 13:
				fscanf(entrada, "%d", &op1);
				criarinstruct(instructnodeHeader, opcode, op1, -1, contador,linha);
				contador = contador+2;
				linha++;
				break;

			case 14:
				criarinstruct(instructnodeHeader, opcode, -1, -1, contador,linha);
				contador = contador+1;
				linha++;
				break;
		}
	}
	tamanho = contador;
	while(fscanf(entrada, "%d", &opcode) != EOF){
		criasimbol(simbolnodeHeader, opcode, contador);
		contador++;
	}
	percorrei = instructnodeHeader;
	while(percorrei->opcode != 14){
		percorres = simbolnodeHeader;
		switch(percorrei->opcode){
			case 1:
				for (i = 0; i < (percorrei->op1-tamanho); ++i){
					percorres = percorres->proximo;
				}
				acc = acc + percorres->valor;
				printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
				percorrei=percorrei->proximo;
				break;

			case 2:
				for (i = 0; i < (percorrei->op1-tamanho); ++i){
					percorres = percorres->proximo;
				}
				acc = acc - percorres->valor;
				printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
				percorrei=percorrei->proximo;
				break;
				
			case 3:
				for (i = 0; i < (percorrei->op1-tamanho); ++i){
					percorres = percorres->proximo;
				}
				acc = acc * percorres->valor;
				printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
				percorrei=percorrei->proximo;
				break;
				
			case 4:
				for (i = 0; i < (percorrei->op1-tamanho); ++i){
					percorres = percorres->proximo;
				}
				acc = acc / percorres->valor;
				printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
				percorrei=percorrei->proximo;
				break;
				
			case 5:
				op1 = percorrei->op1;
				percorrei = instructnodeHeader;
				i=0;
				while(i<op1){
					if(percorrei->opcode!= 9){
						i=i+2;
					}
					else{
						i=i+3;
					}
					printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
					percorrei = percorrei->proximo;
				}
				break;
				
			case 6:
				if(acc<0){
					op1 = percorrei->op1;
					percorrei = instructnodeHeader;
					i=0;
					while(i<op1){
						if(percorrei->opcode!= 9){
							i=i+2;
						}
						else{
							i=i+3;
						}
						printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
						percorrei = percorrei->proximo;
					}
				}
				else{
					printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
					percorrei = percorrei->proximo;
				}
				break;
					
			case 7:
				if(acc>0){
					op1 = percorrei->op1;
					printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
					percorrei = instructnodeHeader;
					i=0;
					while(i<op1){
						if(percorrei->opcode!= 9){
							i=i+2;
						}
						else{
							i=i+3;
						}
						percorrei = percorrei->proximo;
					}
				}
				else{
					printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
					percorrei = percorrei->proximo;
				}
				break;

			case 8:
				if(acc==0){
					op1 = percorrei->op1;
					percorrei = instructnodeHeader;
					i=0;
					while(i<op1){
						if(percorrei->opcode!= 9){
							i=i+2;
						}
						else{
							i=i+3;
						}
						printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
						percorrei = percorrei->proximo;
					}
				}
				else{
					printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
					percorrei = percorrei->proximo;
				}
				break;
				
			case 9:
				for (i = 0; i < (percorrei->op1-tamanho); ++i){
					percorres = percorres->proximo;
				}
				op1 = percorres->valor;
				percorres=simbolnodeHeader;
				for (i = 0; i < (percorrei->op2-tamanho); ++i){
					percorres = percorres->proximo;
				}
				percorres->valor = op1;
				printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
				percorrei=percorrei->proximo;
				break;

			case 10:
				for (i = 0; i < (percorrei->op1-tamanho); ++i){
					percorres = percorres->proximo;
				}
				acc = percorres->valor;
				printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
				percorrei=percorrei->proximo;
				break;
				
			case 11:
				for (i = 0; i < (percorrei->op1-tamanho); ++i){
					percorres = percorres->proximo;
				}
				percorres->valor = acc;
				printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
				percorrei=percorrei->proximo;
				break;
				
			case 12:
				for (i = 0; i < (percorrei->op1-tamanho); ++i){
					percorres = percorres->proximo;
				}
				scanf("%d", &(percorres->valor));
				while(getchar() != '\n');
				printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
				percorrei=percorrei->proximo;
				break;
				
			case 13:
				for (i = 0; i < (percorrei->op1-tamanho); ++i){
					percorres = percorres->proximo;
				}
				printf("%d\n", percorres->valor);
				while(getchar() != '\n');
				printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
				percorrei=percorrei->proximo;
				break;
			}	
	}
	printf("linha = %d acc = %d contador = %d \n", percorrei->linha, acc, percorrei->contador);
	return 0;
}