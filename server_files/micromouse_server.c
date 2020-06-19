#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include "../shared_files/moving.h"
#include "reading.h"
#include "surroundings_server.h"
#include "pthread_wrappers.h"
#define COLUMNS 67
#define ROWS 32
#define MAX 10

// PRODUCER/CONSUMER
/*
inicialização dos mutexes
*/
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t socket_mutex = PTHREAD_MUTEX_INITIALIZER;
/*
inicialização das variáveis condição
*/
pthread_cond_t fill  = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty  = PTHREAD_COND_INITIALIZER;
/*
inicialização do buffer e das variáveis de controlo do buffer.
*/
char buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

/*
consumer_args: estrutura de dados que contém os argumentos necessários para
a thread consumidora.
*/
typedef struct consumer_args {
    int *pos;
    int *oldpos;
    int **maze;
    int mode;
    int sockfd;
    int * surroundings;
    struct sockaddr *src_addr;
    socklen_t *addrlen;
} consumer_args;

/*
producer_args: estrutura de dados que contém os argumentos necessários para
a thread produtora.
*/
typedef struct producer_args {
    int sockfd;
    struct sockaddr *src_addr;
    socklen_t *addrlen;
    int * surroundings;
    int * pos;
    int ** maze;
} producer_args;

// PRODUCER
/*
put: coloca a tecla recebida pelo cliente no buffer e atualiza as variáveis de
controlo do buffer.
*/
int put(char tecla) {
  buffer[fill_ptr] = tecla;
  fill_ptr = (fill_ptr+1) % MAX;
  count++;
}
/*
producer: recebe a tecla do cliente, espera até poder colocar dados no buffer,
coloca lá a tecla.
*/
void *producer(void *arg) {
  producer_args *args = (producer_args *) arg;
  char tecla;
  while (1){
    if (args->maze[args->pos[1]-1][args->pos[0]-1] != 3){
    // OBTER TECLA DO CLIENTE
    Pthread_mutex_lock(&socket_mutex);
    int n = recvfrom(args->sockfd, &tecla, sizeof(tecla), MSG_WAITALL,
    args->src_addr, args->addrlen);
    Pthread_mutex_unlock(&socket_mutex);
    if (tecla=='q' || tecla=='Q') break;

    // COLOCAR TECLA NO BUFFER
    Pthread_mutex_lock(&buffer_mutex);
    while (count == MAX)
      Pthread_cond_wait(&empty, &buffer_mutex);
    put(tecla);
    Pthread_cond_signal(&fill);
    Pthread_mutex_unlock(&buffer_mutex);
    }
    else
      return (void *) 0;
  }
}

//  CONSUMER
/*
get: retira os dados do buffer devolve-os. atualiza as variáveis de controlo
do buffer.
*/
char get() {
  char tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;
  return tmp;
}

/*
consumer: espera que o buffer tenha dados para processar, obtem os dados,
processa-os movendo o cursor, obtem os surroundings do rato e envia-os para
o cliente.
*/
void *consumer(void *arg) {
  char tmp;
  consumer_args *args = (consumer_args *) arg;
  while (1){
    if (args->maze[args->pos[1]-1][args->pos[0]-1] != 3) {
    Pthread_mutex_lock(&buffer_mutex);
    while (count == 0)
      Pthread_cond_wait(&fill, &buffer_mutex);
    // OBTER TECLA DO BUFFER
    tmp = get();
    Pthread_cond_signal(&empty);
    Pthread_mutex_unlock(&buffer_mutex);

    if (tmp == 'q' || tmp == 'Q') break;
    movecursor(args->maze, args->pos, args->oldpos,
      tmp, args->mode);

    int moved = hasmoved(args->pos, args->oldpos);

    getsurroundings(args->surroundings, args->maze,
      args->pos, moved, args->mode);
    memcpy(args->oldpos, args->pos, sizeof(int)*3);


    Pthread_mutex_lock(&buffer_mutex);
    sendto(args->sockfd,
      args->surroundings,
      sizeof(int)*8,
      MSG_CONFIRM, args->src_addr, *args->addrlen);
    Pthread_mutex_unlock(&buffer_mutex);
  }
  else
    return (void *) 0;
  }
}

// MAIN
/*
main: configura o socket, inicializa as variáveis e estruturas de dados,
cria as threads e espera que estas terminem a sua execução.
*/
int main(int argc, char* argv[]) {

  // CONFIGURAÇÃO DO SOCKET
  int sock, length;
  struct sockaddr_in name;
  /* Create socket from which to read. */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    perror("opening datagram socket");
    exit(1);
  }
  /* Create name with wildcards. */
  name.sin_family = AF_INET;
  name.sin_addr.s_addr = INADDR_ANY;
  //name.sin_addr.s_addr = inet_addr("127.0.0.1");
  //name.sin_port = 0;
  name.sin_port = htons(41111);
  if (bind(sock,(struct sockaddr *)&name, sizeof name) == -1) {
    perror("binding datagram socket");
    exit(1);
  }
  /* Find assigned port value and print it out. */
  length = sizeof(name);
  if (getsockname(sock,(struct sockaddr *) &name, &length) == -1) {
    perror("getting socket name");
    exit(1);
  }
  int n, len;
  struct sockaddr_in client;
  len = sizeof(client);
  // FIM DA CONFIGURAÇÃO DO SOCKET

  // CONFIGURAÇÃO DO LABIRINTO
  int mode = 0; //modo WASD por defeito
  if (argc > 2) mode = 1;
  /*
  caso o utilizador passe mais argumentos, passa para o modo WAD.
  não é o ideal, mas basta por enquanto
  */
  // inicialização das variáveis e variáveis apontador e array.
  char tecla;
  int **maze;
  int *oldpos;
  int *pos;
  int surroundings[7];

  clearscreen(); //Limpa o ecrã
  maze = ler(argc, argv); //lê o ficheiro e retorna a matriz com o labirinto
  pos = mapa(maze, mode); // inicia o movimento do cursor utilizando a matriz
  // aloca espaço para a oldpos (a posição anterior do rato)
  oldpos = malloc(sizeof(int*) * 3);
  // e copia para lá os dados da posição atual.
  memcpy(oldpos, pos, sizeof(int)*3);
  // FIM DA CONFIGURAÇÃO DO LABIRINTO

  // cria uma estrutura de dados com os argumentos para a thread produtora.
  producer_args producer_arg = {
    sock,
    (struct sockaddr *) &client,
    &len,
    surroundings,
    pos,
    maze
  };
  // cria uma estrutura de dados com os argumentos para a thread consumidora.
  consumer_args consumer_arg = {
    pos,
    oldpos,
    maze,
    mode,
    sock,
    surroundings,
    (struct sockaddr *) &client,
    &len
  };

  void *pret, *cret; // apontadores para o retorno das threads
  pthread_t p, c; // inicialização das estruturas de dados das threads
  // cria as threads
  Pthread_create(&p, producer, &producer_arg);
  Pthread_create(&c, consumer, &consumer_arg);
  // espera que as threads terminem
  Pthread_join(p, &pret);
  Pthread_join(c, &cret);
  // termina o jogo
  printf("\033[%d;%dH",ROWS+1+5,0);
  if (surroundings[6]==3) printf("\n\n\n%s\n", "PARABÉNS! CHEGOU AO OBJETIVO!");
  return 0;
}
