#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define INF INT_MAX

// Função de busca em largura (BFS)
bool bfs(int** rGraph, int s, int t, int path[], int V) {

    // Cria um vetor de boleanos
    bool visited[V];
    // Preenche o vetor com 0 (falso)
    memset(visited, 0, sizeof(visited));

    // queue é uma fila, front é a frente da fila e end é o final da fila
    int queue[V], front = 0, end = 0;

    // Enfileira o vértice super fonte e marca como visitado
    queue[end++] = s;
    visited[s] = true;
    path[s] = -1;

    // Continua executando enquanto houver elementos na fila
    while (front < end) {
        // Armazena o primeiro valor da fila e deloca o inicio da fila em uma unidade
        int u = queue[front++];
        
        // Pecorre a linha u, procura os vertices com capacidade disponivel
        for (int v = 0; v < V; v++) {
            // Ao encontrar o vertice
            if (!visited[v] && rGraph[u][v] > 0) {
                // Adiciona o vertice a fila e incrementa o final da fila
                queue[end++] = v;
                // Adiciona o vertice u ao caminho na posição v e marca a posição v como visitada
                path[v] = u;
                visited[v] = true;
            }
        }
    }

    // retorna se o super sumidouro foi visitado
    return visited[t];
}

// Função para calcular o fluxo máximo usando Ford-Fulkerson
int fordFulkerson(int** graph, int s, int t, int V) {
    
    // Cria uma matriz de inteiros para representar o grafo residual
    int** rGraph = (int**)malloc(V * sizeof(int*));

    // Preenche o grafo residual com as informações do grafo
    int u, v;
    for (u = 0; u < V; u++) {
        rGraph[u] = (int*)malloc(V * sizeof(int));
        for (v = 0; v < V; v++) {
            rGraph[u][v] = graph[u][v];
        }
    }

    int path[V];
    int max_flow = 0;

    // Executa enquanto houver capacidade disponivel entre s e t
    while (bfs(rGraph, s, t, path, V)) {

        // Busca a menor capacidade de fluxo no caminho encontrado
        int flow = INF;
        for (v = t; v != s; v = path[v]) {
            u = path[v];
            if (rGraph[u][v] < flow)
                flow = rGraph[u][v];
        }
        
        // Subtrai a quantidade de fluxo encontrada da aresta uv e soma a aresta vu
        for (v = t; v != s; v = path[v]) {
            u = path[v];
            rGraph[u][v] -= flow;
            rGraph[v][u] += flow;
        }
        
        // Acumula a quantidade de fluxo utilizada, até que não seja mais possivel ter fluxo de s até t
        max_flow += flow;
    }

    // Libera o espaço alocado para a matriz rGraph
    for (u = 0; u < V; u++)
        free(rGraph[u]);
    free(rGraph);

    // Retorna o fluxo máximo
    return max_flow;
}


int main() {
    // Cria um ponteiro para o arquivo
    FILE *file = fopen("grafo.txt", "r");

    // Avisa o usuário e finaliza o programa caso o arquivo não possa ser aberto corretamente
    if (!file) {
        printf("Erro ao abrir o arquivo\n");
        return 1;
    }

    // V é número de vertices, E é o número de arestas
    int V, E;
    fscanf(file, "%d", &V);
    fscanf(file, "%d", &E);

    // num_sources é o número de fontes
    int num_sources;
    fscanf(file, "%d", &num_sources);

    // Cria um vetor de vertices fontes (identificados por um número inteiro) e preenche esse vetor
    int* sources;
    sources = (int*)malloc(num_sources * sizeof(int));
    for (int i = 0; i < num_sources; i++) {
        fscanf(file, "%d", &sources[i]);
        sources[i] -= 1;
    }

    // num_sinks é o número de sumidouros
    int num_sinks; 
    fscanf(file, "%d", &num_sinks);

    // Cria um vetor de vertices sumidouros (identificados por um número inteiro) e preenche esse vetor
    int* sinks;
    sinks = (int*)malloc(num_sinks * sizeof(int));
    for (int i = 0; i < num_sinks; i++) {
        fscanf(file, "%d", &sinks[i]);
        sinks[i] -= 1;
    }

    // O grafo é representado na forma de uma matriz de inteiros
    int** graph = (int**)malloc((V + 2) * sizeof(int*));
    for (int i = 0; i < V + 2; i++) {
        graph[i] = (int*)calloc(V + 2, sizeof(int));
    }

    // O matriz que representa o grafo é preenchida com as informações presentes no arquivo
    for (int i = 0; i < E; i++) {
        int u, v, cap;
        fscanf(file, "%d %d %d", &u, &v, &cap);
        graph[u-1][v-1] = cap;
    }

    fclose(file);

    // Cria uma super fonte e um super sumidouro
    int super_source = V;
    int super_sink = V + 1;

    // Define capacidade infinita entre as fontes e a super fonte
    for (int i = 0; i < num_sources; i++) {
        graph[super_source][sources[i]] = INF;
    }

    // Define capacidade infinita entre os sumidouros e o super sumidouro
    for (int i = 0; i < num_sinks; i++) {
        graph[sinks[i]][super_sink] = INF;
    }

    printf("O fluxo máximo é %d\n", fordFulkerson(graph, super_source, super_sink, V + 2));

    // Libera os espaços alocados para as estruturas
    for (int i = 0; i < V + 2; i++)
        free(graph[i]);
    free(graph);
    free(sources);
    free(sinks);

    return 0;
}

// gcc fluxo_maximo.c -o fluxo_maximo
//./fluxo_maximo

