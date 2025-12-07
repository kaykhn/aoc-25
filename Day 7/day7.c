#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void task2(char** input, int size) {  
  long long* beams = (long long*)calloc(strlen(input[0]), sizeof(long long));

  for(int i = 0; i < strlen(input[0]); i++) {
    if(input[0][i] == 'S') {
      beams[i] = 1;
    }
  }

  for(int i = 1; i < size; i++) {
    long long* newBeams = (long long*) calloc(strlen(input[i]), sizeof(long long));

    for(int j = 0; j < strlen(input[i]); j++) {
      if(beams[j] > 0) {
        if(input[i][j] == '.') {
          newBeams[j] += beams[j];
        } else if(input[i][j] == '^') {
          if(j > 0) newBeams[j-1] += beams[j];
          if(j < strlen(input[i]) - 1) newBeams[j+1] += beams[j];
        }
      }    
    }

    free(beams);
    beams = newBeams;
  }
  long long res = 0;
  for(int i = 0; i < strlen(input[0]); i++) res += beams[i];

  free(beams);
  printf("%lld\n", res);
}

void task1(char** input, int size) {
  long long res = 0;
  for(int i = 0; i < strlen(input[0]); i++) {
    if(input[0][i] == 'S') input[0][i] = 'x';
  }
  for(int i = 1; i < size; i++) {
    for(int j = 0; j < strlen(input[i]); j++) {
      if(input[i][j] == '^' && input[i-1][j] == 'x') {
        res++;
        if(j > 0 && j < strlen(input[i]) - 1) {
          input[i][j-1] = 'x';
          input[i][j+1] = 'x';
        }else if (j > 0) {
          input[i][j-1] = 'x';
        } else if (j < strlen(input[i]) - 1) {
          input[i][j+1] = 'x';
        }
      } else if(input[i][j] == '.' && input[i-1][j] == 'x') {
        input[i][j] = 'x';
      }
    }
  }
  for(int i = 0; i < size; i++) {
    printf("%s\n", input[i]);
  }
  printf("Res: %lld\n", res);
}

int main() {
  int allocSize = 10;
  char** input = (char**) malloc(allocSize * (sizeof(char*)));
  int inputSize = 0;
  char line[1000];
  while(fgets(line, sizeof(line), stdin) != NULL) {
    if(inputSize == allocSize) {
      allocSize *= 2;
      input = realloc(input, allocSize * sizeof(char*));
    }
    char* p = line;
    while(*p && *p != '\n') p++;
    if(*p == '\n') *p = '\0';
    input[inputSize] = (char*) malloc(strlen(line) * sizeof(char));
    strcpy(input[inputSize], line);
    inputSize++;
  }

  task2(input, inputSize);

  for(int i = 0; i < inputSize; i++) free(input[i]);
  free(input);

  return 0;
}
