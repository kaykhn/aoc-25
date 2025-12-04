#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void task1(char** grid, int gridSize) {
  int res = 0;
  for(int i = 0; i < gridSize; i++) {
    for(int j = 0; j < strlen(grid[i]); j++) {
      int count = 0;
      if(grid[i][j] == '.') continue;
      if(i == 0 && j == 0) {
        count = 0;
      } else if(i == 0 && j == strlen(grid[i]) - 1) {
        count = 0;
      } else if(j == 0 && i == gridSize - 1) {
        count = 0;
      } else if(i == 0) {
        if(grid[i][j-1] == '@') count++;
        if(grid[i][j+1] == '@') count++;
        if(grid[i+1][j+1] == '@') count++;
        if(grid[i+1][j] == '@') count++;
        if(grid[i+1][j-1] == '@') count++;
      } else if(j == 0) {
        if(grid[i-1][j] == '@') count++;
        if(grid[i+1][j] == '@') count++;
        if(grid[i-1][j+1] == '@') count++;
        if(grid[i][j+1] == '@') count++;
        if(grid[i+1][j+1] == '@') count++;
      } else if(i == gridSize - 1){
        if(grid[i][j-1] == '@') count++;
        if(grid[i][j+1] == '@') count++;
        if(grid[i-1][j-1] == '@') count++;
        if(grid[i-1][j] == '@') count++;
        if(grid[i-1][j+1] == '@') count++;
      } else if(j == strlen(grid[i]) - 1) {
        if(grid[i-1][j] == '@') count++;
        if(grid[i+1][j] == '@') count++;
        if(grid[i-1][j-1] == '@') count++;
        if(grid[i][j-1] == '@') count++;
        if(grid[i+1][j-1] == '@') count++;
      } else {
        if(grid[i-1][j] == '@') count++;
        if(grid[i+1][j] == '@') count++;
        if(grid[i][j+1] == '@') count++;
        if(grid[i][j-1] == '@') count++;
        if(grid[i+1][j+1] == '@') count++;
        if(grid[i-1][j+1] == '@') count++;
        if(grid[i+1][j-1] == '@') count++;
        if(grid[i-1][j-1] == '@') count++;
      }
      if(count < 4) {
        res++;
      }
    }
  }
  printf("%d\n", res);
}

void task2(char** grid, int gridSize) {
  int res = 0;
  int res_old = -1;
  while(res_old != res) {
    res_old = res;
    for(int i = 0; i < gridSize; i++) {
      for(int j = 0; j < strlen(grid[i]); j++) {
        int count = 0;
        if(grid[i][j] == '.' || grid[i][j] == 'x') continue;
        if(i == 0 && j == 0) {
          count = 0;
        } else if(i == 0 && j == strlen(grid[i]) - 1) {
          count = 0;
        } else if(j == 0 && i == gridSize - 1) {
          count = 0;
        } else if(i == 0) {
          if(grid[i][j-1] == '@') count++;
          if(grid[i][j+1] == '@') count++;
          if(grid[i+1][j+1] == '@') count++;
          if(grid[i+1][j] == '@') count++;
          if(grid[i+1][j-1] == '@') count++;
        } else if(j == 0) {
          if(grid[i-1][j] == '@') count++;
          if(grid[i+1][j] == '@') count++;
          if(grid[i-1][j+1] == '@') count++;
          if(grid[i][j+1] == '@') count++;
          if(grid[i+1][j+1] == '@') count++;
        } else if(i == gridSize - 1){
          if(grid[i][j-1] == '@') count++;
          if(grid[i][j+1] == '@') count++;
          if(grid[i-1][j-1] == '@') count++;
          if(grid[i-1][j] == '@') count++;
          if(grid[i-1][j+1] == '@') count++;
        } else if(j == strlen(grid[i]) - 1) {
          if(grid[i-1][j] == '@') count++;
          if(grid[i+1][j] == '@') count++;
          if(grid[i-1][j-1] == '@') count++;
          if(grid[i][j-1] == '@') count++;
          if(grid[i+1][j-1] == '@') count++;
        } else {
          if(grid[i-1][j] == '@') count++;
          if(grid[i+1][j] == '@') count++;
          if(grid[i][j+1] == '@') count++;
          if(grid[i][j-1] == '@') count++;
          if(grid[i+1][j+1] == '@') count++;
          if(grid[i-1][j+1] == '@') count++;
          if(grid[i+1][j-1] == '@') count++;
          if(grid[i-1][j-1] == '@') count++;
        }
        if(count < 4) {
          res++;
          grid[i][j] = 'x';
        }
      }
    }
  }
  printf("%d\n", res);
}

int main() {
  char** grid = (char**)malloc(10 * sizeof(char*));
  char line[1000];
  int count = 0;
  int gridSize = 10;
  while(fgets(line, sizeof(line), stdin) != NULL) {
    char* p = line;
    while(*p && *p != '\n') p++;
    if(*p == '\n') *p = '\0';
    if(count == gridSize) {
      gridSize *= 2;
      grid = realloc(grid, gridSize * sizeof(char*));
    }
    grid[count] = (char*)malloc(strlen(line) + 1);
    memcpy(grid[count], line, strlen(line) + 1);
    count++;
  }
  task2(grid, count); 
  for(int i = 0; i < count; i++) free(grid[i]);
  free(grid);
  return 0;
}
