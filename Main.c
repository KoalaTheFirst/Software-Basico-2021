#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct TSNode {
	char rotulo[51];
	int valor;
	struct InstructNode* instruct;
	struct TSNode* proximo;
};

struct InstructNode{
	int opcode; 
	int op1def;
	int op2def;
	struct TSNode* op1;
	struct TSNode* op2;
	struct InstructNode* proximo;
};

void criaTSNode(struct TSNode* tsnodeHeader, char* leitura, int valor, struct InstructNode* instruct) {
	struct TSNode* percorre = tsnodeHeader;
	
	if (percorre->proximo==NULL && percorre->valor==-1){
		
		percorre->valor = valor;
		strcpy(percorre->rotulo, leitura);
		percorre->instruct=instruct;
		percorre->proximo = NULL;
	}
	else {
		while (percorre->proximo != NULL) {
			percorre = percorre->proximo;
		}
		percorre->proximo = (struct TSNode*)malloc(sizeof(struct TSNode));
		percorre = percorre->proximo;
		percorre->proximo = NULL;
		percorre->instruct=instruct;
		percorre->valor = valor;
		strcpy(percorre->rotulo, leitura);
	}
}

struct InstructNode* criaInstructNode(struct InstructNode* instructnodeHeader, int opcode, struct TSNode* op1, struct TSNode* op2) {
	struct InstructNode* percorre = instructnodeHeader;

	if (percorre->proximo==NULL && percorre->opcode==-1&&percorre->op1==NULL&&percorre->op2==NULL){
		percorre->opcode = opcode;
		percorre->op1 = op1;
		percorre->op2 = op2;
	}
	else {
		while (percorre->proximo != NULL) {
			percorre = percorre->proximo;
		}
		percorre->proximo = (struct InstructNode*)malloc(sizeof(struct InstructNode));
		percorre = percorre->proximo;
		percorre->opcode = opcode;
		percorre->op1 = op1;
		percorre->op2 = op2;
		percorre->proximo = NULL;
	}

	if(op1==NULL){
		percorre->op1def = 0;
	}
	else{
		percorre->op1def = 1;
	}

	if(op2==NULL){
		percorre->op2def = 0;
	}
	else{
		percorre->op2def = 1;
	}

	return percorre;
}

void trata(char* leitura, FILE* Entrada){
	int i, copy=0;
	char leitura2[102];
	fpos_t position;

	if(leitura[0]==':'){
		leitura[0]='\0';
	}

	for (i = 0; i < strlen(leitura); i++){
			leitura[i] = toupper(leitura[i]);
	}
	
	if(strcmp(leitura, "ADD") != 0 && strcmp(leitura, "SUB") != 0 && strcmp(leitura, "MUL") != 0 && strcmp(leitura, "DIV") != 0 && strcmp(leitura, "JMP") != 0 && strcmp(leitura, "JMPN") != 0 && strcmp(leitura, "JMPP") != 0 && strcmp(leitura, "JMPZ") != 0 && strcmp(leitura, "COPY") != 0 && strcmp(leitura, "LOAD") != 0 && strcmp(leitura, "STORE") != 0 && strcmp(leitura, "INPUT") != 0 && strcmp(leitura, "OUTPUT") != 0 && strcmp(leitura, "STOP") != 0 && strcmp(leitura, "SECTION") != 0 && strcmp(leitura, "DATA") != 0 && strcmp(leitura, "SPACE") != 0 && strcmp(leitura, "CONST") != 0){
		fgetpos(Entrada, &position);
		fscanf(Entrada,"%s",leitura2);
		if(leitura2[0] == ':'){
			for (i = 0;leitura[i] != '\0'; ++i){}
			
			leitura[i] = ':';
			leitura[i+1] = '\0';	
		}
		fsetpos(Entrada, &position);
	}
}

struct TSNode* checasimbolo(struct TSNode* tsnodeHeader, char* leitura2, char* leitura3){
	struct TSNode *percorre=tsnodeHeader;
	int i=0,j=0;

	for (i = 0; leitura2[i] != ',' && leitura2[i] != '\0'; ++i){}

	leitura2[i] = '\0';
	i++;

	for(i=i;leitura2[i]!='\0';i++){
		leitura3[j] = leitura2[i];
		j++;
	}
	leitura3[j]='\0';

	while(percorre!= NULL && strcmp(percorre->rotulo, leitura2)!=0){
		percorre=percorre->proximo;
	}
	if(strcmp(percorre->rotulo,leitura2)==0){
		return percorre;
	}
	else{
		return NULL;
	}

}

struct InstructNode* instruct(char* leitura2, char* leitura3, FILE* Entrada, struct TSNode* tsnodeHeader, struct InstructNode* instructnodeHeader){
	struct TSNode *retornoTS, *retornoTS2;
	struct InstructNode* retornoTI;

		if (strcmp(leitura2, "ADD") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,1,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "SUB") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,2,retornoTS,NULL);
			return retornoTI;	
		}
		else if (strcmp(leitura2, "MUL") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,3,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "DIV") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,4,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "JMP") == 0){
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,5,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "JMPN") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,6,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "JMPP") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,7,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "JMPZ") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,8,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "COPY") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2, leitura3);
			retornoTS2 = checasimbolo(tsnodeHeader, leitura3, leitura2);
			retornoTI = criaInstructNode(instructnodeHeader,9,retornoTS,retornoTS2);
			return retornoTI;
		}
		else if (strcmp(leitura2, "LOAD") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,10,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "STORE") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,11,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "INPUT") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2, leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,12,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "OUTPUT") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
			retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3);
			retornoTI = criaInstructNode(instructnodeHeader,13,retornoTS,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "STOP") == 0) {
			retornoTI = criaInstructNode(instructnodeHeader,14,NULL,NULL);
			return retornoTI;
		}
		else if (strcmp(leitura2, "SECTION") == 0) {
			fscanf(Entrada, "%s", leitura2);
			trata(leitura2,Entrada);
		}
		else if (leitura2[0] == ';') {
			while (fgetc(Entrada) != '\n' && fgetc(Entrada) != EOF);
		}
	return NULL;
}

void rotulo(char* leitura, FILE* Entrada, struct InstructNode* instructnodeHeader, struct TSNode* tsnodeHeader){
	char leitura2[102], leitura3[51];
	struct InstructNode* retornoTI;

	leitura[strlen(leitura)-1] = '\0';
	
	do{
		fscanf(Entrada, "%s", leitura2);
		trata(leitura2,Entrada);
	}while(leitura2[0]=='\0');
	
	if (strcmp(leitura2, "SPACE") == 0) {
		criaTSNode(tsnodeHeader, leitura, 0,NULL);	
	}

	else if (strcmp(leitura2, "CONST") == 0) {
		trata(leitura2,Entrada);
		fscanf(Entrada, "%s", leitura2);
		criaTSNode(tsnodeHeader, leitura, atoi(leitura2), NULL);
	}
	
	else{
		retornoTI = instruct(leitura2, leitura3, Entrada, tsnodeHeader, instructnodeHeader);
		criaTSNode(tsnodeHeader,leitura,1,retornoTI);
	}

}

/*void analise(char* leitura, FILE* Entrada, FILE* Saida,struct InstructNode* instructnodeHeader, struct TSNode* tsnodeHeader){
	char leitura2[51];

	if (strcmp(leitura, "ADD") == 0) {
		criaInstructNode(instructnodeHeader, 1, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "SUB") == 0) {
		criaInstructNode(instructnodeHeader, 2, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "MUL") == 0) {
		criaInstructNode(instructnodeHeader, 3, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "DIV") == 0) {
		criaInstructNode(instructnodeHeader, 4, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "JMP") == 0){
		criaInstructNode(instructnodeHeader, 5, NULL, NULL);
	}
	else if (strcmp(leitura, "JMPN") == 0) {
		criaInstructNode(instructnodeHeader, 6, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "JMPP") == 0) {
		criaInstructNode(instructnodeHeader, 7, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "JMPZ") == 0) {
		criaInstructNode(instructnodeHeader, 8, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "COPY") == 0) {
		criaInstructNode(instructnodeHeader, 9, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "LOAD") == 0) {
		criaInstructNode(instructnodeHeader, 10, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "STORE") == 0) {
		criaInstructNode(instructnodeHeader, 11, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "INPUT") == 0) {
		criaInstructNode(instructnodeHeader, 12, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "OUTPUT") == 0) {
		criaInstructNode(instructnodeHeader, 13, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "STOP") == 0) {
		criaInstructNode(instructnodeHeader, 14, NULL, NULL);
		fscanf(Entrada, "%s", leitura2);
	}
	else if (strcmp(leitura, "SECTION") == 0) {
		fscanf(Entrada, "%s", leitura);
	}
	else if (leitura[0] == ';') {
		while (fgetc(Entrada) != '\n' && fgetc(Entrada) != EOF);
	}
	else {
		if (leitura[strlen(leitura) - 1] == ':') {
			fscanf(Entrada, "%s", leitura2);
			if (strcmp(leitura2, "SPACE") == 0) {
				criaTSNode(tsnodeHeader, leitura, 1);
				
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
					criaTSNode(tsnodeHeader, leitura, 1);
				}
				if (strcmp(leitura2, "CONST") == 0) {
					fscanf(Entrada, "%s", leitura2);
					criaTSNode(tsnodeHeader, leitura, atoi(leitura2));
				}
			}
		}
	}
}*/

void main() {
	printf("cheguei aqui");
	FILE *Entrada, *Saida;
	char leitura[51], leitura2[102];
	struct InstructNode *instructnodeHeader = (struct InstructNode*)malloc(sizeof(struct InstructNode)), *percorreinstruct;
	struct TSNode *tsnodeHeader = (struct TSNode*)malloc(sizeof(struct TSNode)), *percorreTS;

	tsnodeHeader->proximo=NULL;
	tsnodeHeader->valor=-1;
	instructnodeHeader->opcode = -1;
	instructnodeHeader->proximo = NULL;
	instructnodeHeader->op1 = NULL;
	instructnodeHeader->op2 = NULL;
	Entrada = fopen("test.txt", "r");
	Saida = fopen("test.asm", "w");

	while (fscanf(Entrada, "%s", leitura) != EOF){	
		trata(leitura, Entrada);
		if(leitura[strlen(leitura)-1] == ':'){
			rotulo(leitura, Entrada, instructnodeHeader, tsnodeHeader);
		}
		else{
			instruct(leitura, leitura2, Entrada, tsnodeHeader, instructnodeHeader);
		}
	}
	/*printf("TS:\n");
	percorreTS = tsnodeHeader;
	while (percorreTS->proximo != NULL) {
		printf("%s %d\n", percorreTS->rotulo, percorreTS->valor);
		percorreTS = percorreTS->proximo;
	}
	printf("%s\n", percorreTS->rotulo);
	printf("TI:\n");
	percorreinstruct = instructnodeHeader;
	while (percorreinstruct->proximo != NULL) {
		printf("%d ", percorreinstruct->opcode);
		if(percorreinstruct->op1def == 1){
			printf("%d ", percorreinstruct->op1->valor);
		}
		if(percorreinstruct->op2def == 1){
			printf("%d", percorreinstruct->op2->valor);
		}
		printf("\n");
		percorreinstruct = percorreinstruct->proximo;
	}
	printf("%d\n", percorreinstruct->opcode);*/
	fclose(Entrada);
	fclose(Saida);
}
