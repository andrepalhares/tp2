#include <stdio.h>
#include <stdlib.h>

struct Vertice {
    int seguinte;
    int peso;
    struct Vertice* proximo;
};

struct Lista {
    struct Vertice *cabeca;
};

struct Grafo {
    int qte_vertices;
    struct Lista* sequencia;
};

struct Vertice* adiciona_vertice(int seguinte, int distancia) {
    struct Vertice* no_criado = (struct Vertice*) malloc(sizeof(struct Vertice));
    no_criado->seguinte = seguinte;
    no_criado->peso = distancia;
    no_criado->proximo = NULL;
    return no_criado;
}

struct Grafo* cria_grafo(int qte_vertices) {
    struct Grafo* grafo_criado = (struct Grafo*) malloc(sizeof(struct Grafo));
    grafo_criado->qte_vertices = qte_vertices;
    grafo_criado->sequencia = (struct Lista*) malloc(qte_vertices * sizeof(struct Lista));

    for (int i = 0; i < qte_vertices; ++i) {
        grafo_criado->sequencia[i].cabeca = NULL;
    }

    return grafo_criado;
}

void adiciona_aresta(struct Grafo* grafo, int a, int b, int distancia) {
    struct Vertice* no_criado = adiciona_vertice(a, distancia);
    no_criado->proximo = grafo->sequencia[b].cabeca;
    grafo->sequencia[b].cabeca = no_criado;

    no_criado = adiciona_vertice(b, distancia);
    no_criado->proximo = grafo->sequencia[a].cabeca;
    grafo->sequencia[a].cabeca = no_criado;
}

void imprime(struct Grafo* grafo)
{
    for (int v = 0; v < grafo->qte_vertices; v++)
    {
        struct Vertice* no_aux = grafo->sequencia[v].cabeca;
        printf("%d: ", v);
        while (no_aux)
        {
            printf("-> %d (%d) ", no_aux->seguinte, no_aux->peso);
            no_aux = no_aux->proximo;
        }
        printf("\n");
    }
}

// Driver program to test above functions
int main()
{
    // create the graph given in above fugure
    int V = 5;
    struct Grafo* graph = cria_grafo(V);
    adiciona_aresta(graph, 0, 1, 5);
    adiciona_aresta(graph, 0, 4, 7);
    adiciona_aresta(graph, 1, 2, 6);
    adiciona_aresta(graph, 1, 3, 11);
    adiciona_aresta(graph, 1, 4, 99);
    adiciona_aresta(graph, 2, 3, 52);
    adiciona_aresta(graph, 3, 4, 75);

    // print the adjacency list representation of the above graph
    imprime(graph);

    return 0;
}
