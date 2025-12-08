#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  int x;
  int y;
  int z;
} point;

typedef struct {
  int from;
  int to;
  float dist;
} edge;

float distance(point p1, point p2) {
  float dx = (float)(p1.x - p2.x);
  float dy = (float)(p1.y - p2.y);
  float dz =(float)( p1.z - p2.z);
  return sqrtf(dx*dx + dy*dy + dz*dz);
}

int containsEdge(edge* edges, int eSize, int from, int to) {
  for(int i = 0; i < eSize; i++) {
    if(edges[i].from == from && edges[i].to == to) return 1;
    if(edges[i].from == to && edges[i].to == from) return 1;
  }
  return 0;
}

int contains(int* circuits, int cSize, int from, int to) {
  if(circuits[from] == circuits[to]) return 1; 
  return 0;
}

void merge(int* circuits, int cSize, int from, int to) {
  int a = circuits[from];
  int b = circuits[to];
  int new_circuit = (a < b) ? a : b;
  int old_circuit = (a < b) ? b : a;
  for(int i = 0; i < cSize; i++) {
    if(circuits[i] == old_circuit) {
      circuits[i] = new_circuit;
    }
  }
}

int cmpEdge(const void *a, const void *b) {
  const edge *x = a;
  const edge *y = b;
  if(x->dist < y->dist) return -1;
  else if(x-> dist > y->dist) return 1;
  else return 0;
}

int cmpNum(const void *a, const void *b) {
  long long x = *(long long*)a;
  long long y = *(long long*)b;
  if(x < y) return 1;
  else if(x > y) return -1;
  else return 0;
}

int isOne(int* circuit, int cSize) {
  int target = circuit[0];
  for(int i = 1; i < cSize; i++) {
    if(circuit[i] != target) return 0;
  }
  return 1;
}

void task2(point* points, int pSize) {
  int* circuit = (int*) malloc(pSize * sizeof(int)); 
  for(int i = 0; i < pSize; i++) {
    circuit[i] = i;
  }

  int totalEdges = pSize * (pSize - 1) / 2;
  edge* edges = (edge*)malloc(totalEdges * sizeof(edge));
  int eSize = 0;
 
  for(int i = 0; i < pSize; i++) {
    for(int j = i+1; j < pSize; j++) {
      edges[eSize++] = (edge) {i, j, distance(points[i], points[j])}; 
    }
  }

  qsort(edges, eSize, sizeof(edge), cmpEdge);

  int final_from = 0;
  int final_to = 0;
 
  for(int i = 0; i < eSize; i++) {
    edge e = edges[i];
    int from = e.from;
    int to = e.to;
    if(contains(circuit, pSize, from, to)) {
      continue;
    } else{
      merge(circuit, pSize, from, to);
      if(isOne(circuit, pSize)) {
        final_from = from;
        final_to = to;
        break;
      }
    }
  }


  long long res = (long long)points[final_from].x * points[final_to].x;

  printf("%lld\n", res);

  free(circuit);
  free(edges);

}

void task1(point* points, int pSize, int limit) { 
  int* circuit = (int*) malloc(pSize * sizeof(int)); 
  for(int i = 0; i < pSize; i++) {
    circuit[i] = i;
  }

  int totalEdges = pSize * (pSize - 1) / 2;
  edge* edges = (edge*)malloc(totalEdges * sizeof(edge));
  int eSize = 0;
 
  for(int i = 0; i < pSize; i++) {
    for(int j = i+1; j < pSize; j++) {
      edges[eSize++] = (edge) {i, j, distance(points[i], points[j])}; 
    }
  }

  qsort(edges, eSize, sizeof(edge), cmpEdge);

  int iter = 0;
 
  for(int i = 0; i < eSize; i++) {
    if(iter == limit) break;
    edge e = edges[i];
    int from = e.from;
    int to = e.to;
    if(contains(circuit, pSize, from, to)) {
      iter++;
      continue;
    } else{
      merge(circuit, pSize, from, to);
      iter++;
    }
  }


  long long res = 1;

  // Compute res...
  //
  long long* amount = (long long*)calloc(pSize, sizeof(long long));
  for(int i = 0; i < pSize; i++) {
    amount[circuit[i]]++;
  }

  qsort(amount, pSize, sizeof(long long), cmpNum);

  for(int i = 0; i < 3; i++) {
    res *= amount[i];
  }

  printf("%lld\n", res);

  free(circuit);
  free(edges);
  free(amount);
}

int main() {
  int allocedSize = 10;
  point* points = (point*) malloc(allocedSize * sizeof(point));
  int pSize = 0;
  
  int curr_x;
  int curr_y;
  int curr_z;
  while(scanf("%d,%d,%d", &curr_x, &curr_y, &curr_z) == 3) {
    if(pSize == allocedSize) {
      allocedSize *= 2;
      points = realloc(points, allocedSize * sizeof(point));
    }
    points[pSize++] = (point) { curr_x, curr_y, curr_z };
  }
  
  task2(points, pSize);

  free(points);
  return 0;
}
