#include<stdio.h>

void task1() {
  char dir;
  int num;
  int res = 0;
  int curr_num = 50;
  while(scanf(" %c%d", &dir, &num) == 2) {
    switch(dir) {
      case 'L':
        curr_num = (curr_num - num + 100) % 100;
        break;
      case 'R':
        curr_num = (curr_num + num) % 100;
        break;
      default:
        printf("Invalid direction\n");
    }
    if(curr_num == 0) res++;
  }
  printf("%d\n", res);
}

void task2() {
  char dir;
  int num;
  int res = 0;
  int curr_num = 50;
  while(scanf(" %c%d", &dir, &num) == 2) { 
    while(num >= 100) {
      res++;
      num -= 100;
    }

    switch(dir) {
      case 'L':
        if(num > curr_num && curr_num != 0) {
          res++;
        }
        curr_num = (curr_num - num + 100) % 100;
        break;
      case 'R':
        if(curr_num + num > 100) {
          res++;
        }
        curr_num = (curr_num + num) % 100;
        break;
      default:
        printf("Invalid direction\n");
    }
    if(curr_num == 0) {
      res++;
    }
  }
  printf("%d\n", res);

}

int main() {
  task2();
  return 0;
}
