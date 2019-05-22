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
    int qte_arestas;
    struct Lista* sequencia;
    int* verificado;
    int* arestas;
    int* ind_ver;
};

struct LEG {
  int tamanhoL;
  int tamanhoE;
  int tamanhoG;
  int* L;
  int* E;
  int* G;
};

struct Vertice* adiciona_vertice(int seguinte, int distancia) {
    struct Vertice* no_criado = (struct Vertice*) malloc(sizeof(struct Vertice));
    no_criado->seguinte = seguinte;
    no_criado->peso = distancia;
    no_criado->proximo = NULL;
    return no_criado;
}

struct Grafo* cria_grafo(int qte_vertices, int qte_arestas) {
    struct Grafo* grafo_criado = (struct Grafo*) malloc(sizeof(struct Grafo));
    grafo_criado->qte_vertices = qte_vertices;
    grafo_criado->qte_arestas = qte_arestas;
    grafo_criado->sequencia = (struct Lista*) malloc(qte_vertices * sizeof(struct Lista));
    grafo_criado->verificado = (int *) malloc(qte_vertices * (qte_vertices - 1) * sizeof(int));
    grafo_criado->arestas = (int *) malloc(qte_arestas * sizeof(int));
    grafo_criado->ind_ver = (int *) malloc(2 * qte_arestas * sizeof(int));

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
    for (int v = 0; v < grafo->qte_vertices; v++) {
        struct Vertice* no_aux = grafo->sequencia[v].cabeca;
        printf("%d: ", v);
        while (no_aux){
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
    if ((arq = fopen(nome, "r")) == NULL) {
      printf("Ocorreu um erro ao abrir o arquivo");
      exit(1);
    }

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
  int pos = 0;
  for(int i=0; i < qte_vertices; i++) {
    for(int a = i+1; a < qte_vertices; a++) {
      adiciona_aresta(grafo, i, a, pontos);
      grafo->arestas[pos] = distanceEarthKm(pontos[i].x, pontos[i].y, pontos[a].x, pontos[a].y);
      grafo->ind_ver[(2 * pos)] = i;
      grafo->ind_ver[(2 * pos) + 1] = a;
      pos++;
    }
  }
}

int qte_vertices(char nome[64]) {
  FILE *cidades;
	cidades = fopen(nome, "r");
  if ((cidades = fopen(nome, "r")) == NULL) {
    printf("Ocorreu um erro ao abrir o arquivo");
    exit(1);
  }
  int qte_cidades;

	fscanf (cidades, "%d", &qte_cidades);

	fclose(cidades);

	return qte_cidades;
}

//Encontrar a mediana

struct LEG* partition(int arr[], int n, int pivot);

int compare(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

int retorna_mediana(int aux_vec[], int tamanho) {
  qsort(aux_vec, tamanho, sizeof(int), compare);
  int pos = floor(tamanho/2);
  return (aux_vec[pos]);
}

int retorna_posicao(int arr[], int n, int pivot) {
  for(int i = 0; i < n; i++) {
    if(arr[i] == pivot) {
      return i;
    }
  }
}

int mom(int arr[], int n, int kth) {
  if(n <= 50) {
    qsort(arr, n, sizeof(int), compare);
    return (arr[kth]);
  } else {
    int *aux_vec;
    int median = 0;
    int num_grupos = floor(n/5);
    int medianas[num_grupos];
    for (int i = 0; i < num_grupos; i++) {
      aux_vec = (int [5]){arr[(i*5)], arr[(i*5)+1], arr[(i*5)+2], arr[(i*5)+3], arr[(i*5)+4]};

      medianas[i] = retorna_mediana(aux_vec, 5);
    }

    int pos = mom(medianas, num_grupos, num_grupos/2);
    struct LEG* LEG = partition(arr, n, pos);
    if (kth <= LEG->tamanhoL) {
        return mom(LEG->L, LEG->tamanhoL, kth);
    } else if (kth > LEG->tamanhoL+ LEG->tamanhoE) {
        return mom(LEG->G, LEG->tamanhoG, (kth - LEG->tamanhoL - LEG->tamanhoE));
    } else {
      return pos;
    }
  }
}

void troca_elemento(int *a, int *b) {
  int aux = *a;
  *a = *b;
  *b = aux;
}

struct LEG* partition(int arr[], int n, int pivot) {
  int pos_mom = retorna_posicao(arr, n, pivot);
  troca_elemento(&arr[pos_mom], &arr[0]);

  int pos_mediana = floor(n/2);
  int pos_pivo = 0;
  int pos_final = n - 1;
  int aux_pivo = 0;
  int pivot_temp;
  int tam_aux = 0;

  for(int k=1; k < n; k++) {
    if (arr[aux_pivo] > arr[aux_pivo + 1]) {
      troca_elemento(&arr[pos_pivo], &arr[aux_pivo + 1]);
      pos_pivo++;
      aux_pivo++;
    } else if (arr[aux_pivo] < arr[aux_pivo + 1]) {
      troca_elemento(&arr[aux_pivo + 1], &arr[pos_final]);
      pos_final--;
    } else {
      aux_pivo++;
    }
  }

    struct LEG* novosLMR;
    novosLMR->tamanhoL = pos_pivo;
    novosLMR->L = (int*) malloc(novosLMR->tamanhoL * sizeof(int));
    novosLMR->tamanhoE = aux_pivo - pos_pivo + 1;
    novosLMR->E = (int*) malloc(novosLMR->tamanhoE * sizeof(int));
    novosLMR->tamanhoG = n - aux_pivo;
    novosLMR->G = (int*) malloc(novosLMR->tamanhoG * sizeof(int));

    int l = 0;
    int e = 0;
    int g = 0;
    for (int i=0; i<n; i++){
        if (arr[i] < arr[pos_pivo]){
            novosLMR->L[l] = arr[i];
            l++;
        } else if (arr[i] == arr[pos_pivo]) {
            novosLMR->E[e] = arr[i];
            e++;
        } else if (arr[i] > arr[aux_pivo]) {
            novosLMR->G[g] = arr[i];
            g++;
        }
    }

    return novosLMR;

}

//Fim de encontrar a mediana

void DFS(struct Grafo* grafo, int vertice) {
    struct Vertice* temp = grafo->sequencia[vertice].cabeca;

    grafo->verificado[vertice] = 1;

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
            DFS(grafo, v);
        }
    }
    return n;
}

int MBST(struct Grafo* grafo, struct Ponto* pontos) {

  if(grafo->qte_arestas == 1) {
    return grafo->sequencia[0].cabeca->peso;
  }
  int disp_a = 0;
  int disp_b = 0;
  //printf("\n\nnumero de arestas depois de chamada a MBST %d\n\n",grafo->qte_arestas);
  //printf("\n\nvalor atual da mediana das medianas %d\n\n", mediana);
  int qte_componentes = 0;
  int pos_pivot = 0;
  int pos_mediana = floor(grafo->qte_arestas/2);
  int usado[grafo->qte_arestas];
  for(int a=0; a < grafo->qte_arestas; a++) {
    usado[a] = grafo->arestas[a];
  }
  int mediana = mom(usado, grafo->qte_arestas, pos_mediana);

  //Cria os grupos A e B
  int tamanho_a = (grafo->qte_arestas/2);
  struct Grafo* grafo_A = cria_grafo(grafo->qte_vertices, tamanho_a);
  int tamanho_b = grafo->qte_arestas - (grafo->qte_arestas/2);
  struct Grafo* grafo_B = cria_grafo(grafo->qte_vertices, tamanho_b);

  for(int i=0; i < grafo->qte_arestas/2; i++) {
    //Adicionando a B
    if((grafo->arestas[i] < mediana) && (disp_b < tamanho_b)) {
      adiciona_aresta(grafo_B, grafo->ind_ver[(2*i)+1], grafo->ind_ver[(2*i)], pontos);
      disp_b++;
    } else if((grafo->arestas[i] == mediana) && (grafo->qte_arestas%2 != 0) && (disp_b < tamanho_b)){
      adiciona_aresta(grafo_B, grafo->ind_ver[(2*i)+1], grafo->ind_ver[(2*i)], pontos);
      disp_b++;
    } else {
      adiciona_aresta(grafo_A, grafo->ind_ver[(2*i)+1], grafo->ind_ver[(2*i)], pontos);
      disp_a++;
    }
  }

  qte_componentes = FOREST(grafo_B);
  printf("\n\nENCONTROU %d COMPONENTES\n\n", qte_componentes);

  if(qte_componentes > 1) {
    
  } else {
    
  }

  return 0;


  /*int qte_ver = grafo->qte_vertices;
  int qte_are = qte_ver * (qte_ver - 1);
  int tamanho_a = floor(qte_are/2);
  int tamanho_b = qte_are - tamanho_a;
  printf("%d %d ", tamanho_a, tamanho_b);
  int A[tamanho_a];
  int B[tamanho_b];

  int disp_a = 0;
  int disp_b = 0;

  for (int v = 0; v < qte_ver; v++) {
      struct Vertice* no_aux = grafo->sequencia[v].cabeca;
      while (no_aux) {
          printf("%d ", no_aux->peso);
          if(no_aux->peso >= mediana) {
            A[disp_a] = no_aux->peso;
            disp_a++;
          } else {
            B[disp_b] = no_aux->peso;
            disp_b++;
          }
          no_aux = no_aux->proximo;
      }
  }

  for(int i=0; i < tamanho_a; i++) {
    printf("A = %d. %d\n", i, A[i]);
  }

  for(int i=0; i < tamanho_a; i++) {
    printf("B = %d. %d\n", i, B[i]);
  }*/
}

int main(int argc, char ** argv) {
    int qte_ver = qte_vertices(argv[1]);
    int qte_are = (qte_ver * (qte_ver - 1))/2;
    int arr = (int) malloc(qte_ver * (qte_ver - 1) * sizeof(int));
    struct Grafo* graph = cria_grafo(qte_ver, (qte_ver * (qte_ver - 1)));
    struct Ponto* pontos = inicializa_pontos(qte_ver, argv[1]);
    inicializa_arestas(qte_ver, graph, pontos);

    /*int pos_pivot = 0;
    int pos_mediana = floor(qte_are/2);
    int usado[qte_are];
    for(int a=0; a < qte_are; a++) {
      usado[a] = graph->arestas[a];
    }
    int mediana = mom(usado, qte_are, pos_mediana);*/

    MBST(graph, pontos);

    return 0;
}
