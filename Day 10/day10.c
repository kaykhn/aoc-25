#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Machine {
  int* currentState;
  int* goalState;
  int numLights;
  int** buttons;
  int* buttonSizes;
  int numButtons;
  int* jolReq;
  int numJolReq;
} machine;

int* parseIntList(const char* line, int* pos, char close, int* outCount) {
  int cap = 4;
  int n = 0;
  int* arr = malloc(cap * sizeof(int));
  while(line[*pos] != '\0' && line[*pos] != close) {
    if(line[*pos] >= '0' && line[*pos] <= '9') {
      int val = 0;
      while(line[*pos] >= '0' && line[*pos] <= '9') {
        val = val * 10 + (line[*pos] - '0');
        (*pos)++;
      }
      if (n >= cap) {
        cap *= 2;
        arr = realloc(arr, cap * sizeof(int));
      }
      arr[n++] = val;
    } else {
      (*pos)++;
    }
  }
  if(line[*pos] == close) (*pos)++;
  *outCount = n;
  return arr;
}

machine parseMachine(char* line) {
  machine m = {0};
  
  // States
  int pos = 0;
  while(line[pos] != '[' && line[pos] != '\0') {
    pos++;
  }
  if(line[pos] == '\0') return m;
  pos++;
  
  int allocedState = 5;
  int* goalState = (int*) malloc(allocedState * sizeof(int));
  int stateSize = 0;  
  while(line[pos] != ']' && line[pos] != '\0') {
    stateSize++;
    if(stateSize >= allocedState) {
      allocedState *= 2;
      goalState = (int*) realloc(goalState, allocedState * sizeof(int));
    }
    if(line[pos] == '.') {
      goalState[stateSize - 1] = 0;
    } else {
      goalState[stateSize - 1] = 1;
    }
    pos++;
  }

  if(line[pos] == ']') pos++;

  m.goalState = (int*) malloc(stateSize * sizeof(int));
  m.currentState = (int*) malloc(stateSize * sizeof(int));
  for(int i = 0; i < stateSize; i++) {
    m.goalState[i] = goalState[i];
    m.currentState[i] = 0;
  }
  m.numLights = stateSize;
  free(goalState);

   
  // Buttons
  int buttonsAllocated = 5;
  m.numButtons = 0;
  m.buttons = malloc(buttonsAllocated * sizeof(int*));
  m.buttonSizes = malloc(buttonsAllocated * sizeof(int));
  while(1) {
    while(line[pos] != '\0' && line[pos] != '(' && line[pos] != '{') pos++;
    if(line[pos] != '(') break;
    pos++;

    if(m.numButtons >= buttonsAllocated) {
      buttonsAllocated *= 2;
      m.buttons = realloc(m.buttons, buttonsAllocated * sizeof(int*));
      m.buttonSizes = realloc(m.buttonSizes, buttonsAllocated * sizeof(int));
    }
    int sz;
    int* idx = parseIntList(line, &pos, ')', &sz);
    m.buttons[m.numButtons] = idx;
    m.buttonSizes[m.numButtons] = sz;
    m.numButtons++;
  }


  // Reqs
  while(line[pos] != '\0' && line[pos] != '{') pos++;
  if(line[pos] == '{') {
    pos++;
    m.jolReq = parseIntList(line, &pos, '}', &m.numJolReq);
  }

  return m;
}

void freeMachine(machine* m) {
  free(m->currentState);
  free(m->goalState);
  for(int i = 0; i < m->numButtons; i++) free(m->buttons[i]);
  free(m->buttons);
  free(m->buttonSizes);
  free(m->jolReq);
}

void printMachine(const machine* m) {
    printf("state (goal):");
    for (int i = 0; i < m->numLights; i++) printf(" %d", m->goalState[i]);
    printf("\n%d buttons:", m->numButtons);
    for (int i = 0; i < m->numButtons; i++) {
        printf(" [");
        for (int j = 0; j < m->buttonSizes[i]; j++)
            printf("%s%d", j ? "," : "", m->buttons[i][j]);
        printf("]");
    }
    printf("\njolReq:");
    for (int i = 0; i < m->numJolReq; i++) printf(" %d", m->jolReq[i]);
    printf("\n\n");
}

long long solveMachine(const machine* m) {
  int l = m->numLights;

  unsigned long long goal = 0;
  for(int i = 0; i < l; i++) {
    if(m->goalState[i]) goal |= (1ULL << i);
  }

  unsigned long long* bmask = malloc(m->numButtons * sizeof(unsigned long long));
  for(int i = 0; i < m->numButtons; i++) {
    unsigned long long mask = 0;
    for(int j = 0; j < m->buttonSizes[i]; j++) {
      mask |= (1ULL << m->buttons[i][j]);
    }
    bmask[i] = mask;
  }

  size_t total = (size_t) 1 << l;
  int* dist = malloc(total * sizeof(int));
  unsigned long long* queue = malloc(total * sizeof(unsigned long long));

  for(size_t i = 0; i < total; i++) dist[i] = -1;

  size_t head = 0;
  size_t tail = 0;
  dist[0] = 0;
  queue[tail++] = 0;
  while(head < tail) {
    unsigned long long s = queue[head++];
    if(s == goal) break;
    for(int i = 0; i < m->numButtons; i++) {
      unsigned long long ns = s ^ bmask[i];
      if(dist[ns] == -1) {
        dist[ns] = dist[s] + 1;
        queue[tail++] = ns;
      }
    }
  }
  long long answer = dist[goal];
  
  free(bmask);
  free(dist);
  free(queue);
  return answer;
}

void pivotLoop(int m, int tot, int nstruct, int phase, double* T, double* rhs, double* cost, int* basis) {
  for(int iter = 0; iter < 2000000; iter++) {
    int enter = -1;
    for(int j = 0; j < tot; j++) {
      if(phase == 2 && j >= nstruct) continue;
      
      double rc = cost[j];
      for(int i = 0; i < m; i++) rc -= cost[basis[i]] * T[i*tot + j];
      if(rc < -1e-9) {
        enter = j;
        break;
      }
    }
    if(enter < 0) return;
    int leave = -1;
    double best = 0;
    for(int i = 0; i < m; i++) {
      double a = T[i*tot + enter];
      if(a > 1e-9) {
        double ratio = rhs[i] / a;
        if(leave < 0 || ratio < best - 1e-12 || (fabs(ratio - best) < 1e-12 && basis[i] < basis[leave])) {
          best = ratio;
          leave = i;
        }
      }
    }
    if(leave < 0) return;
    double piv = T[leave*tot + enter];
    for(int j = 0; j < tot; j++) T[leave*tot + j] /= piv;
    rhs[leave] /= piv;
    for(int i = 0; i < m; i++) if(i != leave) {
      double f = T[i*tot + enter];
      if(fabs(f) > 1e-15) {
        for(int j = 0; j < tot; j++) T[i*tot + j] -= f * T[leave*tot + j];
        rhs[i] -= f * rhs[leave];
      }
    }
    basis[leave] = enter;
  }
}

int simplex(int m, int n, double* M, double* r, double* c, double* x, double* obj) {
  int tot = n + m;
  double* T = calloc((size_t)m * tot, sizeof(double));
  double* rhs = calloc(m, sizeof(double));
  double* cost = calloc(tot, sizeof(double));
  int* basis = malloc(m * sizeof(int));

  for(int i = 0; i < m; i++) {
    for(int j = 0; j < n; j++) {
      T[i*tot + j] = M[i*n +j];
    }
    T[i*tot + (n + i)] = 1.0;
    rhs[i] = r[i];
    basis[i] = n + i;
  }

  for(int j = 0; j < n; j++) cost[j] = 0;

  for(int j = n; j < tot; j++) cost[j] = 1;
  pivotLoop(m, tot, n, 1, T, rhs, cost, basis);

  double p1 = 0;
  for(int i = 0; i < m; i++) {
    p1 += cost[basis[i]] * rhs[i];
  }
  if(p1 > 1e-6) {
    free(T);
    free(rhs);
    free(cost);
    free(basis);
    return 0;
  }

  for(int i = 0; i < m; i++) if(basis[i] >= n) {
    int col = -1;
    for(int j = 0; j < n; j++) if(fabs(T[i*tot + j])> 1e-7) {
      col = j;
      break;
    }
    if(col >= 0) {
      double piv = T[i*tot + col];
      for(int j = 0; j < tot; j++) T[i*tot + j] /= piv;
      rhs[i] /= piv;
      for(int j = 0; j < m; j++) if(j != i) {
        double f = T[j*tot + col];
        if(fabs(f) > 1e-15) {
          for(int k = 0; k < tot; k++) T[j*tot + k] -= f * T[i*tot + k];
          rhs[j] -= f * rhs[i];
        }
      }
      basis[i] = col;
    }
  }

  for(int j = 0; j < n; j++) cost[j] = c[j];
  for(int j = n; j < tot; j++) cost[j] = 0;
  pivotLoop(m,tot,n,2,T,rhs,cost,basis);

  for(int j = 0; j < n; j++) x[j] = 0;
  double o = 0;
  for(int i = 0; i < m; i++) if(basis[i] < n) {
    x[basis[i]] = rhs[i];
    o += c[basis[i]] * rhs[i];
  }
  *obj = o;
  
  free(T);
  free(rhs);
  free(cost);
  free(basis);

  return 1;
}

typedef struct {
  int var;
  int type;
  int bound;
} Branch;

const machine* GM;
long long BEST;

void bnb(Branch* br, int nbr) {
  int B = GM->numButtons;
  int C = GM->numJolReq;
  int n = B + nbr;
  int m = C + nbr;
  double* M = calloc((size_t)m * n, sizeof(double));
  double* r = calloc(m, sizeof(double));
  double* c = calloc(n, sizeof(double));
  double* x = calloc(n, sizeof(double));
  for(int i = 0; i < B; i++) c[i] = 1.0;
  
  for(int i = 0; i < B; i++) {
    for(int j = 0; j < GM->buttonSizes[i]; j++) {
      int k = GM->buttons[i][j];
      if(k >= 0 && k < C) M[k*n + i] += 1.0;
    }
  }
  for(int j = 0; j < C; j++) r[j] = GM->jolReq[j];
  for(int t = 0; t < nbr; t++) {
    int row = C + t;
    M[row*n + br[t].var] = 1.0;
    M[row*n + (B + t)] = (br[t].type == 0) ? 1.0 : -1.0;
    r[row] = br[t].bound;
  }

  double obj;
  int feas = simplex(m,n,M,r,c,x,&obj);
  if(!feas) {
    free(M);
    free(r);
    free(c);
    free(x);
    return;
  }
  
  long long lb = (long long)ceil(obj - 1e-6);
  if(lb >= BEST) {
    free(M);
    free(r);
    free(c);
    free(x);
    return;
  }

  int frac = -1;
  double bestf = 0;
  for(int i = 0; i < B; i++) {
    double d = x[i] - floor(x[i]);
    double dd = (d < 0.5) ? d : 1 - d;
    if(dd > 1e-6 && dd > bestf) {
      bestf = dd;
      frac = i;
    }
  }
  
  if(frac < 0) {
    long long v = (long long)llround(obj);
    if(v < BEST) BEST = v;
    free(M);
    free(r);
    free(c);
    free(x);
    return;
  }

  int f = (int)floor(x[frac]);
  free(M);
  free(r);
  free(c);
  free(x);

  Branch* down = malloc((nbr + 1) * sizeof(Branch));
  memcpy(down, br, nbr * sizeof(Branch));
  down[nbr] = (Branch){frac, 0, f};
  bnb(down, nbr + 1);
  free(down);

  Branch* up = malloc((nbr + 1) * sizeof(Branch));
  memcpy(up, br, nbr * sizeof(Branch));
  up[nbr] = (Branch){frac, 1, f+1};
  bnb(up, nbr + 1);
  free(up);
}

long long solveJoltage(const machine* m) {
  if(m->numJolReq == 0) return 0;
  GM = m;
  BEST = LLONG_MAX;
  bnb(NULL, 0);
  return BEST;
}

int main() {
  int machineArraySize = 100;
  machine* machines = (machine*) malloc(machineArraySize * sizeof(machine));
  int numMachines = 0;
  char line[1024];
  while(fgets(line, sizeof(line), stdin) != NULL) {
    machine m = parseMachine(line);
    if(m.numLights == 0) continue;
    if(numMachines >= machineArraySize) {
      machineArraySize *= 2;
      machines = realloc(machines, machineArraySize * sizeof(machine));
    }
    machines[numMachines++] = m;
  }

  // for(int i = 0; i < numMachines; i++) printMachine(&machines[i]);

  long long total = 0;
  for(int i = 0; i < numMachines; i++) total += solveMachine(&machines[i]);
  printf("Part 1: %lld\n", total);

  total = 0;
  for(int i = 0; i < numMachines; i++) total += solveJoltage(&machines[i]);
  printf("Part 2: %lld\n", total);

  for(int i = 0; i < numMachines; i++) freeMachine(&machines[i]);
  free(machines);
  return 0;
}
