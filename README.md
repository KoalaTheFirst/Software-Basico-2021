<h1 align="center">Montador e simulador de assembly fake</h1>

<p align="center">Programas para montar e simular outros programas escritos em linguagem assembly inventada.</p>

Este repositório contém dois programas escritos em C e foram projetados para rodar em sistemas com Kernel Linux.
Os programas foram feitos para gerar o código objeto de programas escritos em linguagem assembly inventada e posteriormente simula-lo.
A linguagem assembly inventada consiste nas seguintes instruções:

|Instruções|Opcode|Descrição                      |
|:--------:|:----:|:-----------------------------:|
|ADD       |1     |ACC<-ACC + mem(OP)             |
|SUB       |2     |ACC<-ACC - mem(OP)             |
|MUL       |3     |ACC<-ACC * mem(OP)             |
|DIV       |4     |ACC<-ACC / mem(OP)             |
|JMP       |5     |PC<-OP                         |
|JMPN      |6     |Se ACC<0 então PC<-OP          |
|JMPP      |7     |Se ACC>0 então PC<-OP          |
|JMPZ      |8     |Se ACC=0 então PC<-OP          |
|COPY      |9     |mem(OP2)<-mem(OP1)             |
|LOAD      |10    |ACC<-mem(OP)                   |  
|STORE     |11    |mem(OP)<-ACC                   |
|INPUT     |12    |mem(OP)<-entrada               |
|OUTPUT    |13    |saída<-mem(OP)                 |
|STOP      |14    |Suspende a execução do programa|

Nesta tabela ACC representa o acumulador que guarda o resultado das operações feitas, OP é o operando da instrução, mem(OP) por consquência é o valor guardado naquele operando (os operandos e rótulos serão tratados mais pra frente neste README).
OP é a linha de instrução que será executada em seguida. 'entrada' é algo digitado pelo usuário na linha de comando, e 'saída' algo escrito pelo programa em forma de linha de comando. 

O montador também utiliza as seguintes diretivas:

|Diretiva    |Descrição                      |
|:----------:|:-----------------------------:|
|SECTION DATA|Define o inicio da sessão do código em que são definidas as variáveis que serão utilizadas|
|SECTION TEXT|Define o inicio da sessão do código em que são escritas as instruções do programa         |
|SPACE       |Declara que um rótulo é uma variável inicializada por padrão como 0                       |
|CONST       |Declara que o rótulo é uma constante                                                      |

Cada instrução da linguagem em assembly inventado deve ser escrita da seguinte maneira:  
`<rótulo>` : `<Instrução>` `<Operando 1>` `<Operando 2>`  
O rótulo é opcional, e pode ou não ser declarado. O Operando 2 deve ser declarado apenas quando há a necessidade, como no caso da instrução COPY que utiliza dois operandos para ser executada.  
Cada declaração de variável segue a seguinte estrutura:  
`<rótulo>` : `<diretiva>` `<argumento>`  
A diretiva SPACE pode receber como argumento um número inteiro positivo, caso isto ocorra, aquele rótulo será declarado como um vetor com número de espaços de memória iguais ao número usado como argumento, caso não, será apenas uma variável simples. Exemplo:  
`r1`: `SPACE` `4`  
r1 será então um vetor de 4 espaços. Ao utilizar uma instrução INPUT nesse caso poderiamos então declara-la como:  
`INPUT` `r1+2`  
Então o valor digitado pelo usuário seria escrito no terceiro espaço de memória daquele vetor (Isto ocorre pois o número após o '+' é o índice do vetor. Índices são declarados a partir de 0).  
A diretiva CONST deve receber como argumento um número inteiro positivo como no seguinte exemplo:
`<rótulo>`: `CONST 5`  
Com isto o valor de `rótulo` seria declarado como 5.

Desta forma, um exemplo de código que poderia ser escrito é:  
`SECTION DATA`  
`ENTRADA`: `SPACE 2`  
`UM`: `CONST 1`  
`SAIDA`: `SPACE`  
`SECTION TEXT`  
`INPUT` `ENTRADA`  
`INPUT` `ENTRADA+1`    
`LOAD` `ENTRADA`  
`SUB` `UM`  
`ADD` `ENTRADA+2`  
`STORE` `SAIDA`  
`OUTPUT` `SAIDA`  
`STOP`  
Este programa receberia duas entradas, subtrairia a primeira de 1, somaria a terceira ao resultado da subtração e por fim mostraria na tela o resultado após as operações.

O montador aceita ainda que comentários sejam feitos nas linhas de código. Eles podem ser inseridos digitando ';` e tudo após será ignorado no momento da montagem. Lembre-se que os comentários devem ser feitos sempre após o último operando e devem ser finalizados com uma quebra de linha.

O montador é capaz de detectar alguns erros na escrita do código, entre eles:
* declarações de rótulos ausentes
* declarações de rótulos repetidos
* diretivas inválidas
* instruções inválidas
* instruções com a quantidade de operandos errada
* tokens inválidos
* duas declarações de rótulos na mesma linha

Apesar disso o montador é altamente experimental, e podem ocorrer comportamentos inesperados que não consegui validar, então de nenhuma forma ele é infalível.
Ambos os programas podem ser compilados de maneira simples através do GCC, nenhum dos dois programas utiliza nenhuma biblioteca ou header adicional além dos padrões inclusos na linguagem C.
Após a compilação o arquivo que o montador lerá pode ser declarado na linha de comando utilizando `./montador nomedoarquivo.extensão`. Será gerado na mesma pasta do executável um outro arquivo chamado "nomedoarquivo.OBJ".
Este arquivo gerado deve ser então inserido no simulador utilizando `./simulador nomedoarquivo.OBJ`.

