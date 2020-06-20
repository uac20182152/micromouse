#include <stdlib.h>
/*
hasmoved: deteta se o rato se moveu, comparando a posição atual com a posição anterior.
*/
int hasmoved(int *pos, int *oldpos){
  if (pos[0] == oldpos[0] && pos[1] == oldpos[1])
    return 0;
  else {
    return 1;
  }
}

/* getsurroundings: Recolhe os valores dos espaços adjacentes ao cursor
 * Devolve um apontador para 4 ints com esses valores.
 * A ordem das posições é sul, oeste, norte, este (clockwise).
*/
void getsurroundings(int *surroundings, int **maze, int *pos, int moved, int mode) {
  for (int i=0; i<4; i++){
    surroundings[i] = maze[pos[1]-abs(i-2)][pos[0]+abs(i-1)-2];
    /* Efetivamente o mesmo que fazer:
    surroundings[0] = maze[pos[1]-2][pos[0]-1];
    surroundings[1] = maze[pos[1]-1][pos[0]-2];
    surroundings[2] = maze[pos[1]][pos[0]-1];
    surroundings[3] = maze[pos[1]-1][pos[0]];
    */
  }
  //surroundings[4] guarda a direção, representada por pos[2]
  surroundings[4] = pos[2];
  //moved é um booleano que indica se houve movimento ou não (é um parametro)
  surroundings[5] = moved;
  //surroundings[6] guarda o conteudo da matriz na posição que o rato ocupa
  surroundings[6] = maze[pos[1]-1][pos[0]-1];
  surroundings[7] = mode;

}
