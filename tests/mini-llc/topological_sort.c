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

void topological_sort(int sorted[]) {
    int *in = malloc(n * sizeof(int));
    memset(in, 0, n * sizeof(int));

    for (int u = 0; u < n; ++u) {
        for (int e = adj[u]; e != -1; e = next[e]) {
            int v = to[e];
            ++in[v];
        }
    }

    int *visited = malloc(n * sizeof(int));
    memset(visited, 0, n * sizeof(int));

    int *queue = malloc(n * sizeof(int));
    int head = 0, tail = 0;

    for (int v = 0; v < n; ++v) {
        if (in[v] == 0) {
            queue[tail++] = v;
        }
    }

    for (int i = 0; i < n; ++i) {
        int u = queue[head++];
        sorted[i] = u;
        for (int e = adj[u]; e != -1; e = next[e]) {
            int v = to[e];
            if (--in[v] == 0) {
                queue[tail++] = v;
            }
        }
    }

    free(queue);
    free(visited);
    free(in);
}

int main(void) {
    n = 8;
    m = 9;

    adj = malloc(n * sizeof(int));
    memset(adj, -1, n * sizeof(int));

    to = malloc(m * sizeof(int));
    next = malloc(m * sizeof(int));

    add_edge(0, 0, 1);
    add_edge(1, 1, 2);
    add_edge(2, 1, 5);
    add_edge(3, 1, 7);
    add_edge(4, 3, 1);
    add_edge(5, 3, 4);
    add_edge(6, 4, 5);
    add_edge(7, 6, 4);
    add_edge(8, 6, 7);

    int *sorted = malloc(n * sizeof(int));

    topological_sort(sorted);

    printf("%d %d %d %d %d %d %d %d\n",
        sorted[0], sorted[1], sorted[2], sorted[3], sorted[4], sorted[5], sorted[6], sorted[7]);

    free(sorted);

    free(next);
    free(to);

    free(adj);

    return 0;
}
