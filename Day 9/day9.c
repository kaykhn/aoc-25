#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a,b) ((a < b) ? (b) : (a))
#define min(a,b) ((a < b) ? (a) : (b))

void task1(int* x, int* y, int size) {
  long long max = -1;
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      int dx = x[i] - x[j];
      dx = (dx < 0) ? -dx : dx;
      dx++;
      int dy = y[i] - y[j];
      dy = (dy < 0) ? -dy : dy;
      dy++;
      long long area = (long long)dx * dy;
      if(area > max) {
        max = area;
      }
    }
  }
  printf("%lld\n", max);
}

int find(int* x, int size, int v) {
  for(int i = 0; i < size; i++) {
    if(x[i] == v) return i;
  }
  return -1;
}

int isInside(int** grid, int size1, int size2, int x, int y) {
  int has = 0;
  for(int i = x; i >= 0; i--) {
    if(grid[i][y] == 1) {
      has = 1;
      break;
    }
  }
  if(!has) return 0;
  has = 0;
  for(int i = x; i < size1; i++) {
    if(grid[i][y] == 1) {
      has = 1;
      break;
    }
  }
  if(!has) return 0;
  has = 0;
  for(int i = y; i >= 0; i--) {
    if(grid[x][i] == 1) {
      has = 1;
      break;
    }
  }
  if(!has) return 0;
  has = 0;
  for(int i = y; i < size2; i++) {
    if(grid[x][i] == 1) {
      has = 1;
      break;
    }
  }
  return has;
}

void fill_grid(int** grid, int size1, int size2) {
  for(int i = 0; i < size1; i++) {
    for(int j = 0; j < size2; j++) {
      if(isInside(grid, size1, size2, i, j)) {
        grid[i][j] = 1;
      } else {
        grid[i][j] = 0;
      }
    }
  }
}

int cmp_int(const void *a, const void* b) {
  int x = *(const int*)a;
  int y = *(const int*)b;
  return x - y;
}

int isValid(int idxX, int idxY, int** grid, int* uniX, int size_x, int* uniY, int size_y, int* x, int* y) {
  int cx1 = find(uniX, size_x, x[idxX]) * 2;
  int cy1 = find(uniY, size_y, y[idxX]) * 2;
  int cx2 = find(uniX, size_x, x[idxY]) * 2;
  int cy2 = find(uniY, size_y, y[idxY]) * 2;

  int temp = cx1;
  cx1 = min(cx1, cx2);
  cx2 = max(temp, cx2);

  temp = cy1;
  cy1 = min(cy1,cy2);
  cy2 = max(temp, cy2);

  for(int i = cx1; i <= cx2; i++) {
    for(int j = cy1; j <= cy2; j++) {
      if(grid[i][j] == 0) return 0;
    }
  }
  return 1;
}

void task2(int* x, int* y, int size) {
  int* ox = malloc(size * sizeof(int));
  int* oy = malloc(size * sizeof(int));

  memcpy(ox, x, size * sizeof(int));
  memcpy(oy, y, size * sizeof(int));

  qsort(ox, size, sizeof(int), cmp_int);
  qsort(oy, size, sizeof(int), cmp_int);

  int unique_x = 1;
  int unique_y = 1;
  for(int i = 1; i < size; i++) {
    if(ox[i] != ox[i-1]) unique_x++;
    if(oy[i] != oy[i-1]) unique_y++;
  }

  int* uniX = malloc(unique_x * sizeof(int));
  int* uniY = malloc(unique_y * sizeof(int));
  
  uniX[0] = ox[0];
  uniY[0] = oy[0];
  int idxX = 1;
  int idxY = 1;
  for(int i = 1; i < size; i++){
    if(ox[i] != ox[i-1]) uniX[idxX++] = ox[i];
    if(oy[i] != oy[i-1]) uniY[idxY++] = oy[i];
  }

  int* x_sizes = calloc(unique_x * 2 - 1, sizeof(int));
  int* y_sizes = calloc(unique_y * 2 - 1, sizeof(int));

  int x_sizes_size = unique_x * 2 - 1;
  int y_sizes_size = unique_y * 2 - 1;

  for(int i = 0; i < unique_x - 1; i++) {
    x_sizes[i * 2 + 1] = uniX[i + 1] - uniX[i] - 1;
  }
  for(int i = 0; i < unique_y - 1; i++) {
    y_sizes[i * 2 + 1] = uniY[i + 1] - uniY[i] - 1;
  }

  for(int i = 0; i < x_sizes_size; i++) {
    if(x_sizes[i] == 0) x_sizes[i] = 1;
  }

  for(int i = 0; i < y_sizes_size; i++) {
    if(y_sizes[i] == 0) y_sizes[i] = 1;
  }

  int** grid = malloc((unique_x * 2 - 1) * sizeof(int*));
  for(int i = 0; i < unique_x * 2 - 1; i++) {
    grid[i] = malloc((unique_y * 2 - 1) * sizeof(int));
  }

  for(int i = 0; i < size; i++) {
    int j = (i + 1) % size;
    int cx1 = find(uniX, unique_x, x[i]) * 2;
    int cx2 = find(uniX, unique_y, x[j]) * 2;
    int temp = min(cx1,cx2);
    cx2 = max(cx1,cx2);
    cx1 = temp;

    int cy1 = find(uniY, size, y[i]) * 2;
    int cy2 = find(uniY, size, y[j]) * 2;
    temp = min(cy1,cy2);
    cy2 = max(cy1,cy2);
    cy1 = temp;

    
    for(int j = cx1; j <= cx2; j++) {
      for(int k = cy1; k <= cy2; k++) {
        grid[j][k] = 1;
      }
    }
  }

  fill_grid(grid, unique_x * 2 - 1, unique_y * 2 - 1);

  long long max = -1;

  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      int xi = x[i];
      int yi = y[i];
      int xj = x[j];
      int yj = y[j];

      int dx = xi - xj;
      dx = (dx > 0) ? dx : -dx;
      dx++;
      int dy = yi - yj;
      dy = (dy > 0) ? dy : -dy;
      dy++;
      long long area = (long long)(dx * dy);
      if(area > max) {
        if(isValid(i,j,grid,uniX,unique_x,uniY,unique_y,x,y)) {
          max = area;
        }
      }
    }
  }
  
  printf("%lld\n", max);

  for(int i = 0;i < unique_x * 2 - 1; i++) free(grid[i]);
  free(grid);
  free(ox);
  free(oy);
  free(uniX);
  free(uniY);
  free(x_sizes);
  free(y_sizes);
}

int main() {
  int allocedSize = 10;
  int* x = (int*)malloc(allocedSize * sizeof(int));
  int* y = (int*)malloc(allocedSize * sizeof(int));
  int size = 0;
  while(scanf("%d,%d", &y[size], &x[size]) == 2) {
    size++;
    if(size == allocedSize) {
      allocedSize *= 2;
      x = realloc(x, allocedSize * sizeof(int));
      y = realloc(y, allocedSize * sizeof(int));
    }
  }

  task2(x,y,size);

  free(x);
  free(y);

  return 0;
}
