#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SHAPE_SIZE 3
#define MAX_SHAPES 10

typedef struct {
  int cells[SHAPE_SIZE][SHAPE_SIZE];
} Shape;

void printShape(Shape* shape, int idx) {
  printf("%d:\n", idx);
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      if(shape->cells[i][j] == 1) printf("#");
      else printf(".");
    }
    printf("\n");
  }
  printf("\n");
}

int NW;
unsigned long long* board;
unsigned long long* masks[MAX_SHAPES];
int nPlace[MAX_SHAPES];
int shapeCells[MAX_SHAPES];
int cnts[MAX_SHAPES];
int order[MAX_SHAPES];
int nGroups;

void rot90(int in[3][3], int out[3][3]) {
  for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) out[i][j] = in[2-j][i];
}

int sameGrid(int a[3][3], int b[3][3]) {
  for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) if(a[i][j] != b[i][j]) return 0;
  return 1;
}

int rec(int gi, int k, int start, int need, int freeCells){
  if(gi == nGroups) return 1;
  int s = order[gi];
  if(k == cnts[s]) return rec(gi+1, 0, 0, need, freeCells);

  if(need > freeCells) return 0;

  int sc = shapeCells[s];

  for(int i = start; i < nPlace[s]; i++) {
    unsigned long long* m = masks[s] + (size_t)i*NW;
    int ok = 1;
    for(int w = 0; w < NW; w++) if(board[w] & m[w]) { ok = 0; break; }
    if(!ok) continue;
  
    for(int w = 0; w < NW; w++) board[w] |= m[w];

    if(rec(gi, k+1, i+1, need - sc, freeCells - sc)) {
      for(int w = 0; w < NW; w++) board[w] &= ~m[w];
      return 1;
    }
    for(int w = 0; w < NW; w++) board[w] &= ~m[w];
  }
  return 0;
}

int solve(int width, int height, int counts[], int numShapes, Shape shapes[]) {
  int cells = width * height;
  NW = (cells + 63) / 64;

  long long need = 0;
  for(int s = 0; s < numShapes; s++) {
    int c = 0;
    for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) c += shapes[s].cells[i][j];
    shapeCells[s] = c;
    cnts[s] = counts[s];
    need += (long long)c * counts[s];
  }
  if(need > cells) return 0;

  for(int s = 0; s < numShapes; s++) {
    masks[s] = NULL;
    nPlace[s] = 0;
  }

  int okAll = 1;

  for(int s = 0; s < numShapes && okAll; s++) {
    if(counts[s] == 0 || shapeCells[s] == 0) continue;

    int cap = 0;
    int rots[4][3][3];
    memcpy(rots[0], shapes[s].cells, sizeof(rots[0]));

    for(int r = 1; r < 4; r++) rot90(rots[r-1], rots[r]);

    int nrot = 0;
    int uniq[4][3][3];

    for(int r = 0; r < 4; r++) {
      int dup = 0;
      for(int u = 0; u < nrot; u++) if(sameGrid(rots[r], uniq[u])) { dup=1; break; }
      if(!dup) {memcpy(uniq[nrot], rots[r], sizeof(rots[r])); nrot++; }
    }
  
    
    for(int u = 0; u < nrot; u++) {
      int r0=3,r1=-1,c0=3,c1=-1;
      for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) if(uniq[u][i][j]) {
        if(i < r0) r0 = i;
        if(i > r1) r1 = i;
        if(j < c0) c0 = j;
        if(j > c1) c1 = j;
      }

      int bh = r1-r0+1, bw = c1-c0+1;

      for(int oy=0; oy+bh <= height; oy++) {
        for(int ox=0; ox+bw <= width; ox++) {
          if(nPlace[s] >= cap) {
            cap = cap ? cap*2 : 16;
            masks[s] = realloc(masks[s], (size_t)cap*NW*sizeof(unsigned long long));
          }
          unsigned long long* m = masks[s] + (size_t)nPlace[s]*NW;
          memset(m,0,NW*sizeof(unsigned long long));
          for(int i = r0; i <= r1; i++) for(int j = c0; j <= c1; j++) if(uniq[u][i][j]) {
            int y = oy + (i-r0), x = ox + (j-c0);
            int bit = y*width + x;
            m[bit>>6] |= 1ULL << (bit & 63);
          }
          nPlace[s]++;
        }
      }
      if(nPlace[s] == 0) okAll = 0;
    }
  }

  int result = 0;
  if(okAll) {
    nGroups = 0;
    for(int s = 0; s < numShapes; s++) if(cnts[s] > 0 && shapeCells[s] > 0) order[nGroups++] = s;
    for(int a = 0; a < nGroups; a++) for(int b = a+1; b < nGroups; b++)
      if(shapeCells[order[b]] > shapeCells[order[a]]) { int t = order[a]; order[a] = order[b]; order[b] = t; }

    board = calloc(NW, sizeof(unsigned long long));
    result = rec(0,0,0,(int) need, cells);
    free(board);
  }

  for(int s = 0; s < numShapes; s++) free(masks[s]);
  return result;
}

int main() {
  char line[1024];

  int amountShapes = 0;
  Shape shapes[MAX_SHAPES];  

  while(fgets(line, sizeof(line), stdin) != NULL) {
    if(line[0] == '\n') continue;

    if(strchr(line, 'x')) {
      for(int i = 0; i < amountShapes; i++) {
        printShape(&shapes[i], i);
      }
      int result = 0;
      do{
        int width, height;
        
        char* colon = strchr(line, ':');
        if(!colon) continue;

        if(sscanf(line, "%dx%d", &width, &height) != 2) continue;

        int counts[MAX_SHAPES] = {0};

        char* p = colon + 1;

        for(int i = 0; i < amountShapes; i++) {
          while(isspace((unsigned char)*p)) p++;

          if(*p == '\0') break;

          counts[i] = strtol(p, &p, 10);
        }
        
        result += solve(width, height, counts, amountShapes, shapes);

        printf("PROBLEM:\n");
        printf("%dx%d: ", width, height);
        for(int i = 0; i < amountShapes; i++) {
          printf("%d ", counts[i]);
        }
        printf("\n");
      } while(fgets(line, sizeof(line), stdin) != NULL);
      printf("Result: %d\n", result);
      break;
    }

    int id;
    if(sscanf(line, "%d:", &id) == 1) {
      if(id >= MAX_SHAPES) {
        fprintf(stderr, "Too many shapes\n");
        return 1;
      }

      for(int i = 0; i < SHAPE_SIZE; i++) {
        fgets(line, sizeof(line), stdin);

        for(int j = 0; j < SHAPE_SIZE; j++) {
          shapes[id].cells[i][j] = line[j] == '#' ? 1 : 0;
        }
      }

      if(id + 1 > amountShapes) {
        amountShapes = id + 1;
      }
    }
  }
  return 0;
}
