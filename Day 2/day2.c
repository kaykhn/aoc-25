#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool isInvalid(long long i) {
  if(i< 0) return false;
  char buf[64];
  snprintf(buf, sizeof(buf), "%lld", i);
  int len = strlen(buf);
  if(len % 2 != 0) return false;
  int half = len / 2;
  return strncmp(buf, buf + half, half) == 0;
}

bool isInvalidPart2(long long i) {
  if(i<0) return false;
  char buf[64];
  snprintf(buf, sizeof(buf), "%lld", i);
  int len = strlen(buf);

  for(int block = 1; block <= len / 2; block++) {
    if(len % block != 0) continue;
    bool match = true;
    for(int j = block; j < len; j++) {
      if(buf[j] != buf[j%block]) {
        match = false;
        break;
      }
    }
    if(match) return true;
  }
  return false;
}

long long sumInvalid(long long start, long long end) {
  long long res = 0;
  for(long long i = start; i <= end; i++) {
    if(isInvalidPart2(i)) {
      res += i;
    }
  }
  return res;
}

int main() {
  char line[2048];
  if (fgets(line, sizeof(line), stdin) == NULL) return 1;
  char *token = strtok(line, ",");
  long long res = 0;
  while(token != NULL) {
    long long start, end;
    if(sscanf(token, " %lld-%lld", &start, &end) == 2) {
      if(start > end) {
        long long temp = start;
        start = end;
        end = temp;
      }
      res += sumInvalid(start, end);
    } else {
      fprintf(stderr, "Invalid interval: %s\n", token);
    }
    token = strtok(NULL, ",");
  }
  printf("%lld\n", res);
  return 0;
}
