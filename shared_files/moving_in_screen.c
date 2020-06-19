#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#define COLUMNS 67
#define ROWS 32

struct termios oldtc;
struct termios newtc;
/*Cria estruturas termios para armazenar os parametros e
informações do terminal*/

int modulo(int a, int b){
  int result = a%b;
  return result >= 0? result : result+b;
}

void clearscreen(){
  /*Esta função faz uso das ANSI escape sequences,
  sequências de bytes que ao serem impressas,
  são interpretadas pelo terminal como comandos
  e não como caratéres.*/
  //printf("\033[2J");
  system("clear");  // foi utilizado este com o objetivo de corrigir um bug

}

void cbreak() {
  /*
  Esta função coloca o terminal no modo cbreak ou rare,
  no qual o terminal interpreta um carater de cada vez,
  mas continua a reconhecer combinações de teclas de controlo,
  como Ctrl-C.
  */
  tcgetattr(STDIN_FILENO, &oldtc);
  /*Obtẽm os parâmetros/atributos associados ao
  terminal e coloca-os na struct termios oldtc*/
  newtc = oldtc; //Atribui o valor de oldtc a newtc.
  newtc.c_lflag &= ~(ICANON | ECHO);
  /*Modifica a flag c_lflag que controla o modo do terminal,
  e efetua um bitwise-and com o bitwise-not do bitwise-or das constantes ICANON
  e ECHO, efetivamente definindo o modo não-canónico e a não-ecoação dos carateres
  introduzidos.
  Com o modo canónico desativado, a input do utilizador é dada caratér a carater,
  sem necessidade de delimitadores como newline, entre outras coisas.
  Com ECHO desativado, os carateres introduzidos não são ecoados, ou escritos,
  no ecrã.*/
  tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
  /*Define os atributos do terminal tal como definidos em newtc,
  ou seja, desativa o modo canónico e o eco*/
}

void nocbreak() {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
  /*Repõe os atributos do terminal para aqueles obtidos no início do programa e
  guardados em oldtc.*/
  fflush(stdout);
}

void up(int **maze, int *pos){
  if (maze[pos[1]][pos[0]-1] != 1){
    pos[1]++;             // movimenta para cima
  }                          // retorna a posição atual
}

void down(int **maze, int *pos){
  if (maze[pos[1]-2][pos[0]-1] != 1){
    pos[1]--;
  }
}

void right(int **maze, int *pos){
  //printf("\033[%dC",1);
  if (maze[pos[1]-1][pos[0]] != 1){
    pos[0]++;
  }
}

void left(int **maze, int *pos){

  if (maze[pos[1]-1][pos[0]-2] != 1){
    pos[0]--;
  }
}

int *mapa(int **maze, int mode){
  int *pos;
  pos = malloc(sizeof(int*) * 3);

  for(int i=ROWS;i>=0;i--){           // calcula a posição do S (posição inicial)
    for(int j = 0; j < COLUMNS; j++){
      if (maze[i][j] == 2){
        pos[1] = i+1;
        pos[0] = j+1;
        pos[2] = mode == 1? 0 : 4;
      }
    }
  }
  printf("\033[%d;%dH",ROWS+2-pos[1],pos[0]);   //coloca o cursor na posição do S
  return pos;
}

void erasepos(int *pos, int **maze){
  cbreak();
  printf("\033[%d;%dH",ROWS+2-pos[1]+4,pos[0]+1);
  if (maze[pos[1]-1][pos[0]-1]==2){
    printf("%c", 'S');
  }
  else
    printf(" ");
  printf("\033[%dD",1);
  nocbreak();
}

void printpos(int *pos){
  char* symbol = "V<^>@";
  cbreak(); // coloca o terminal do modo cbreak
  printf("\033[%d;%dH",ROWS+2-pos[1]+4,pos[0]+1);
  printf("%c",symbol[pos[2]]);
  printf("\033[%dD",1);
  nocbreak(); // tira o terminal do modo cbreak
  //printf("\033[%d;%dH",ROWS+2-pos[1],pos[0]);
}

void movecursor(int **maze, int *pos, int *oldpos, char m, int mode){
    switch(m){
      case 'w'://"cima"
      case 'W':
        if (mode==0){
          pos[2]=2;
          up(maze, pos);
        }
        else {
          switch (pos[2]) {
            case 0:
              down(maze, pos);
              break;
            case 1:
              left(maze, pos);
              break;
            case 2:
              up(maze, pos);
              break;
            case 3:
              right(maze, pos);
              break;
          }
        }
        break;

      case 'd'://"direita"
      case 'D':
        if (mode == 0) {
          pos[2] = 3;
          right(maze, pos);
        }
        else {
          pos[2] = modulo(pos[2]+1, 4);
        }

        break;

      case 's'://"baixo"
      case 'S':
        if (mode==0){
          pos[2] = 0;
          down(maze, pos);
        }

        break;

      case 'a'://"esquerda"
      case 'A':
        if (mode == 0) {
          pos[2]=1;
          left(maze, pos);
        }
        else {
          pos[2] = modulo(pos[2]-1, 4);
        }
        break;
      }

    erasepos(oldpos, maze);
    printpos(pos);
}
