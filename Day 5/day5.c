#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_valid(long long* from, long long* to, int size, long long num) {
  for(int i = 0; i < size; i++) {
    if(from[i] <= num && to[i] >= num) return true;
  }
  return false;
}

void task1(long long* from, long long* to, int size) {
  long long curr_num;
  int res = 0;
  while(scanf(" %lld", &curr_num) == 1) {
    if(is_valid(from, to, size, curr_num)) res++;
  }
  printf("%d\n", res);
}

typedef struct {
  long long from,to;
} interval;

int cmp(const void *a, const void *b) {
  const interval *x = a;
  const interval *y = b;
  if(x->from < y->from) return -1;
  if(x->from > y->from) return 1;
  return x->to - y->to;
}

void task2(long long* from, long long* to, int size) {
  long long res = 0;

  interval* ranges = (interval*)malloc(size * sizeof(interval));
  for(int i = 0; i < size; i++) {
    ranges[i].from = from[i];
    ranges[i].to = to[i];
  }

  qsort(ranges, size, sizeof(interval), cmp);

  long long currFrom = ranges[0].from;
  long long currTo = ranges[0].to;

  for(int i = 1; i < size; i++) {
    if(ranges[i].from > currTo + 1) {
      res += (currTo - currFrom + 1);
      currFrom = ranges[i].from;
      currTo = ranges[i].to;
    } else {
      if(ranges[i].to > currTo) currTo = ranges[i].to;
    }
  }

  free(ranges);

  res += (currTo - currFrom + 1);

  printf("%lld\n", res);
}

int main() {
  long long* from = (long long*) malloc(10 * sizeof(long long));
  long long* to = (long long*) malloc(10 * sizeof(long long));
  int sizeArray = 10;
  int currSize = 0;
  char line[1000];
  while(1) {
    if(!fgets(line, sizeof(line), stdin)) break;
    if(line[0] == '\n') break;
    
    if(sscanf(line, " %lld-%lld", &from[currSize], &to[currSize]) != 2) {
      fprintf(stderr, "Invalid range: %s", line);
      continue;
    }
    currSize++;
    if(currSize >= sizeArray) {
      sizeArray *= 2;
      from = realloc(from, sizeArray * sizeof(long long));
      to = realloc(to, sizeArray * sizeof(long long));
    }
  }
  
  int ch;
  while((ch = getchar()) == '\n');
  if(ch != EOF) ungetc(ch, stdin);

  task2(from, to, currSize);

  free(from);
  free(to);

  return 0;
}
