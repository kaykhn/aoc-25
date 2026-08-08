#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  char* name;
  struct node** next;
  int nextNodes;
}node;

node* nodes = NULL;
int nodeCount = 0;
int nodeCap = 0;
int** adj = NULL;
int* adjCount = NULL;
int* adjCap = NULL;
long long* memo = NULL;
char* have = NULL;

int findOrAdd(const char* name) {
  for(int i = 0; i < nodeCount; i++) {
    if(strcmp(nodes[i].name, name) == 0) return i;
  }
  if(nodeCount >= nodeCap) {
    nodeCap = nodeCap ? nodeCap * 2 : 8;
    nodes = realloc(nodes, nodeCap * sizeof(node));
    adj = realloc(adj, nodeCap * sizeof(int*));
    adjCount = realloc(adjCount, nodeCap * sizeof(int));
    adjCap = realloc(adjCap, nodeCap * sizeof(int));
  }

  int i = nodeCount++;
  nodes[i].name = strdup(name);
  nodes[i].next = NULL;
  nodes[i].nextNodes = 0;
  adj[i] = NULL;
  adjCount[i] = 0;
  adjCap[i] = 0;
  return i;
}

void addEdge(int from, int to) {
  if(adjCount[from] >= adjCap[from]) {
    adjCap[from] = adjCap[from] ? adjCap[from] * 2 : 4;
    adj[from] = realloc(adj[from], adjCap[from] * sizeof(int));
  }
  adj[from][adjCount[from]++] = to;
}

void printNode(node* n) {
  printf("%s:", n->name);
  for(int i = 0; i < n->nextNodes; i++) {
    printf(" %s", n->next[i]->name);
  }
  printf("\n");
}

int findAmountPaths(node* curr) {
  if(strcmp(curr->name, "out") == 0) {
    return 1;
  }
  int paths = 0;
  for(int i = 0; i < curr->nextNodes; i++) {
    paths += findAmountPaths(curr->next[i]);
  }
  return paths;
}

long long findAmountPaths2(node* curr, int hasFFT, int hasDAC) {
  if(strcmp(curr->name, "out") == 0) {
    return (hasFFT && hasDAC) ? 1 : 0;
  }

  if(strcmp(curr->name, "fft") == 0) hasFFT = 1;
  if(strcmp(curr->name, "dac") == 0) hasDAC = 1;

  int key = (int)(curr-nodes) * 4 + hasFFT * 2 + hasDAC;
  if(have[key]) return memo[key];
  long long paths = 0;
  for(int i = 0; i < curr->nextNodes; i++) {
    paths += findAmountPaths2(curr->next[i], hasFFT, hasDAC);
  }
  have[key] = 1;
  memo[key] = paths;
  return paths;
}

int main() {
  char line[1024];

  while(fgets(line, sizeof(line), stdin) != NULL) {
    char* colon = strchr(line, ':');
    if(!colon) continue;
    
    *colon = '\0';
    int from = findOrAdd(line);
    char* tok = strtok(colon + 1, " \t\r\n");
    while(tok) {
      addEdge(from, findOrAdd(tok));
      tok = strtok(NULL, " \t\r\n");
    }
  }

  for(int i = 0; i < nodeCount; i++) {
    nodes[i].nextNodes = adjCount[i];
    nodes[i].next = malloc(adjCount[i] * sizeof(node*));
    for(int j = 0; j < adjCount[i]; j++) {
      nodes[i].next[j] = &nodes[adj[i][j]];
    }
  }
  
  for(int i = 0; i < nodeCount; i++) {
    printNode(&nodes[i]);
  }
  
  int paths = 0;
  for(int i = 0; i < nodeCount; i++) {
    if(strcmp(nodes[i].name, "you") == 0) {
      paths = findAmountPaths(&nodes[i]);
      break;
    }
  } 
  printf("%d\n", paths);

  memo = calloc((size_t)nodeCount * 4, sizeof(long long));
  have = calloc((size_t)nodeCount * 4, 1);

  long long part2 = 0;
  for(int i = 0; i < nodeCount; i++) {
    if(strcmp(nodes[i].name, "svr") == 0) {
      part2 = findAmountPaths2(&nodes[i], 0, 0);
      break;
    }
  }

  printf("%lld\n", part2);

  for(int i = 0; i < nodeCount; i++) {
    free(nodes[i].name);
    free(nodes[i].next);
    free(adj[i]);
  }
  
  free(nodes);
  free(adj);
  free(adjCount);
  free(adjCap);
  free(memo);
  free(have);
  return 0;
}
