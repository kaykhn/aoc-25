#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool is_empty(char** input, int size, int idx) {
  for(int i = 0; i < size; i++) {
    if(input[i][idx] != ' ') return false;
  }
  return true;
}

int findNext(char** input, int size, int idx) {
  int num = -1;
  for(int i = idx; i >= 0; i--) {
    if(!is_empty(input, size, i)) {
      num = i; 
      break;
    }
  }
  return num;
}

long long computeSolutionTask2(char** input, int size, char* ops, int opsSize) {
  long long res = 0;
  int cols = 0;
  for(int i = 0; i < size; i++) {
    if(strlen(input[i]) > cols) cols = strlen(input[i]);
  }

  int opIdx = opsSize - 1;
  long long currRes = (ops[opIdx] == '+') ? 0 : 1;

  for(int i = cols - 1; i >= 0; i--) {
    if(is_empty(input, size, i)){
      opIdx--;
      res += currRes;
      currRes = (ops[opIdx] == '+') ? 0 : 1;
      i = findNext(input, size, i) + 1;
      continue;
    }
    
    long long currNum = -1;

    for(int j = 0; j < size; j++) {
      
      int curr = 0;
      if(input[j][i] != ' ') {
        curr = input[j][i] - '0';
      }
      if(currNum == -1) {
        currNum = curr;
      } else if (input[j][i] != ' '){
        currNum *= 10;
        currNum += curr;
      }
    }
    
    if(ops[opIdx] == '+') {
      currRes += currNum;
    } else {
      currRes *= currNum;
    }

  }

  int last = (ops[0] == '+') ? 0 : 1;
  if(currRes != last) res += currRes;

  return res;
}

void task2() {
  int allocatedSize = 10;
  char** input = (char**)malloc(allocatedSize * sizeof(char*));
  int inputSize = 0;
  int allocatedOpsSize = 10;
  int opsSize = 0;
  char* ops = (char*)malloc(allocatedOpsSize * sizeof(char));
  
  char line[10000];

  while(fgets(line, sizeof(line), stdin) != NULL) {
    if(strstr(line, "+") != NULL || strstr(line, "*") != NULL) {
      char* token = strtok(line, " \t\n");
      while(token != NULL) {
        if(allocatedOpsSize == opsSize) {
          allocatedOpsSize *= 2;
          ops = realloc(ops, allocatedOpsSize * sizeof(char));
        }
        ops[opsSize] = token[0];
        token = strtok(NULL, " \t\n");
        opsSize++;
      }
      break;
    }
    char* p = line;
    while(*p && *p != '\n') p++;
    if(*p == '\n') *p = '\0';
    if(inputSize == allocatedSize) {
      allocatedSize *= 2;
      input = realloc(input, allocatedSize * sizeof(char*));
    }
    input[inputSize] = (char*) malloc((strlen(line) + 1) * sizeof(char));
    strcpy(input[inputSize], line); 
    inputSize++;
  }

  long long res = computeSolutionTask2(input, inputSize, ops, opsSize);
  printf("%lld\n", res);

  for(int i = 0; i < inputSize; i++) free(input[i]);
  free(input);
  free(ops);
}

void task1() {
  int allocedSize = 10;
  long long** nums = (long long**)malloc(allocedSize * sizeof(long long*));
  int* rowSizes = (int*) malloc(allocedSize * sizeof(int));
  int colSize = 0;
  char line[10000];
  int opsSize = 10;
  char* ops = (char*) malloc(opsSize * sizeof(char));
  while(fgets(line, sizeof(line), stdin) != NULL) {
    if(strstr(line, "+") != NULL || strstr(line, "*") != NULL) {
      char* token = strtok(line, " \t\n");
      int count = 0;
      while(token != NULL) {
        if(count == opsSize) {
          opsSize *= 2;
          ops = realloc(ops, opsSize * sizeof(char));
        }
        ops[count] = token[0];
        token = strtok(NULL, " \t\n");
        count++;
      }
      opsSize = count;
      break;
    }
    char* p = line;
    while(*p && *p != '\n') p++;
    if(*p == '\n') *p = '\0';
    if(allocedSize == colSize) {
      allocedSize *= 2;
      nums = realloc(nums, allocedSize * sizeof(long long*));
      rowSizes = realloc(rowSizes, allocedSize * sizeof(int));
    }
    rowSizes[colSize] = 1;
    nums[colSize] = (long long*) malloc(rowSizes[colSize] * sizeof(long long));
    char* token = strtok(line, " ");
    int count = 0;
    while(token != NULL) {
      if(count == rowSizes[colSize]) {
        rowSizes[colSize] *= 2;
        nums[colSize] = realloc(nums[colSize], rowSizes[colSize] * sizeof(long long));
      }
      nums[colSize][count] = atoll(token);
      token = strtok(NULL, " ");
      count++;
    }
    rowSizes[colSize] = count;
    colSize++;
  }

  long long res = 0;
  for(int i = 0; i < opsSize; i++) {
    int add = ops[i] == '+';
    long long num = 0;
    if(!add) num = 1;
    for(int j = 0; j < colSize; j++) {
      if(add) num += nums[j][i];
      else num *= nums[j][i];
    }
    res += num;
  }
  printf("%lld\n", res);


  for(int i = 0; i < colSize; i++) free(nums[i]);
  free(rowSizes);
  free(nums);
  free(ops);


}

int main() {
  task2();
  return 0;
}
