#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../shared_files/moving.h"
#include "display_client.h"
#define COLUMNS 135
#define ROWS 65

void display2(char **maze){
  clearscreen();
  for (int i = 0; i < ROWS; i++){
    for(int j = 0; j < COLUMNS; j++){
      printf("%c", maze[i][j]);
      }
    printf("\n");
  }
  printf("\n");
}

/*
haswall: Deteta se há uma parede à frente do rato.
Usada no modo random.
Retorna 1 se ouver uma parede, isto é, Se o conteúdo da posição à frente do rato for 1.
Caso contrário, retorna 0.
A direção do rato é representada por surroundings[4] (0:S, 1:O, 2:N, 3:E)
O conteúdo da posição é representado por surroundings[0:4] (0:S, 1:O, 2:N, 3:E)
*/
int haswall(int *surroundings){
  return surroundings[surroundings[4]]==1;
}

/*
teclas, teclasforward e randtecla: Escolhem uma tecla para o modo aleatório.
teclas: representa as escolhas a tomar quando confrontado com uma parede.
(só pode virar para os lados)
teclasforward: representa as escolhas a tomar quando confrontado com o vazio.
(3/4 de probabilidade de andar em frente, 1/4 de virar para um dos lados.)
randtecla: deteta se há parede, faz o sorteio da tecla adequada, retorna a tecla
*/
char * teclas = "ad";
char * teclasforward = "wwwwwwad";
char * teclaswasd = "wasd";
int randchar = 'a';
int randnum = 0;
char randtecla(int wall, int *surroundings){
  usleep(100000);
  if (surroundings[7] == 0){
    randnum = rand()%4;
    if (randnum<1 || wall) randchar = teclaswasd[rand()%4];
    return randchar;
  }
  else if (wall){
    return teclas[rand()%2];
  }
  else
    return teclasforward[rand()%8];
}

/*
main: configura o socket e estruturas de dados, estabelece comunicação com o servidor, delega
(a maioria das) tarefas para as restantes funções na ordem correta.
*/
void main(int argc, char* argv[])
{

  /*
  Configuração do socket. Endereço IP e número da porta recebidos pela linha
  de comandos, nos dois primeiros argumentos a seguir ao nome do programa
  ex: micromouse_client 127.0.0.1 41111 (...)
  */
  int sock;
  struct sockaddr_in name;
  struct addrinfo *hp,hints;

  //struct hostent *hp, *gethostbyname();
  /* Cria socket para enviar. */
  sock = socket(AF_INET,SOCK_DGRAM, 0);
  if (sock == -1) {
    perror("opening datagram socket");
    exit(1);
  }
  /*
  Construir o nome, sem wildcards, do socket para o qual vai enviar.
  * gethostbyname devolve uma estrutura que inclui o endereço IP do host
  * especificado. O número da porta é recebido na linha de comanodos.
  */
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  int res = getaddrinfo(argv[1],argv[2],&hints,&hp);
  if (res != 0) {
    fprintf(stderr, "%s: getting address error\n", argv[1]);
    exit(2);
  }

  int n,len;
  int surroundings[8];
  struct sockaddr_in server;
  char tecla[2];
  tecla[0]=' ';
  clearscreen();


  char **maze; // cria um apontador
  maze = malloc(sizeof(char*) * ROWS); // o malloc aloca a memoria dinamicamente no apontador
  for (int i = 0; i < ROWS; i++) {
    maze[i] = malloc(sizeof(char*) * COLUMNS);
    for (int j=0; j < COLUMNS; j++) {
      maze[i][j] = ' ';
    }
  }
  int* pos = malloc(sizeof(int) * 2);
  pos[0] = COLUMNS / 2 + 1;
  pos[1] = ROWS / 2 + 1;

  box bounding;
  int moderandom=0;
  if (argc>3){
    moderandom=1;
  }

  int wall=0;
  do {
    /* Enviar tecla.
     * A primeira tecla enviada é sempre um espaço (que não afeta a posição),
     * para receber os surroundings assim que o cliente se conecta.*/
    if (sendto(sock,tecla, sizeof(tecla), 0, hp->ai_addr,hp->ai_addrlen) == -1){
      perror("sending datagram message");
    }

    // Esperar e receber os surroundings do micromouse.
    n = recvfrom(sock, (int *)surroundings, 64, MSG_WAITALL,
      (struct sockaddr *) &server, &len);
    if (n==-1) perror("receiving datagram message");
    //for (int i = 0; i<6; i++) printf("%d ", surroundings[i]);
    if (surroundings[6]==3) break;
    wall = haswall(surroundings);
    move(pos, surroundings);
    decode(surroundings); // Descodificar os surroundings
    updatemaze(pos, surroundings, maze);

    displaymaze(maze, &bounding);
    if (moderandom==0){
      cbreak(); // Por o terminal no modo cbreak
      tecla[0] = getchar(); // Pedir tecla ao utilizador
      nocbreak(); // Tirar o terminal no modo cbreak
    }
    else {
      tecla[0] = randtecla(wall, surroundings);
    }

    tecla[1] = '\0';

  } while (tecla[0] != 'q' && tecla[0] != 'Q');

  if (surroundings[6]==3) printf("\n\n%s\n", "PARABÉNS! CHEGOU AO OBJETIVO!");
  sendto(sock,tecla, sizeof(tecla), 0, hp->ai_addr,hp->ai_addrlen);

  close(sock);
  exit(0 );
}
