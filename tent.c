#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

struct LEG {
  int tamanhoL;
  int tamanhoE;
  int tamanhoG;
  int* L;
  int* E;
  int* G;
};

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

int mom(int arr[], int qte_arestas, int kth) {
  printf("qte de arestas %d\nkth element %d\n\n", qte_arestas, kth);
  int n = qte_arestas;
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
    struct LEG* LEG = partition(arr, qte_arestas, pos);
    if (kth <= LEG->tamanhoL) {
        printf("primeiro if = %d\n", kth);
        return mom(LEG->L, LEG->tamanhoL, kth);
    } else if (kth > LEG->tamanhoL+ LEG->tamanhoE) {
        printf("segundo if\n");
          return mom(LEG->G, LEG->tamanhoG, (kth - LEG->tamanhoL - LEG->tamanhoE));
    } else {
      printf("retorna valor\n");
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

void MBST(int arr[], int pivot, int n, int pos_pivot) {
  //Inicializa os vetores
  int tamanho_b = pos_pivot + 1;
  int tamanho_a = n - pos_pivot - 1;
  int a[tamanho_a];
  int b[tamanho_b];

  printf("\n\nmaiores:\n");

  //Inicializa o A (maiores)
  for(int k = 0; k < tamanho_a; k++) {
    a[k] = arr[tamanho_b + k];
    printf("%d ", a[k]);
  }

  printf("\n\nmenores:\n");

  //Inicializa o B (vetores menores)
  for(int k=0; k < tamanho_b; k++) {
    b[k] = arr[k];
    printf("%d ", b[k]);
  }

}

void main() {
  int arr[54] = {29,22,28,14,45,10,44,23,9,39,38,52,6,5,50,37,11,26,3,15,2,53,40,54,25,55,12,19,30,16,18,13,1,48,41,24,43,46,47,17,34,20,31,32,22,35,4,49,51,7,36,27,8,21};
  int n = 54;
  int pos_pivot = 0;
  int pos_mediana = floor(n/2);
  int mediana = mom(arr, n, pos_mediana);
  printf("mediana encontrada %d", mediana);

}
