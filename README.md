
# Simulador Micromouse
### Projeto de Sistemas Operativos
#### Francisco Mendonça, Gil Silva, Rui Bento (Grupo 1)
#### 2020, Informática - Redes e Multimédia, Universidade dos Açores

## Introdução
Um simulador de modelo cliente/servidor, escrito em C, do desafio Micromouse, no qual pequenos robôs tentam resolver um labirinto, chegando ao seu objetivo.

## Como configurar

### Linux:

Descarregar os ficheiros e extrair para uma pasta ou fazer clone do repositório.
Para maior conveniência, extrair os labirintos do ficheiro mazes.tar.gz para uma subpasta no mesmo diretório.
Para construir o simulador basta correr dois comandos no terminal, a partir da raiz do projeto:

    make micromouse_server
    make micromouse_client

## Como correr
O servidor e o cliente são executados em duas janelas diferentes do terminal.
Para correr o servidor, usar o comando `./micromouse_server /path/to/maze.txt {-wad}`, onde `/path/to/maze.txt` é o caminho para o ficheiro de texto do labirinto, e `-wad` é uma flag opcional para colocar no modo três teclas.
Para executar o cliente, usar o comando `./micromouse_client 127.0.0.1 41111 {-r}`, onde `-r` é uma flag opcional para colocar o cliente no modo random. `127.0.0.1` e `41111` são o endereço IP do localhost e porta do servidor. Apenas testámos no mesmo computador, portanto estes devem tomar esses valores.

## Modos de execução
### Servidor
-   **Modo WASD:** Modo de quatro teclas. O micromouse avança uma casa na direção correspondente à tecla. (sem flags)
-   **Modo WAD:** Modo de três teclas. O micromouse muda de direção com as teclas A e D e apenas avança com a tecla W. (flag `-wad`)

###  Cliente
-   **Modo Interativo:** Modo em que o cursor se move segundo a input do utilizador (sem flags)
-   **Modo Random:** Modo em que o cursor se move autonomamente e aleatoriamente (flag `-r`).
    
## Como foi implementado
Este simulador foi escrito na linguagem C e apenas foi testado no SO Xubuntu 19.10, uma distribuição do Linux.
O simulador é composto por dois programas: o cliente e o servidor. O servidor lê um ficheiro de texto que contém o labirinto (deve seguir as regras apresentadas na secção Labirintos), gera uma matriz correspondente e apresenta-o no terminal, ficando à espera de input do cliente. O cliente é um programa single-thread que envia os movimentos desejados pelo utilizador (ou aleatórios) para o servidor, através de um socket UDP. O servidor, ao receber inputs do cliente, processa-as, movendo o micromouse no labirinto e envia uma resposta pelo mesmo socket com o que o micromouse vê à sua volta, entre outras informações de controlo. Ao receber esta resposta, o cliente vai construindo uma matriz com o que conhece do labirinto, e vai apresentando-a ao utilizador.

O servidor implementa uma solução produtor/consumidor. A thread produtor recebe inputs do cliente e coloca-as num buffer, enquanto que a thread consumidor retira-as do buffer, processa-as, atualiza a representação do labirinto e do micromouse, e envia a resposta para o cliente.

Do lado do servidor podemos ver todo o labirinto e a posição do micromouse nele. Do lado do cliente apenas podemos ver o que já foi explorado.
 
O cliente funciona de duas formas: forma interativa, em que o cliente espera por um input do utilizador no terminal, e forma aleatória, em que o cliente gera inputs aleatórias autonomamente (com um certo delay para se poder acompanhar o movimento).

## Labirintos
Os labirintos devem ser ficheiros de texto com 32 colunas e 67 colunas, em que o espaço disponível é representado por um caráter espaço `' '`, o ponto de partida por um S maiúsculo `'S'`, os objetivos representados por um G maiúsculo `'G'` e finalmente os obstáculos por outro caractere não nulo/whitespace.
