#include "tp2.h"

void main(int argc, char ** argv) {
  int max_aresta;
  int qte_ver = qte_vertices(argv[1]);
  struct Ponto cidades[qte_ver];
  int arestas[qte_ver][qte_ver];

  inicia_pontos(argv[1], qte_ver, cidades);
  inicia_arestas(argv[1], qte_ver, cidades, arestas, &max_aresta);

  prim(qte_ver, arestas, max_aresta);
}