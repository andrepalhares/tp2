#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

#define earthRadiusKm 6371.0
#define M_PI 3.14159265358979323846

struct Ponto {
  double x;
  double y;
};

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
    int* verificado;
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
    grafo_criado->verificado = (int *) malloc(qte_vertices * (qte_vertices - 1) * sizeof(struct Lista));

    for (int i = 0; i < qte_vertices; ++i) {
        grafo_criado->sequencia[i].cabeca = NULL;
    }

    return grafo_criado;
}

double deg2rad(double deg) {
  return ((deg * M_PI) / 180);
}

int distanceEarthKm(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return (int) 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

void adiciona_aresta(struct Grafo* grafo, int a, int b, struct Ponto* pontos) {
    int distancia = distanceEarthKm(pontos[a].x, pontos[a].y, pontos[b].x, pontos[b].y);
    struct Vertice* no_criado = adiciona_vertice(a, distancia);
    no_criado->proximo = grafo->sequencia[b].cabeca;
    grafo->sequencia[b].cabeca = no_criado;

    no_criado = adiciona_vertice(b, distancia);
    no_criado->proximo = grafo->sequencia[a].cabeca;
    grafo->sequencia[a].cabeca = no_criado;
}

void imprime(struct Grafo* grafo) {
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

struct Ponto* inicializa_pontos(int qte_vertices, char nome[64]) {
    struct Ponto* pontos = (struct Ponto*) malloc(qte_vertices * sizeof(struct Ponto));

    FILE *arq;
    arq = fopen(nome, "r");
    char line[256];
    int i = 0;
    int j = 0;
    while (fgets(line, sizeof(line), arq)) {
        i++;
        if(i <= qte_vertices ) {
          fscanf(arq, "%lf %lf", &pontos[j].x, &pontos[j].y);
          j++;
        }
    }

    return pontos;
}

void inicializa_arestas(int qte_vertices, struct Grafo* grafo, struct Ponto* pontos) {
  for(int i=0; i < qte_vertices; i++) {
    for(int a = i+1; a < qte_vertices; a++) {
      adiciona_aresta(grafo, i, a, pontos);
    }
  }
}

int qte_vertices(char nome[64]) {
  FILE *cidades;
	cidades = fopen(nome, "r");
  int qte_cidades;

	fscanf (cidades, "%d", &qte_cidades);

	fclose(cidades);

	return qte_cidades;
}

void DFS(struct Grafo* grafo, int vertice) {
    struct Vertice* temp = grafo->sequencia[vertice].cabeca;

    grafo->verificado[vertice] = 1;
    printf("Visitado %d \n", vertice);

    while(temp!=NULL) {
      int verticeConectado = temp->seguinte;

      if(grafo->verificado[verticeConectado] == 0) {
        DFS(grafo, verticeConectado);
      }
      temp = temp->proximo;
    }
}

int FOREST(struct Grafo* grafo) {
    int v;
    int n = 0;
    for (v = 0; v < grafo->qte_vertices; v++){
        grafo->verificado[v] = 0;
    }

    for (v = 0; v < grafo->qte_vertices; v++) {
        if(grafo->verificado[v]==0){
            n++;
            printf("\nComponente n%d:\n", n);
            DFS(grafo, v);
            printf("\n");
        }
    }
    return n;
}


int main(int argc, char ** argv) {
    int qte_ver = qte_vertices(argv[1]);
    int qte_are = (qte_ver * (qte_ver - 1))/2;
    printf("qte vertices %d", qte_ver);
    int arr = (int) malloc(qte_ver * (qte_ver - 1) * sizeof(int));
    struct Grafo* graph = cria_grafo(qte_ver);
    struct Ponto* pontos = inicializa_pontos(qte_ver, argv[1]);
    inicializa_arestas(qte_ver, graph, pontos);

    //adicionar o algoritmo da mediana
    int mediana = 8248;

    FOREST(graph);

    //imprime(graph);

    return 0;
}
