#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define earthRadiusKm 6371.0
#define M_PI 3.14159265358979323846

struct Ponto {
  double x;
  double y;
};

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

int qte_vertices(char nome[64]) {
  FILE *cidades;
	cidades = fopen(nome, "r");
  int qte_cidades;

	fscanf (cidades, "%d", &qte_cidades);

	fclose(cidades);

	return qte_cidades;
}

void inicia_pontos(char nome[64], int qte_vertices, struct Ponto cidades[]) {
  FILE *arq;
	arq = fopen(nome, "r");
  char line[256];
  int i = 0;
  int j = 0;
  while (fgets(line, sizeof(line), arq)) {
      i++;
      if(i <= qte_vertices ) {
          fscanf(arq, "%lf %lf", &cidades[j].x, &cidades[j].y);
          j++;
      }
  }
}

void inicia_arestas(char nome[64], int qte_vertices, struct Ponto cidades[], int arestas[][qte_vertices], int *max_aresta) {
  int aux_max = 0;
  //Caso 1: A diagonal da matriz é sempre 0, pois é a distância do ponto a ele mesmo
  for (int i = 0; i < qte_vertices; i++) {
    arestas[i][i] = 0;
  }

  //Caso 2: agrupamento de uma aresta i com todos os vertices i+1 até que i seja igual a qte_vertices
  for (int i = 0; i < qte_vertices-1; i++) {
    for (int j=i+1; j < qte_vertices; j++) {
      arestas[i][j] = distanceEarthKm(cidades[i].x, cidades[i].y, cidades[j].x, cidades[j].y);
      arestas[j][i] = arestas[i][j];

      if(aux_max < arestas[i][j]) {
        aux_max = arestas[i][j];
      }

    }
  }

  *max_aresta = aux_max;

}

// A utility function to find the vertex with
// minimum key value, from the set of vertices
// not yet included in MST
int minKey(int key[], bool mstSet[], int qte_ver, int max_aresta) {
	// Initialize min value
	int min = max_aresta + 1;
  int min_index;

	for (int v = 0; v < qte_ver; v++)
		if (mstSet[v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}

// A utility function to print the
// constructed MST stored in parent[]
int printMST(int parent[], int qte_ver, int graph[qte_ver][qte_ver]) {
  int max = 0;
	for (int i = 1; i < qte_ver; i++) {
    if(max < graph[i][parent[i]]) {
      max = graph[i][parent[i]];
    }
  }
  printf("%d", max);
}

// Function to construct and print MST for
// a graph represented using adjacency
// matrix representation
void primMST(int qte_ver, int graph[qte_ver][qte_ver], int max_aresta) {
	// Array to store constructed MST
	int parent[qte_ver];
	// Key values used to pick minimum weight edge in cut
	int key[qte_ver];
	// To represent set of vertices not yet included in MST
	bool mstSet[qte_ver];

	// Initialize all keys as INFINITE
	for (int i = 0; i < qte_ver; i++) {
		key[i] = max_aresta + 1;
    mstSet[i] = false;
  }

	// Always include first 1st vertex in MST.
	// Make key 0 so that this vertex is picked as first vertex.
	key[0] = 0;
	parent[0] = -1; // First node is always root of MST

	// The MST will have V vertices
	for (int count = 0; count < qte_ver-1; count++)
	{
		// Pick the minimum key vertex from the
		// set of vertices not yet included in MST
		int u = minKey(key, mstSet, qte_ver, max_aresta);

		// Add the picked vertex to the MST Set
		mstSet[u] = true;

		// Update key value and parent index of
		// the adjacent vertices of the picked vertex.
		// Consider only those vertices which are not
		// yet included in MST
		for (int v = 0; v < qte_ver; v++)

		// graph[u][v] is non zero only for adjacent vertices of m
		// mstSet[v] is false for vertices not yet included in MST
		// Update the key only if graph[u][v] is smaller than key[v]
		if (graph[u][v] && mstSet[v] == false && graph[u][v] < key[v])
			parent[v] = u, key[v] = graph[u][v];
	}

	// print the constructed MST
	printMST(parent, qte_ver, graph);
}

void main() {
  int max_aresta;
  int qte_ver = qte_vertices("cidades2.txt");
  struct Ponto cidades[qte_ver];
  int arestas[qte_ver][qte_ver];

  inicia_pontos("cidades2.txt", qte_ver, cidades);
  inicia_arestas("cidades2.txt", qte_ver, cidades, arestas, &max_aresta);

  primMST(qte_ver, arestas, max_aresta);
  
}
