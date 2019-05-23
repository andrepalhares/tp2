double deg2rad(double deg);
int distanceEarthKm(double lat1d, double lon1d, double lat2d, double lon2d);
int qte_vertices(char nome[64]);
void inicia_pontos(char nome[64], int qte_vertices, struct Ponto cidades[qte_vertices]);
void inicia_arestas(char nome[64], int qte_vertices, struct Ponto cidades[qte_vertices], int arestas[][qte_vertices], int *max_aresta);
int minima_aresta(int menores_valores[], int vertices_fora[], int qte_ver, int maior_aresta);
int imprime(int no_anterior[], int qte_ver, int grafo[qte_ver][qte_ver]);
void prim(int qte_ver, int graph[qte_ver][qte_ver], int max_aresta);