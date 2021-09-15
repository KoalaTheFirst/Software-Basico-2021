#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>

struct TSNode {
	char rotulo[51];
	int space;
	int valor;
	struct InstructNode* instruct;
	struct TSNode* proximo;
};

struct InstructNode{
	int opcode; 
	int op1def;
	int op2def;
	char op1rotulo[51];
	char op2rotulo[51];
	struct TSNode* op1;
	struct TSNode* op2;
	int valorop1;
	int valorop2;
	struct InstructNode* proximo;
};

void criaTSNode(struct TSNode* tsnodeHeader, char* leitura, int valor, struct InstructNode* instruct, int spacebool) {
	struct TSNode* percorre = tsnodeHeader;
	
	if (percorre->proximo==NULL && percorre->valor==-1){
		
		percorre->valor = valor;
		strcpy(percorre->rotulo, leitura);
		percorre->instruct=instruct;
		percorre->proximo = NULL;
		percorre->space = spacebool;
	}
	else {
		while (percorre->proximo != NULL && strcmp(percorre->rotulo, leitura) != 0) {
			percorre = percorre->proximo;
		}
		if(strcmp(percorre->rotulo,leitura)==0){
			printf("Declarações de rótulos repetidos. Erro sintático '%s'.\n", leitura);
		}
		else{
			percorre->proximo = (struct TSNode*)malloc(sizeof(struct TSNode));
			percorre = percorre->proximo;
			percorre->proximo = NULL;
			percorre->instruct=instruct;
			percorre->valor = valor;
			strcpy(percorre->rotulo, leitura);
			percorre->space = spacebool;
		}
	}
}

struct InstructNode* criaInstructNode(struct InstructNode* instructnodeHeader, int opcode, struct TSNode* op1, struct TSNode* op2, char* leitura2, char* leitura3, int val1, int val2) {
	struct InstructNode* percorre = instructnodeHeader;

	if (percorre->proximo==NULL && percorre->opcode==-1&&percorre->op1==NULL&&percorre->op2==NULL){
		percorre->opcode = opcode;
		percorre->op1 = op1;
		percorre->op2 = op2;
		percorre->proximo = NULL;
		strcpy(percorre->op1rotulo, leitura2);
		strcpy(percorre->op2rotulo, leitura3);
		percorre->valorop1 = val1;
		percorre->valorop2 = val2;
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
		strcpy(percorre->op1rotulo, leitura2);
		strcpy(percorre->op2rotulo, leitura3);
		percorre->valorop1 = val1;
		percorre->valorop2 = val2;
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
	int i;
	char c=' ';
	fpos_t position1;

	for (i = 0; i < strlen(leitura); i++){
			leitura[i] = toupper(leitura[i]);
	}
	fgetpos(Entrada, &position1);
	if(leitura[0] != '\0' && strcmp(leitura, "ADD") != 0 && strcmp(leitura, "SUB") != 0 && strcmp(leitura, "MUL") != 0 && strcmp(leitura, "DIV") != 0 && strcmp(leitura, "JMP") != 0 && strcmp(leitura, "JMPN") != 0 && strcmp(leitura, "JMPP") != 0 && strcmp(leitura, "JMPZ") != 0 && strcmp(leitura, "COPY") != 0 && strcmp(leitura, "LOAD") != 0 && strcmp(leitura, "STORE") != 0 && strcmp(leitura, "INPUT") != 0 && strcmp(leitura, "OUTPUT") != 0 && strcmp(leitura, "STOP") != 0 && strcmp(leitura, "SECTION") != 0 && strcmp(leitura, "DATA") != 0 && strcmp(leitura, "SPACE") != 0 && strcmp(leitura, "CONST") != 0 && strcmp(leitura, "TEXT") != 0){
		while(c!= '\n' && c!= EOF && c!= ':' && (c=='\t' || c==' ')){
			c = fgetc(Entrada);
		}

		if(c==':'){
			fseek(Entrada,-1, SEEK_CUR);
			fprintf(Entrada, " ");
			fsetpos(Entrada, &position1);
			fprintf(Entrada, ":");
			leitura[strlen(leitura)+1]='\0';
			leitura[strlen(leitura)]=':';
		}
		else{
			fsetpos(Entrada, &position1);
		}
	}
}

struct TSNode* checasimbolo(struct TSNode* tsnodeHeader, char* leitura2, char* leitura3, int contador){
	struct TSNode *percorre=tsnodeHeader;
	int i=0,j=0, copy=0;

	for (i = 0; leitura2[i] != ',' && leitura2[i] != '\0'; ++i){}

	if(leitura2[i]==','){
		copy=1;
	}
	if(copy==1){
		leitura2[i] = '\0';
		i++;

		for(i=i;leitura2[i]!='\0';i++){
			leitura3[j] = leitura2[i];
			j++;
		}
	}
	leitura3[j]='\0';

	for (i = 0; leitura2[i] != '\0'; ++i){
		if(isalnum(leitura2[i])==0 && leitura2[i] != '_'){
			printf("Token inválido. Erro léxico na linha %d.\n", contador);
			break;
		}
	}

	if(isdigit(leitura2[0]) != 0){
		printf("Token inválido. Erro léxico na linha %d.\n", contador);
	}


	while(percorre->proximo != NULL && strcmp(percorre->rotulo, leitura2) !=0){
		percorre=percorre->proximo;
	}

	if(strcmp(percorre->rotulo,leitura2)==0){
		return percorre;
	}
	else{
		return NULL;
	}
}

struct InstructNode* instruct(char* leitura2, char* leitura3, FILE* Entrada, struct TSNode* tsnodeHeader, struct InstructNode* instructnodeHeader, int contador){
	struct TSNode *retornoTS, *retornoTS2;
	struct InstructNode* retornoTI;
	char dummy[1],c=' ';
	fpos_t position, position2;
	int i = 0, valor=0, valor2=0, j = 0;

		if (strcmp(leitura2, "ADD") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 1, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,1,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "SUB") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 2, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,2,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "MUL") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 3, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,3,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "DIV") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 4, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,4,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "JMP") == 0){
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 5, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,5,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "JMPN") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				fseek(Entrada, -1, SEEK_CUR);
				fputc(' ', Entrada);
				fgetpos(Entrada, &position2);
				while(c!=' ' && (isdigit(c) != 0 || c =='+')){
					c = fgetc(Entrada);
				}
				if(isdigit(c)==0 && c!=' ' && c!='\n'){
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
				else if(c == ' ' || c == '\n'){
					fsetpos(Entrada, &position2);
					fscanf(Entrada,"%d",&valor);
				}
				
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,6,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "JMPP") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 7, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,7,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "JMPZ") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 8, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,8,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "COPY") == 0) {
			fgetpos(Entrada, &position);
			while(c != '\n' && c != EOF && c != ','){
				c = fgetc(Entrada);
			}
			if(c == ','){ //existem dois argumentos
				fsetpos(Entrada, &position);
				c = ' ';
				while(c != '\n' && c != EOF && c != '+'){
					c = fgetc(Entrada);
				}
				if(c == '+'){ //algum argumento é um vetor
					fsetpos(Entrada, &position); //retorna pro inicio dos argumentos de copy
					fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
					//inserir testes pro primeiro argumento de copy
					c = ' ';
					i=0;
					while(c != ',' && c != '+'){
						c = fgetc(Entrada);
						i++;
					}
					if(c == '+'){ //operando 1 é um vetor
						fsetpos(Entrada, &position);
						fseek(Entrada, 1, SEEK_CUR);
						for (j = 0; j < i; j++){
							leitura2[j] = fgetc(Entrada);
						}
						leitura2[j-1] = '\0';
						fgetpos(Entrada, &position);
						c = ' ';
						while(c != ',' && (isdigit(c) != 0 || c == ' ')){
							c = fgetc(Entrada);
						}
						if( c == ','){ //primeiro vetor declarado corretamente
							fsetpos(Entrada, &position);
							fscanf(Entrada, "%d", &valor);
							fseek(Entrada, 1, SEEK_CUR);
							fgetpos(Entrada, &position);
							c = ' ';
							i=0;
							while(c != '\n' && c != EOF && c != '+'){
								c = fgetc(Entrada);
								i++;
							}
							if(c == '+'){ //segundo argumento é um vetor
								fsetpos(Entrada, &position);
								for (j = 0; j < i; j++){
									leitura3[j] = fgetc(Entrada);
								}
								leitura3[j-1] = '\0';
								fgetpos(Entrada, &position);
								c = ' ';
								while(c != '\n' && c != ' ' && c != EOF && isdigit(c) != 0){
									c = fgetc(Entrada);
								}
								if(c == '\n' || c == EOF || c == ' ' || c == '\t' || c == EOF){ //segundo vetor declarado corretamente
									fsetpos(Entrada, &position);
									fscanf(Entrada, "%d", &valor2);
									retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
									retornoTS2 = checasimbolo(tsnodeHeader, leitura3, dummy, contador);
									retornoTI = criaInstructNode(instructnodeHeader, 9, retornoTS, retornoTS2, leitura2, leitura3, valor, valor2);
									return retornoTI;
								}
								else{ //erro léxico na declaração do inteiro do segundo rótulo
									printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
									while(c != '\n' && c != EOF){
										c = fgetc(Entrada);
									}
									c = '\0';
									return NULL;
								}
							}
							else{ //prossegue com a leitura padrão do segundo rótulo
								fsetpos(Entrada, &position);
								fscanf(Entrada, "%s", leitura3);
								retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
								retornoTS2 = checasimbolo(tsnodeHeader, leitura3, dummy, contador);
								retornoTI = criaInstructNode(instructnodeHeader, 9, retornoTS, retornoTS2, leitura2, leitura3, valor, valor2);
								return retornoTI;
							}

						}
						else{ //erro léxico, o que vem depois de + não é um número
							printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
							while(c != '\n' && c != EOF){
								c = fgetc(Entrada);
							}
							c = '\0';
							return NULL;
						}
					}
					else{ //operando 1 não é um vetor
						i = 0;
						fsetpos(Entrada, &position);
						while(fgetc(Entrada) != ','){
							i++;
						}
						fsetpos(Entrada, &position);
						fseek(Entrada, 1, SEEK_CUR);
						for (j = 0; j < i; ++j){
							leitura2[j] = fgetc(Entrada);
						}
						leitura2[j-1] = '\0';
						//inicio dos testes do segundo rótulo
						fgetpos(Entrada, &position);
						c = ' ';
						i=0;
						while(c != '\n' && c != EOF && c != '+'){
							c = fgetc(Entrada);
							i++;
						}
						if(c == '+'){ //segundo argumento é um vetor
							fsetpos(Entrada, &position);
							for (j = 0; j < i; j++){
								leitura3[j] = fgetc(Entrada);
							}
							leitura3[j-1] = '\0';
							fgetpos(Entrada, &position);
							c = '0';
							printf("c1 = '%c'\n", c);
							while(c != '\n' && c != ' ' && c != EOF && isdigit(c) != 0){
								c = fgetc(Entrada);
							}
							if(c == '\n' || c == EOF || c == ' ' || c == '\t' || c == EOF){ //segundo vetor declarado corretamente
								printf("c2 = '%c'\n", c);
								fsetpos(Entrada, &position);
								fscanf(Entrada, "%d", &valor2);
								retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
								retornoTS2 = checasimbolo(tsnodeHeader, leitura3, dummy, contador);
								retornoTI = criaInstructNode(instructnodeHeader, 9, retornoTS, retornoTS2, leitura2, leitura3, valor, valor2);
								return retornoTI;
							}
							else{ //erro léxico na declaração do inteiro do segundo rótulo
								printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
								while(c != '\n' && c != EOF){
									c = fgetc(Entrada);
								}
								c = '\0';
								return NULL;
							}
						}
						else{ //prossegue com a leitura padrão do segundo rótulo
							fsetpos(Entrada, &position);
							fscanf(Entrada, "%s", leitura3);
							retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
							retornoTS2 = checasimbolo(tsnodeHeader, leitura3, dummy, contador);
							retornoTI = criaInstructNode(instructnodeHeader, 9, retornoTS, retornoTS2, leitura2, leitura3, valor, valor2);
							return retornoTI;
						}
					}
				}
				else{ //prossegue com a leitura padrão dos rótulos
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%s", leitura2);
					trata(leitura2, Entrada);
					for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
					if(strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':' && leitura2[0] != '\0'){
						retornoTS = checasimbolo(tsnodeHeader, leitura2, leitura3, contador);
						for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
						if(strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':' && leitura2[0] != '\0'){
							if(leitura3[0] == '\0'){
								printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
								while(c != '\n' && c != EOF){
									c = fgetc(Entrada);
								}
								c = '\0';
								return NULL;
							}
							else{
								for (i = 0; leitura3[i] != '\0' && leitura3[i] != ':'; ++i){}
								if(strcmp(leitura3, "ADD") != 0 && strcmp(leitura3, "SUB") != 0 && strcmp(leitura3, "MUL") != 0 && strcmp(leitura3, "DIV") != 0 && strcmp(leitura3, "JMP") != 0 && strcmp(leitura3, "JMPN") != 0 && strcmp(leitura3, "JMPP") != 0 && strcmp(leitura3, "JMPZ") != 0 && strcmp(leitura3, "COPY") != 0 && strcmp(leitura3, "LOAD") != 0 && strcmp(leitura3, "STORE") != 0 && strcmp(leitura3, "INPUT") != 0 && strcmp(leitura3, "OUTPUT") != 0 && strcmp(leitura3, "STOP") != 0 && strcmp(leitura3, "SECTION") != 0 && strcmp(leitura3, "DATA") != 0 && strcmp(leitura3, "SPACE") != 0 && strcmp(leitura3, "CONST") != 0 && strcmp(leitura3, "TEXT") != 0 && leitura3[i] != ':' && leitura3[0] != '\0'){
									retornoTS2 = checasimbolo(tsnodeHeader, leitura3,dummy, contador);
									retornoTI = criaInstructNode(instructnodeHeader,9,retornoTS,retornoTS2,leitura2,leitura3, valor, 0);
									return retornoTI;
								}	
							}
						}
						else{
							printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
							while(c != '\n' && c != EOF){
								c = fgetc(Entrada);
							}
							c = '\0';
							return NULL;
						}
					}
					else{
						printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
						while(c != '\n' && c != EOF){
							c = fgetc(Entrada);
						}
						c = '\0';
						return NULL;
					}
				}	
			}
			else{
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "LOAD") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 10, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,10,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "STORE") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 11, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,11,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "INPUT") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 12, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,12,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "OUTPUT") == 0) {
			fgetpos(Entrada, &position);
			while(c!='\n' && c!=EOF && c!= '+'){
				c = fgetc(Entrada);
			}
			if(c=='+'){
				c = ' ';
				fsetpos(Entrada, &position); //retorna para logo após ler a instrução
				fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
				i = 0;
				while(c != '+'){
					c = fgetc(Entrada);
					i++;
				}
				fsetpos(Entrada, &position);
				fseek(Entrada, 1, SEEK_CUR);
				for (j = 0; j < i; ++j){
					leitura2[j] = fgetc(Entrada);
				}
				leitura2[j-1] = '\0';
				fgetpos(Entrada, &position);
				i=0;
				c = '0';
				while(isdigit(c) != 0){
					c = fgetc(Entrada);
				}
				if (c == ' ' || c == '\n' || c == EOF || c == '\t'){
					fsetpos(Entrada, &position);
					fscanf(Entrada, "%d", &valor);
					retornoTS = checasimbolo(tsnodeHeader, leitura2, dummy, contador);
					retornoTI = criaInstructNode(instructnodeHeader, 13, retornoTS, NULL, leitura2, dummy, valor, 0);
					return retornoTI;
				}
				else{
					printf("Erro léxico. O argumento na linha %d deve ser um inteiro.\n", contador);
					while(c != '\n' && c != EOF){
						c = fgetc(Entrada);
					}
					c = '\0';
					return NULL;
				}
			}
			else{
				valor = 0;
			}
			fsetpos(Entrada, &position);
			fscanf(Entrada, "%s", leitura2);
			if(valor!=0){
				fscanf(Entrada,"%d", &valor);
			}
			trata(leitura2,Entrada);
			for (i = 0; leitura2[i] != '\0' && leitura2[i] != ':'; ++i){}
			if(leitura2[0] != '\0' && strcmp(leitura2, "ADD") != 0 && strcmp(leitura2, "SUB") != 0 && strcmp(leitura2, "MUL") != 0 && strcmp(leitura2, "DIV") != 0 && strcmp(leitura2, "JMP") != 0 && strcmp(leitura2, "JMPN") != 0 && strcmp(leitura2, "JMPP") != 0 && strcmp(leitura2, "JMPZ") != 0 && strcmp(leitura2, "COPY") != 0 && strcmp(leitura2, "LOAD") != 0 && strcmp(leitura2, "STORE") != 0 && strcmp(leitura2, "INPUT") != 0 && strcmp(leitura2, "OUTPUT") != 0 && strcmp(leitura2, "STOP") != 0 && strcmp(leitura2, "SECTION") != 0 && strcmp(leitura2, "DATA") != 0 && strcmp(leitura2, "SPACE") != 0 && strcmp(leitura2, "CONST") != 0 && strcmp(leitura2, "TEXT") != 0 && leitura2[i] != ':'){
				retornoTS = checasimbolo(tsnodeHeader, leitura2,leitura3, contador);
				retornoTI = criaInstructNode(instructnodeHeader,13,retornoTS,NULL, leitura2, leitura3, valor, 0);
				return retornoTI;
			}
			else{
				fsetpos(Entrada, &position);
				printf("Erro sintático.Quantidade de operandos inválida na linha %d\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
				return NULL;
			}
		}
		else if (strcmp(leitura2, "STOP") == 0) {
			retornoTI = criaInstructNode(instructnodeHeader,14,NULL,NULL, dummy, dummy, 0, 0);
			return retornoTI;
		}
	printf("Instrução inválida '%s'. Erro léxico na linha %d.\n", leitura2, contador);
	while(c != '\n' && c != EOF){
		c = fgetc(Entrada);
	}
	c = '\0';
	return NULL;
}

void rotulo(char* leitura, FILE* Entrada, struct InstructNode* instructnodeHeader, struct TSNode* tsnodeHeader, int contador){
	char  c=' ', leitura2[102], leitura3[51];
	struct InstructNode* retornoTI;
	fpos_t position;
	int val=0;

	leitura[strlen(leitura)-1] = '\0';

	fgetpos(Entrada, &position);
	while(c!='\n' && c!=':' && c!=EOF){
		c = fgetc(Entrada);
	}
	if(c == ':'){
		printf("Erro sintático. Declaração de dois rótulos na linha %d\n", contador);
		while(c!='\n' && c!=EOF){
			c = fgetc(Entrada);
		}
		return;
	}
	else{
		fsetpos(Entrada, &position);
	}
	c = ' ';
	do{
		fscanf(Entrada, "%s", leitura2);
		trata(leitura2,Entrada);
	}while(leitura2[0]=='\0');
	
	if (strcmp(leitura2, "SPACE") == 0) {
		fgetpos(Entrada, &position);
		while(c!=EOF && c!='\n' && (c ==' ' || c == '\t')){
			c = fgetc(Entrada);
		}
		if(c!=EOF && c != '\n' && c!=' ' && c!='\t'){
			fsetpos(Entrada,&position);
			fseek(Entrada, 1, SEEK_CUR); //pula a barra de espaço
			while(isdigit(c) != 0){
				c = fgetc(Entrada);
			}
			if(c == ' ' || c == '\n' || c == EOF || c == '\t'){

				fsetpos(Entrada, &position);
				fscanf(Entrada,"%d", &val);
				criaTSNode(tsnodeHeader, leitura, val, NULL, 1);
			}
			else{
				printf("Erro léxico. Erro no argumento de Space na linha %d, o argumento não é um inteiro.\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
			}
		}
		else if(c=='\n' || c==EOF){
			criaTSNode(tsnodeHeader, leitura, 1, NULL,1);
		}	
	}

	else if (strcmp(leitura2, "CONST") == 0) {
		fgetpos(Entrada, &position);
		while(c!=EOF && c!='\n' && (c ==' ' || c == '\t')){
			c = fgetc(Entrada);
		}
		if(c!=EOF && c!='\n'){
			fsetpos(Entrada,&position);
			c = '0';
			while(isdigit(c) != 0){
				c = fgetc(Entrada);
			}
			if(c == ' ' || c == '\n' || c == EOF || c == '\t'){
				fsetpos(Entrada, &position);
				fscanf(Entrada,"%d", &val);
				criaTSNode(tsnodeHeader, leitura, val, NULL, 0);
			}
			else{
				printf("Erro léxico. Erro no argumento de Const na linha %d, o argumento não é um inteiro.\n", contador);
				while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
			}
		}
		else{
			printf("Erro sintático. Argumento ausente na linha %d.\n", contador);
			while(c != '\n' && c != EOF){
					c = fgetc(Entrada);
				}
				c = '\0';
		}
	}
	
	else{
		retornoTI = instruct(leitura2, leitura3, Entrada, tsnodeHeader, instructnodeHeader, contador);
		criaTSNode(tsnodeHeader,leitura,1,retornoTI, 0);
	}

}

int associa(struct TSNode* tsnodeHeader, struct InstructNode* instructnodeHeader){
	struct TSNode* percorreTS = tsnodeHeader;
	struct InstructNode* percorreTI = instructnodeHeader;
	int erro = 0, contador=1;

	while(percorreTI->proximo!=NULL){
		contador++;
		if(percorreTI->opcode != 9 && percorreTI->opcode>0 && percorreTI->opcode<14){
			if(percorreTI->op1def==0){
				while(percorreTS->proximo!=NULL && strcmp(percorreTS->rotulo,percorreTI->op1rotulo) != 0){
					percorreTS=percorreTS->proximo;
				}
				if(strcmp(percorreTS->rotulo,percorreTI->op1rotulo) == 0){
					percorreTI->op1=percorreTS;
				}
				else{
					printf("Declaração de rótulo ausente. Erro semântico na linha %d.\n", contador);
					erro = 1;
				}
			}
		}
		else if(percorreTI->opcode==9){
			if(percorreTI->op1def==0){
				while(percorreTS->proximo!=NULL && strcmp(percorreTS->rotulo,percorreTI->op1rotulo) != 0){
					percorreTS=percorreTS->proximo;
				}
				if(strcmp(percorreTS->rotulo,percorreTI->op1rotulo) == 0){
					percorreTI->op1=percorreTS;
				}
				else{
					printf("Declaração de rótulo ausente. Erro semântico na linha %d.\n", contador);
					erro = 1;
				}
			}
			percorreTS = tsnodeHeader;
			if(percorreTI->op2def==0){
				while(percorreTS->proximo!=NULL && strcmp(percorreTS->rotulo,percorreTI->op2rotulo) != 0){
					percorreTS=percorreTS->proximo;
				}
				if(strcmp(percorreTS->rotulo,percorreTI->op2rotulo) == 0){
					percorreTI->op2=percorreTS;
				}
				else{
					printf("Declaração de rótulo ausente. Erro semântico na linha %d\n", contador);
					erro = 1;
				}
			}

		}
		percorreTS = tsnodeHeader;
		percorreTI=percorreTI->proximo;
	}
	return erro;
}

int main(int argc, char* argv[]) {
	FILE *Entrada, *Saida;
	char leitura[51], leitura2[102],c;
	struct InstructNode *instructnodeHeader = (struct InstructNode*)malloc(sizeof(struct InstructNode)), *percorreinstruct, *validainstruct;
	struct TSNode *tsnodeHeader = (struct TSNode*)malloc(sizeof(struct TSNode)), *percorreTS;
	int contador=0, i=1;

	tsnodeHeader->proximo=NULL;
	tsnodeHeader->valor=-1;
	instructnodeHeader->opcode = -1;
	instructnodeHeader->proximo = NULL;
	instructnodeHeader->op1 = NULL;
	instructnodeHeader->op2 = NULL;
	Entrada = fopen(argv[1], "r");
	
	for (i = 0; argv[1][i] != '.'; ++i){}
	argv[1][i+1] = 'o';
	argv[1][i+2] = 'b';
	argv[1][i+3] = 'j';
	Saida = fopen(argv[1], "w");

	while (fscanf(Entrada, "%s", leitura) != EOF){
		trata(leitura, Entrada);
		contador++;
		if(leitura[strlen(leitura)-1] == ':'){
			rotulo(leitura, Entrada, instructnodeHeader, tsnodeHeader, contador);
		}
		else if(leitura[0] == ';'){		
			while (c != '\n' && c != EOF){
				c = fgetc(Entrada);
			}
		}
		else if (strcmp(leitura, "SECTION") == 0) {
			fscanf(Entrada, "%s", leitura);
			trata(leitura,Entrada);
			if(strcmp(leitura,"DATA") != 0 && strcmp(leitura, "TEXT") != 0){
				printf("Erro sintático. Diretiva inválida na linha %d\n", contador);
			}
		}
		else{
			validainstruct = instruct(leitura, leitura2, Entrada, tsnodeHeader, instructnodeHeader, contador);
		}
		c = 'a';
	}
	contador = 0;

	associa(tsnodeHeader, instructnodeHeader);

	//Faz a contagem das instruções
	percorreinstruct = instructnodeHeader;
	while(percorreinstruct != NULL){
		if((percorreinstruct->opcode>0 && percorreinstruct->opcode<9) || (percorreinstruct->opcode>9 && percorreinstruct->opcode<14)){
			contador=contador+2;
		}
		if (percorreinstruct->opcode == 9){
			contador=contador+3;
		}
		if (percorreinstruct->opcode == 14){
			contador = contador + 1;
		}
		percorreinstruct=percorreinstruct->proximo;
	}
	percorreTS = tsnodeHeader;
	while(percorreTS != NULL){
		if(percorreTS->instruct == NULL){
			percorreinstruct = instructnodeHeader;
			while(percorreinstruct->proximo != NULL){
				if (strcmp(percorreinstruct->op1rotulo, percorreTS->rotulo) == 0){
					percorreinstruct->valorop1 = percorreinstruct->valorop1 + contador;
					percorreinstruct = percorreinstruct->proximo;
				}
				else if(strcmp(percorreinstruct->op2rotulo, percorreTS->rotulo) == 0){
					percorreinstruct->valorop2 = percorreinstruct->valorop2 + contador;
					percorreinstruct = percorreinstruct->proximo;
				}
				else{
					percorreinstruct = percorreinstruct->proximo;
				}
			}
			if(percorreTS->space==0){
				contador++;
			}
			else{
				contador = contador + percorreTS->valor;
			}
		}
		else{
			validainstruct = instructnodeHeader;
			i=0;
			while(validainstruct != percorreTS->instruct && validainstruct->proximo != NULL){
				if((validainstruct->opcode>0 && validainstruct->opcode<9) || (validainstruct->opcode>9 && validainstruct->opcode<14)){
					i=i+2;
				}
				if (validainstruct->opcode == 9){
					i=i+3;
				}
				if (validainstruct->opcode == 14){
					i = i + 1;
				}
				validainstruct = validainstruct->proximo;
			}
			percorreinstruct = instructnodeHeader;
			while(percorreinstruct->proximo != NULL){
				if (strcmp(percorreinstruct->op1rotulo, percorreTS->rotulo) == 0){
					percorreinstruct->valorop1 = i + percorreinstruct->valorop1;
					percorreinstruct = percorreinstruct->proximo;
				}
				else if(strcmp(percorreinstruct->op2rotulo, percorreTS->rotulo) == 0){
					percorreinstruct->valorop2 = i + percorreinstruct->valorop2;
					percorreinstruct = percorreinstruct->proximo;
				}
				else{
					percorreinstruct = percorreinstruct->proximo;
				}
			}
		}
		percorreTS = percorreTS->proximo;
	}

	//Escreve o .asm de saída
	percorreinstruct = instructnodeHeader;
	while(percorreinstruct != NULL){
		if(percorreinstruct->opcode != 9 && percorreinstruct->opcode != 14){
			fprintf(Saida, "%d %d ", percorreinstruct->opcode, percorreinstruct->valorop1);
		}
		if (percorreinstruct->opcode == 9){
			fprintf(Saida, "%d %d %d ", percorreinstruct->opcode, percorreinstruct->valorop1, percorreinstruct->valorop2);
		}
		if(percorreinstruct->opcode == 14){
			fprintf(Saida, "%d ", percorreinstruct->opcode);
		}
		percorreinstruct = percorreinstruct->proximo;
	}
	percorreTS = tsnodeHeader;
	while(percorreTS != NULL){
		if(percorreTS->space == 1){
			for (i = 0; i < percorreTS->valor; ++i){
				fprintf(Saida, "0 ");
			}
		}
		if (percorreTS->space == 0 && percorreTS->instruct == NULL){
			fprintf(Saida, "%d ", percorreTS->valor);
		}
		percorreTS = percorreTS->proximo;
	}


	//desaloca a memória das listas
	percorreTS = tsnodeHeader;
	while (percorreTS->proximo != NULL) {
		percorreTS = percorreTS->proximo;
		free(tsnodeHeader);
		tsnodeHeader = percorreTS;
	}
	free(tsnodeHeader);
	percorreinstruct = instructnodeHeader;
	while (percorreinstruct->proximo != NULL) {
		percorreinstruct = percorreinstruct->proximo;
		free(instructnodeHeader);
		instructnodeHeader = percorreinstruct;
	}
	free(instructnodeHeader);

	fclose(Entrada);
	fclose(Saida);
	return 0;
}
