#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getNumLineTask1(char line[]) {
  int max = 0;
  int idx = 0;
  for(int i = 0; i < strlen(line) - 1; i++) { 
    int curr = line[i] - '0';
    if(curr > max) {
      max = curr;
      idx = i;
    }
  }
  int nextMax = 0;
  for(int i = idx + 1; i < strlen(line); i++) {
    int curr = line[i] - '0';
    if(curr > nextMax) nextMax = curr;
  }
  return max * 10 + nextMax;
}

long long getNumLineTask2(char line[]) {
  int nums[12] = {0};
  int idx[12] = {0};
  for(int i = 0; i < strlen(line) - 11; i++) {
    int curr = line[i] - '0';
    if(curr > nums[0]) {
      nums[0] = curr;
      idx[0] = i;
    } 
  }
  for(int i = 1; i < 12; i++) {
    for(int j = idx[i-1] + 1; j < strlen(line) - (11 - i); j++) {
      int curr = line[j] - '0';
      if(curr > nums[i]) {
        nums[i] = curr;
        idx[i] = j;
      }
    }
  }
  long long res = 0;
  for(int i = 0; i < 12; i++) {
    res = res * 10;
    res += nums[i];
  }
  return res;
}

void task() {
  char line[1000];
  long long res = 0;
  while(fgets(line, sizeof(line), stdin) != NULL) {
    char* p = line;
    while(*p && *p != '\n') p++;
    if(*p == '\n') *p = '\0';
    res += getNumLineTask2(line);
  }
  printf("%lld\n", res);
}

int main() {
  task();
  return 0;
}
