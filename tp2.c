#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tp2.h"

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

void inicia_pontos(char nome[64], int qte_vertices, struct Ponto cidades[qte_vertices]) {
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

void inicia_arestas(char nome[64], int qte_vertices, struct Ponto cidades[qte_vertices], int arestas[][qte_vertices], int *max_aresta) {
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

int minima_aresta(int menores_valores[], int vertices_fora[], int qte_ver, int maior_aresta) {
	int pos_min;
	int min = maior_aresta + 1;

	for (int v = 0; v < qte_ver; v++) {
		if (vertices_fora[v] == 0 && menores_valores[v] < min) {
			min = menores_valores[v];
			pos_min = v;
		}
	}

	return pos_min;
}

int imprime(int no_anterior[], int qte_ver, int grafo[qte_ver][qte_ver]) {
  int maior_aresta = 0;
	for (int i = 1; i < qte_ver; i++) {
    if(maior_aresta < grafo[i][no_anterior[i]]) {
      maior_aresta = grafo[i][no_anterior[i]];
    }
  }
  printf("%d", maior_aresta);
}

void prim(int qte_ver, int graph[qte_ver][qte_ver], int max_aresta) {
	int no_anterior[qte_ver];
	int menores_valores[qte_ver];
	int vertices_fora[qte_ver];

	// De início, todos os valores flag para os menores serão os maiores possíveis
	for (int i = 0; i < qte_ver; i++) {
		menores_valores[i] = max_aresta + 1;
    vertices_fora[i] = 0;
  }

	//Use o primeiro vértice como a raiz da AGM
	menores_valores[0] = 0;
	no_anterior[0] = -1;

	for (int count = 0; count < qte_ver-1; count++) {
		int u = minima_aresta(menores_valores, vertices_fora, qte_ver, max_aresta);

		// Inclui o vértice escolhido na AGM
		vertices_fora[u] = 1;

		// Se o vértice nao estiver na AGM, vamos definir os menores valores para o vértice escolhido na iteração
		for (int v = 0; v < qte_ver; v++) {
			if (graph[u][v] && vertices_fora[v] == 0 && graph[u][v] < menores_valores[v]) {
				no_anterior[v] = u;
				menores_valores[v] = graph[u][v];
			}
		}
	}

	imprime(no_anterior, qte_ver, graph);
}