#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define INF INT_MAX

// Função de busca em largura (BFS)
bool bfs(int** rGraph, int s, int t, int parent[], int V) {
    bool visited[V];
    memset(visited, 0, sizeof(visited));

    int queue[V], front = 0, rear = 0;
    queue[rear++] = s;
    visited[s] = true;
    parent[s] = -1;

    while (front < rear) {
        int u = queue[front++];

        for (int v = 0; v < V; v++) {
            if (!visited[v] && rGraph[u][v] > 0) {
                queue[rear++] = v;
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    return visited[t];
}

// Função para calcular o fluxo máximo usando Ford-Fulkerson
int fordFulkerson(int** graph, int s, int t, int V) {
    int u, v;
    int** rGraph = (int**)malloc(V * sizeof(int*));
    for (u = 0; u < V; u++) {
        rGraph[u] = (int*)malloc(V * sizeof(int));
        for (v = 0; v < V; v++) {
            rGraph[u][v] = graph[u][v];
        }
    }

    int parent[V];
    int max_flow = 0;

    while (bfs(rGraph, s, t, parent, V)) {
        int path_flow = INF;
        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            if (rGraph[u][v] < path_flow)
                path_flow = rGraph[u][v];
        }

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        
        max_flow += path_flow;
    }

    for (u = 0; u < V; u++)
        free(rGraph[u]);
    free(rGraph);

    return max_flow;
}

// Função principal
int main() {
    FILE *file = fopen("grafo.txt", "r");
    if (!file) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    int V, E, num_sources, num_sinks;
    fscanf(file, "%d", &V);
    fscanf(file, "%d", &E);

    int* sources;
    int* sinks;
    fscanf(file, "%d", &num_sources);
    sources = (int*)malloc(num_sources * sizeof(int));
    for (int i = 0; i < num_sources; i++) {
        fscanf(file, "%d", &sources[i]);
        sources[i] -= 1;
    }

    fscanf(file, "%d", &num_sinks);
    sinks = (int*)malloc(num_sinks * sizeof(int));
    for (int i = 0; i < num_sinks; i++) {
        fscanf(file, "%d", &sinks[i]);
        sinks[i] -= 1;
    }

    int** graph = (int**)malloc((V + 2) * sizeof(int*));
    for (int i = 0; i < V + 2; i++) {
        graph[i] = (int*)calloc(V + 2, sizeof(int));
    }

    for (int i = 0; i < E; i++) {
        int u, v, cap;
        fscanf(file, "%d %d %d", &u, &v, &cap);
        graph[u-1][v-1] = cap;
    }

    fclose(file);

    int super_source = V;
    int super_sink = V + 1;

    for (int i = 0; i < num_sources; i++) {
        graph[super_source][sources[i]] = INF;
    }

    for (int i = 0; i < num_sinks; i++) {
        graph[sinks[i]][super_sink] = INF;
    }

    printf("O fluxo máximo é %d\n", fordFulkerson(graph, super_source, super_sink, V + 2));

    for (int i = 0; i < V + 2; i++)
        free(graph[i]);
    free(graph);
    free(sources);
    free(sinks);

    return 0;
}
