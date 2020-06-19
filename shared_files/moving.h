void clearscreen();

void cbreak();

void nocbreak();

int *up(int **maze, int *pos);
int *down(int **maze, int *pos);
int *right(int **maze, int *pos);
int *left(int **maze, int *pos);

int *mapa(int **maze, int mode);

void *movecursor(int **maze, int *pos, int *oldpos, char m, int mode);
