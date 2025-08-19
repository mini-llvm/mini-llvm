void *malloc(unsigned long);
void free(void *);
void *memset(void *, int, unsigned long);
int printf(const char *, ...);

int n, m;
int *adj;
int *to, *weight, *next;

void add_edge(int e, int u, int v, int w) {
    to[e] = v;
    weight[e] = w;
    next[e] = adj[u];
    adj[u] = e;
}

int *heap;
int heap_size;

int *heap_idx;

int *dist;

void percolate_down(int i) {
    int tmp = heap[i];
    while (i < heap_size / 2) {
        int j = i * 2 + 1;
        if (j + 1 < heap_size && dist[heap[j + 1]] < dist[heap[j]]) ++j;
        if (dist[tmp] <= dist[heap[j]]) break;
        heap[i] = heap[j];
        heap_idx[heap[i]] = i;
        i = j;
    }
    heap[i] = tmp;
    heap_idx[heap[i]] = i;
}

void percolate_up(int i) {
    int tmp = heap[i];
    while (i > 0) {
        int j = (i - 1) / 2;
        if (dist[heap[j]] <= dist[tmp]) break;
        heap[i] = heap[j];
        heap_idx[heap[i]] = i;
        i = j;
    }
    heap[i] = tmp;
    heap_idx[heap[i]] = i;
}

int is_empty(void) {
    return heap_size == 0;
}

int find_min(void) {
    return heap[0];
}

void insert(int v, int new_dist) {
    dist[v] = new_dist;
    ++heap_size;
    heap[heap_size - 1] = v;
    heap_idx[heap[heap_size - 1]] = heap_size - 1;
    percolate_up(heap_size - 1);
}

void delete_min(void) {
    --heap_size;
    heap[0] = heap[heap_size];
    heap_idx[heap[0]] = 0;
    percolate_down(0);
}

void decrease_key(int v, int new_dist) {
    dist[v] = new_dist;
    percolate_up(heap_idx[v]);
}

void dijkstra(int s) {
    insert(s, 0);
    while (!is_empty()) {
        int u = find_min();
        delete_min();
        for (int e = adj[u]; e != -1; e = next[e]) {
            int v = to[e];
            int w = weight[e];
            if (dist[v] == -1) {
                insert(v, dist[u] + w);
            } else if (dist[v] > dist[u] + w) {
                decrease_key(v, dist[u] + w);
            }
        }
    }
}

int main(void) {
    n = 5;
    m = 10;

    adj = malloc(n * sizeof(int));
    memset(adj, -1, n * sizeof(int));

    to = malloc(m * sizeof(int));
    weight = malloc(m * sizeof(int));
    next = malloc(m * sizeof(int));

    add_edge(0, 0, 1, 3);
    add_edge(1, 0, 3, 5);
    add_edge(2, 1, 2, 6);
    add_edge(3, 1, 3, 2);
    add_edge(4, 2, 4, 2);
    add_edge(5, 3, 1, 1);
    add_edge(6, 3, 2, 4);
    add_edge(7, 3, 4, 6);
    add_edge(8, 4, 0, 3);
    add_edge(9, 4, 2, 7);

    heap = malloc(n * sizeof(int));

    heap_idx = malloc(n * sizeof(int));

    dist = malloc(n * sizeof(int));
    memset(dist, -1, n * sizeof(int));

    dijkstra(0);

    printf("%d %d %d %d %d\n", dist[0], dist[1], dist[2], dist[3], dist[4]);

    free(dist);

    free(heap_idx);

    free(heap);

    free(next);
    free(weight);
    free(to);

    free(adj);

    return 0;
}
