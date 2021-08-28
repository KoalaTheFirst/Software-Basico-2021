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

if (strcmp(leitura, "ADD") == 0) {
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
if (strcmp(leitura, "SPACE") == 0) {
	fprintf(Saida, "00 ");
}
if (strcmp(leitura, "CONST") == 0) {
	fscanf(Entrada, "%s", leitura);
	fprintf(Saida, "%s ", leitura);
}

void main() {
	FILE *Entrada, *Saida;
	char leitura[51];
	int i;

	Entrada = fopen("test.txt", "r");
	Saida = fopen("test.asm", "w");

	while (fscanf(Entrada, "%s", leitura) != EOF){
		for (i = 0; i < strlen(leitura); i++){
			leitura[i] = toupper(leitura[i]);
		}
		if (strcmp(leitura, "ADD") == 0) {
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
		if (strcmp(leitura, "JMP") == 0) {
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
		if (strcmp(leitura, "COPY") == 0) {
			fprintf(Saida, "09 ");
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
		if (strcmp(leitura, "STOP") == 0) {
			fprintf(Saida, "14 ");
		}
		if (strcmp(leitura, "SPACE") == 0) {
			fprintf(Saida, "00 ");
		}
		if (strcmp(leitura, "CONST") == 0) {
			fscanf(Entrada, "%s", leitura);
			fprintf(Saida, "%s ", leitura);
		}
		if (leitura[0] == ';') {
			while (fgetc(Entrada) != '\n' && fgetc(Entrada) != EOF);
		}
		printf("%s ", leitura);
	}
	fclose(Entrada);
	fclose(Saida);
}