#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../shared_files/moving.h"
/*
box: esturutura que representa o retângulo mínimo envolvente do que o rato já conhece.
*/
typedef struct box{
  int minrow;
  int maxrow;
  int mincol;
  int maxcol;
} box;

/*
rowhasnonzero: verifica se há pelo menos um char não espaço (' ') num char array.
*/
int rowhasnonzero(char *array, int size);
/*
colhasnonzero: verifica se há pelo menos um char não espaço (' ') numa coluna de
uma matriz char **array
*/
int colhasnonzero(char **matrix, int rows, int column);
/*
minrow: identifica a linha não nula de menor índice de uma matriz char **array
*/
int minrow(char **matrix, int rows, int columns);
/*
maxrow: identifica a linha não nula de maior índice de uma matriz char **array
*/
int maxrow(char **matrix, int rows, int columns);

/*
mincolumn: identifica a coluna não nula de menor índice de uma matriz char **array
*/
int mincolumn(char **matrix, int rows, int columns);

/*
maxcolumn: identifica a coluna não nula de maior índice de uma matriz char **array
*/
int maxcolumn(char **matrix, int rows, int columns);

/*
boundingbox: preenche a estrutura que representa o retângulo mínimo envolvente
do que o rato já conhece.
*/
void boundingbox(char **maze, box *b);

/*
move: atualiza a posição do rato, representada por int *pos, com base nas
informações recolhidas no último movimento.
*/
void move(int *pos, int *surroundings);
/*
move: atualiza a matriz do que o rato conhece, representada por char**maze,
com base nas informações recolhidas em surroundings e na posição pos atualizada.
*/
void updatemaze(int *pos, int *surroundings, char **maze);
/*
surroundings_code: os símbolos para representar o labirinto, nos ĩndices
correspondentes ao valor int que os representa na matriz do servidor.
mouse_code: os símbolos para representar o rato nas várias direções, nos ĩndices
correspondentes ao valor int que as representa.
decode: traduz os valores de surroundings de int para chars usando os strings anteriores.
*/

void decode(int *surroundings);

/*
display: mostra o que o rato vê nas posições adjacentes.
substituida por displaymaze.
*/
void display(int *surroundings);

/*
display2: mostra o que o rato conhece do labirinto.
substituida por displaymaze.
*/
void display2(char **maze);

/*
displaymaze: apresenta o logo micromous, a parte do labirinto que o rato conhece,
a moldura do labirinto, e o nome do grupo.
*/
void displaymaze(char **maze, box *b);
