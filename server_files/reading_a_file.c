#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../shared_files/moving.h"
#define FILE_DEFAULT "./test.txt"

#define COLUMNS 67
#define ROWS 32

/*
 * Lê o ficheiro, dado como argv[] e retorna a matriz,
 * como apontador para apontador para inteiro;
*/
int **ler(int argc, char* argv[]) {

  FILE *fp;
  char buff[COLUMNS];
  char c;
  char ficheiro[200];
  //Selecting the maze using the commandline
  if (argc > 1){
    strcpy(ficheiro,argv[1]);
  }
  else
  {
    strcpy(ficheiro, FILE_DEFAULT);
  }
  if ((fp = fopen(ficheiro,"r")) == NULL){
    nocbreak();
    printf("Error! opening file\n");
    // Program exits if the file pointer returns NULL.
    exit(1);
  }
  // reads text until newline is encountered
  int **maze; // cria um apontador

  maze = malloc(sizeof(int*) * COLUMNS); // o malloc aloca a memoria dinamicamente no apontador

  for(int i = 0; i < COLUMNS; i++) {
    maze[i] = malloc(sizeof(int*) * ROWS);
  }

  FILE *fs;
  fs = fopen(ficheiro, "r");
  /*
    * NOTA: Os prints comentados que se encontram no meio do código,
    * servem para verificar como a matriz está construída, o \n final
    * serve para "passar" para a linha seguinte
    *
    * Valores:
    * Espaço vazio - 0 (int)
    * Limites - 1 (int)
    * Start - 2 (int)
    * Goal - 3 (int)
  */
  printf("╔╦╗╦╔═╗╦═╗╔═╗╔╦╗╔═╗╦ ╦╔═╗╔═╗  ┌─┐┌─┐┬─┐┬ ┬┌─┐┬─┐\n║║║║║  ╠╦╝║ ║║║║║ ║║ ║╚═╗║╣   └─┐├┤ ├┬┘│┌┘├┤ ├┬┘\n╩ ╩╩╚═╝╩╚═╚═╝╩ ╩╚═╝╚═╝╚═╝╚═╝  └─┘└─┘┴└─└┘ └─┘┴└─\n███████████████████████████████████████████████████████████████████\n");
  for (int i=ROWS;i>=0;i--){ //(!feof(fp))
    fgets(buff,COLUMNS, (FILE*)fs);
    buff[COLUMNS-2]='\0';
    printf("█%s█\n", buff);
    for(int j = 0; j < COLUMNS; j++){
      if(buff[j] ==  ' '){
        maze[i][j] = 0;
        //printf("%d", maze[i][j]);
      } else if(buff[j] == 'S'){
        maze[i][j] = 2;
        //printf("%d", maze[i][j]);
      } else if(buff[j] == 'G'){
        maze[i][j] = 3;
        //printf("%d", maze[i][j]);
      } else{
        maze[i][j] = 1;
        //printf("%d", maze[i][j]);
      }
    }
    //printf("\n");
  }
  printf("███████████████████████████████████████████████████████████████████\n");
  printf("Grupo 1 - Francisco Mendonça, Gil Silva e Rui Bento\n");
  fclose(fp);

  return maze; //retorna o apontador que aponta para a matriz

}
