#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../shared_files/moving.h"
#define COLUMNS 135
#define ROWS 65
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
int rowhasnonzero(char *array, int size) {
  for (int i=0; i<size; i++){
    if (array[i]!=' ') return 1;
  }
  return 0;
}

/*
colhasnonzero: verifica se há pelo menos um char não espaço (' ') numa coluna de
uma matriz char **array
*/
int colhasnonzero(char **matrix, int rows, int column){
  for (int i = 0; i<rows; i++){
    if (matrix[i][column]!=' '){
      return 1;
    }
  }
  return 0;
}

/*
minrow: identifica a linha não nula de menor índice de uma matriz char **array
*/
int minrow(char **matrix, int rows, int columns){
  for (int i=0; i<rows; i++) {
    if (rowhasnonzero(matrix[i], columns)==1) {
      return i;
    }
  }
  return 0;
}

/*
maxrow: identifica a linha não nula de maior índice de uma matriz char **array
*/
int maxrow(char **matrix, int rows, int columns){
  for (int i=rows-1; i>=0; i--) {
    if (rowhasnonzero(matrix[i], columns)==1) {
      return i;
    }
  }
  return 0;
}

/*
mincolumn: identifica a coluna não nula de menor índice de uma matriz char **array
*/
int mincolumn(char **matrix, int rows, int columns){
  for (int i=0; i<columns; i++){
    if (colhasnonzero(matrix, rows, i) == 1) return i;
  }
  return 0;
}

/*
maxcolumn: identifica a coluna não nula de maior índice de uma matriz char **array
*/
int maxcolumn(char **matrix, int rows, int columns){
  for (int i=columns-1; i>=0; i--){
    if (colhasnonzero(matrix, rows, i) == 1) return i;
  }
  return 0;
}

/*
boundingbox: preenche a estrutura que representa o retângulo mínimo envolvente
do que o rato já conhece.
*/
void boundingbox(char **maze, box *b){
  b->mincol = mincolumn(maze, ROWS, COLUMNS);
  b->minrow = minrow(maze, ROWS, COLUMNS);
  b->maxcol = maxcolumn(maze, ROWS, COLUMNS);
  b->maxrow = maxrow(maze, ROWS, COLUMNS);
}

/*
move: atualiza a posição do rato, representada por int *pos, com base nas
informações recolhidas no último movimento.
*/
void move(int *pos, int *surroundings){
  if (surroundings[5]==1){
    switch (surroundings[4]){
      case 0:
        pos[1]++;
        break;
      case 1:
        pos[0]--;
        break;
      case 2:
        pos[1]--;
        break;
      case 3:
        pos[0]++;
        break;
    }
  }
}


/*
move: atualiza a matriz do que o rato conhece, representada por char**maze,
com base nas informações recolhidas em surroundings e na posição pos atualizada.
*/
void updatemaze(int *pos, int *surroundings, char **maze){
  maze[pos[1]+1][pos[0]] = surroundings[0];
  maze[pos[1]][pos[0]-1] = surroundings[1];
  maze[pos[1]-1][pos[0]] = surroundings[2];
  maze[pos[1]][pos[0]+1] = surroundings[3];
  maze[pos[1]][pos[0]] = surroundings[4];
}

/*
surroundings_code: os símbolos para representar o labirinto, nos ĩndices
correspondentes ao valor int que os representa na matriz do servidor.
mouse_code: os símbolos para representar o rato nas várias direções, nos ĩndices
correspondentes ao valor int que as representa.
decode: traduz os valores de surroundings de int para chars usando os strings anteriores.
*/
char *surroundings_code = " %SG";
char *mouse_code = "v<^>";
void decode(int *surroundings){
  // String com o símbolo na posição correspondente
  // 0=espaço, 1=obstáculo, 2=posição inicial, 3=objetivo
  // ver reading_a_file.c
  int i; // variável contador
  for (i=0; i<4; i++) {
    surroundings[i]=surroundings_code[surroundings[i]];
    // Substituir cada valor pelo símbolo correspondente.
  }
  surroundings[4]=mouse_code[surroundings[4]];
}

/*
display: mostra o que o rato vê nas posições adjacentes.
substituida por displaymaze.
*/
void display(int *surroundings){
  printf("\033[0;0H\033[2JMICROMOUSE CLIENT - Grupo1 Corporation\n");
  printf("\nMicroVision™:\n      ▁ \n     ▟%c▙\n    ▐%c%c%c▌\n     ▜%c▛\n      ▔ \n",
    surroundings[2], surroundings[1], surroundings[4], surroundings[3], surroundings[0]);
}

/*
displaymaze: apresenta o logo micromouse, a parte do labirinto que o rato conhece,
a moldura do labirinto, e o nome do grupo.
*/
void displaymaze(char **maze, box *b){
  // obtem o retangulo minimo envolvente que o rato conhece
  boundingbox(maze, b);
  // calcula o offset das linhas para o centro
  int rowoffset = ((ROWS/4)-(b->maxrow - b->minrow))/2;
  // calcula o offset das colunas para o centro
  int coloffset = ((COLUMNS/4)-(b->maxcol - b->mincol))/2;

  // IMPRESSÃO NO ECRÃ
  // limpa o ecra
  clearscreen();

  // IMPRESSÃO DO LOGÓTIPO
  printf("%s\n", "╔╦╗╦╔═╗╦═╗╔═╗╔╦╗╔═╗╦ ╦╔═╗╔═╗  ┌─┐┬  ┬┌─┐┌┐┌┌┬┐\n║║║║║  ╠╦╝║ ║║║║║ ║║ ║╚═╗║╣   │  │  │├┤ │││ │ \n╩ ╩╩╚═╝╩╚═╚═╝╩ ╩╚═╝╚═╝╚═╝╚═╝  └─┘┴─┘┴└─┘┘└┘ ┴ ");
  // imprime uma linha em branco
  for (int i=0; i<COLUMNS/2; i++) {
    printf(" ");
  }
  printf("\n");

  // IMPRESSÃO DO CONTEUDO DA MATRIZ DO CLIENTE
  // imprime tantas newlines quanto for preciso (offset)
  for (int rocounter=0; rocounter<rowoffset; rocounter++) {
    printf(" \n");
  }
  // imprime espaços em branco até chegar ao retangulo minimo
  for (int i = b->minrow; i <= b->maxrow; i++){
    printf(" ");
    // quando chega ao retangulo minimo
    for (int cocounter=0; cocounter<coloffset; cocounter++) printf(" ");
    for(int j = b->mincol; j <= b->maxcol; j++){
      // imprime o seu conteudo
      printf("%c", maze[i][j]);
    }
    printf("\n");
  }

  // IMPRESSÃO DA MOLDURA
  //entra no modo cbreak para pode imprimir por cima do que ja foi impresso
  cbreak();
  for (int i=4; i<ROWS/2+7; i++){ //percorre as linhas
    for (int j=0; j<COLUMNS/2+2; j++){ //percorre as colunas
      // se estiver sobre a linha 4, coluna 0, linha maxima ou coluna maxima
      if (i==4 || j==0 || i==ROWS/2+6 || j==COLUMNS/2+1){
        printf("\033[%d;%dH",i,j); // move o cursor para lá
        printf("%s", "█"); // imprime o carater cheio
      }
    }
  }
  //por fim imprime o nome do grupo
  printf("\nGrupo 1 - Francisco Mendonça, Gil Silva e Rui Bento");
  nocbreak(); // sai do modo cbreak
}
