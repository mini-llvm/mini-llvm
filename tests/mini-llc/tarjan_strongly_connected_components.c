void *malloc(unsigned long);
void free(void *);
void *memset(void *, int, unsigned long);
int printf(const char *, ...);

int n, m;
int *adj;
int *to, *next;

void add_edge(int e, int u, int v) {
    to[e] = v;
    next[e] = adj[u];
    adj[u] = e;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int *dfn;
int timer;

int *low;

int *scc;
int scc_cnt;

int *stack;
int top;

void tarjan(int u) {
    dfn[u] = low[u] = ++timer;
    stack[top++] = u;
    for (int e = adj[u]; e != -1; e = next[e]) {
        int v = to[e];
        if (!dfn[v]) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (!scc[v]) {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if (low[u] == dfn[u]) {
        ++scc_cnt;
        int v;
        do {
            v = stack[--top];
            scc[v] = scc_cnt;
        } while (v != u);
    }
}

void tarjan_all(void) {
    for (int v = 0; v < n; ++v) {
        if (!dfn[v]) {
            tarjan(v);
        }
    }
}

int main(void) {
    n = 8;
    m = 14;

    adj = malloc(n * sizeof(int));
    memset(adj, -1, n * sizeof(int));

    to = malloc(m * sizeof(int));
    next = malloc(m * sizeof(int));

    add_edge(0, 0, 1);
    add_edge(1, 1, 2);
    add_edge(2, 1, 4);
    add_edge(3, 1, 5);
    add_edge(4, 2, 3);
    add_edge(5, 2, 6);
    add_edge(6, 3, 2);
    add_edge(7, 3, 7);
    add_edge(8, 4, 0);
    add_edge(9, 4, 5);
    add_edge(10, 5, 6);
    add_edge(11, 6, 5);
    add_edge(12, 6, 7);
    add_edge(13, 7, 7);

    dfn = malloc(n * sizeof(int));
    memset(dfn, 0, n * sizeof(int));

    low = malloc(n * sizeof(int));

    scc = malloc(n * sizeof(int));
    memset(scc, 0, n * sizeof(int));

    stack = malloc(n * sizeof(int));

    tarjan_all();

    printf("%d %d %d %d %d %d %d %d\n",
        scc[0], scc[1], scc[2], scc[3], scc[4], scc[5], scc[6], scc[7]);

    free(stack);

    free(scc);

    free(low);

    free(dfn);

    free(next);
    free(to);

    free(adj);

    return 0;
}
