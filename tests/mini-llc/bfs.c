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

void visit(int v) {
    printf("%d\n", v);
}

void bfs(int s) {
    int *visited = malloc(n * sizeof(int));
    memset(visited, 0, n * sizeof(int));

    int *queue = malloc(n * sizeof(int));
    int head = 0, tail = 0;

    visit(s);
    visited[s] = 1;
    queue[tail++] = s;

    while (head != tail) {
        int u = queue[head++];
        for (int e = adj[u]; e != -1; e = next[e]) {
            int v = to[e];
            if (!visited[v]) {
                visit(v);
                visited[v] = 1;
                queue[tail++] = v;
            }
        }
    }

    free(queue);
    free(visited);
}

int main(void) {
    n = 9;
    m = 12;

    adj = malloc(n * sizeof(int));
    memset(adj, -1, n * sizeof(int));

    to = malloc(m * sizeof(int));
    next = malloc(m * sizeof(int));

    add_edge(0, 0, 1);
    add_edge(1, 1, 2);
    add_edge(2, 3, 4);
    add_edge(3, 4, 5);
    add_edge(4, 6, 7);
    add_edge(5, 7, 8);
    add_edge(6, 0, 3);
    add_edge(7, 1, 4);
    add_edge(8, 2, 5);
    add_edge(9, 3, 6);
    add_edge(10, 4, 7);
    add_edge(11, 5, 8);

    bfs(0);

    free(next);
    free(to);

    free(adj);

    return 0;
}
