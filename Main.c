#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct TSNode {
	char rotulo[51];
	int valor;
	struct TSNode* proximo;
};

struct InstructNode{
	int opcode;
	struct TSNode* op1;
	struct TSNode* op2;
	struct InstructNode* proximo;
};

void criaTSNode(struct TSNode* tsnodeHeader, char* leitura, int valor) {
	struct TSNode* percorre = tsnodeHeader;
	if (percorre->proximo==NULL && percorre->rotulo[0]==NULL && percorre->valor==0){
		percorre->valor = valor;
		strcpy(percorre->rotulo, leitura);
	}
	else {
		while (percorre->proximo != NULL) {
			percorre = percorre->proximo;
		}
		percorre->proximo = (struct TSNode*)malloc(sizeof(struct TSNode*));
		percorre = percorre->proximo;
		percorre->proximo = NULL;
		percorre->valor = valor;
		strcpy(percorre->rotulo, leitura);
	}
}

void criaInstructNode(struct InstructNode* instructnodeHeader, int opcode, struct TSNode* op1, struct TSNode* op2) {
	struct InstructNode* percorre = instructnodeHeader;

	if (percorre->proximo==NULL && percorre->opcode==0&&percorre->op1==0&&percorre->op2==0){
		percorre->opcode = opcode;
		percorre->op1 = op1;
		percorre->op2 = op2;
	}
	else {
		while (percorre->proximo != NULL) {
			percorre = percorre->proximo;
		}
		percorre->proximo = (struct InstructNode*)malloc(sizeof(struct InstructNode*));
		percorre = percorre->proximo;
		percorre->opcode = opcode;
		percorre->op1 = op1;
		percorre->op2 = op2;
		percorre->proximo = NULL;
	}
}

void analise(char* leitura, FILE* Entrada, FILE* Saida,struct InstructNode* instructnodeHeader, struct TSNode* tsnodeHeader){
	char leitura2[51];

	if (strcmp(leitura, "ADD") == 0) {
		criaInstructNode(instructnodeHeader, 1, NULL, NULL);
	}
	if (strcmp(leitura, "SUB") == 0) {
		criaInstructNode(instructnodeHeader, 2, NULL, NULL);
	}
	if (strcmp(leitura, "MUL") == 0) {
		criaInstructNode(instructnodeHeader, 3, NULL, NULL);
	}
	if (strcmp(leitura, "DIV") == 0) {
		criaInstructNode(instructnodeHeader, 4, NULL, NULL);
	}
	if (strcmp(leitura, "JMP") == 0){
		criaInstructNode(instructnodeHeader, 5, NULL, NULL);
	}
	if (strcmp(leitura, "JMPN") == 0) {
		criaInstructNode(instructnodeHeader, 6, NULL, NULL);
	}
	if (strcmp(leitura, "JMPP") == 0) {
		criaInstructNode(instructnodeHeader, 7, NULL, NULL);
	}
	if (strcmp(leitura, "JMPZ") == 0) {
		criaInstructNode(instructnodeHeader, 8, NULL, NULL);
	}
	if (strcmp(leitura, "COPY") == 0) {
		criaInstructNode(instructnodeHeader, 9, NULL, NULL);
	}
	if (strcmp(leitura, "LOAD") == 0) {
		criaInstructNode(instructnodeHeader, 10, NULL, NULL);
	}
	if (strcmp(leitura, "STORE") == 0) {
		criaInstructNode(instructnodeHeader, 11, NULL, NULL);
	}
	if (strcmp(leitura, "INPUT") == 0) {
		criaInstructNode(instructnodeHeader, 12, NULL, NULL);
	}
	if (strcmp(leitura, "OUTPUT") == 0) {
		criaInstructNode(instructnodeHeader, 13, NULL, NULL);
	}
	if (strcmp(leitura, "STOP") == 0) {
		criaInstructNode(instructnodeHeader, 14, NULL, NULL);
	}
	else {
		if (leitura[strlen(leitura) - 1] == ':') {
			criaTSNode(tsnodeHeader, leitura, 0);
			fscanf(Entrada, "%s", leitura);
			if (strcmp(leitura2, "SPACE") == 0) {
				criaTSNode(tsnodeHeader, leitura, 0);
			}
			if (strcmp(leitura2, "CONST") == 0) {
				fscanf(Entrada, "%s", leitura2);
				criaTSNode(tsnodeHeader, leitura, atoi(leitura2));
			}
		}
		else {
			fscanf(Entrada, "%s", leitura2);
			if (strcmp(leitura2, ":") == 0) {
				fscanf(Entrada, "%s", leitura2);
				if (strcmp(leitura2, "SPACE") == 0) {
					criaTSNode(tsnodeHeader, leitura, 0);
				}
				if (strcmp(leitura2, "CONST") == 0) {
					fscanf(Entrada, "%s", leitura2);
					criaTSNode(tsnodeHeader, leitura, atoi(leitura2));
				}
			}
		}
	}
	/*if (strcmp(leitura, "ADD") == 0) {
		fprintf(Saida, "01 ");
	}
	if (strcmp(leitura, "SUB") == 0) {
		fprintf(Saida, "02 ");
	}
	if (strcmp(leitura, "MUL") == 0) {
		fprintf(Saida, "03 ");
	}
	if (strcmp(leitura, "DIV") == 0) {
		fprintf(Saida, "04 ");
	}
	if (strcmp(leitura,"COPY")==0){
		fprintf(Saida, "05 ");
	}
	if (strcmp(leitura, "JMPN") == 0) {
		fprintf(Saida, "06 ");
	}
	if (strcmp(leitura, "JMPP") == 0) {
		fprintf(Saida, "07 ");
	}
	if (strcmp(leitura, "JMPZ") == 0) {
		fprintf(Saida, "08 ");
	}
	if (strcmp(leitura, "LOAD") == 0) {
		fprintf(Saida, "10 ");
	}
	if (strcmp(leitura, "STORE") == 0) {
		fprintf(Saida, "11 ");
	}
	if (strcmp(leitura, "INPUT") == 0) {
		fprintf(Saida, "12 ");
	}
	if (strcmp(leitura, "OUTPUT") == 0) {
		fprintf(Saida, "13 ");
	}
	if (strcmp(leitura, "STOP")==0){
		fprintf(Saida, "14 ");
	}
	if (strcmp(leitura, "SPACE") == 0) {
		fprintf(Saida, "00 ");
	}
	if (strcmp(leitura, "CONST") == 0) {
		fscanf(Entrada, "%s", leitura);
		fprintf(Saida, "%s ", leitura);
	}*/
	if (leitura[0] == ';') {
		while (fgetc(Entrada) != '\n' && fgetc(Entrada) != EOF);
	}
}


void main() {
	FILE *Entrada, *Saida;
	char leitura[51];
	int i;
	struct InstructNode *instructnodeHeader = (struct InstructNode*)malloc(sizeof(struct InstructNode*)), *percorreinstruct;
	struct TSNode *tsnodeHeader = (struct TSNode*)malloc(sizeof(struct TSNode*)), *percorreTS;

	instructnodeHeader->proximo = NULL;
	instructnodeHeader->opcode = 0;
	instructnodeHeader->op1 = 0;
	instructnodeHeader->op2 = 0;
	tsnodeHeader->proximo = NULL;
	tsnodeHeader->valor = 0;
	tsnodeHeader->rotulo[0] = NULL;
	Entrada = fopen("test.txt", "r");
	Saida = fopen("test.asm", "w");

	while (fscanf(Entrada, "%s", leitura) != EOF){
		for (i = 0; i < strlen(leitura); i++){
			leitura[i] = toupper(leitura[i]);
		}
		analise(leitura, Entrada, Saida, instructnodeHeader, tsnodeHeader);
	}
	printf("TS:\n");
	percorreTS = tsnodeHeader;
	while (percorreTS->proximo != NULL) {
		printf("%s %d\n", percorreTS->rotulo, percorreTS->valor);
		percorreTS = percorreTS->proximo;
	}
	printf("%s %d\n", percorreTS->rotulo, percorreTS->valor);
	printf("opcodes:\n");
	percorreinstruct = instructnodeHeader;
	while (percorreinstruct->proximo != NULL) {
		printf("%d\n", percorreinstruct->opcode);
		percorreinstruct = percorreinstruct->proximo;
	}
	printf("%d\n", percorreinstruct->opcode);
	fclose(Entrada);
	fclose(Saida);
}