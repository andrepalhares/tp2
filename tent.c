#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

int compare(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

int retorna_mediana(int aux_vec[]) {
  qsort(aux_vec, 5, sizeof(int), compare);
  return (aux_vec[2]);
}

int mom(int arr[], int n) {
  if(n <= 5) {
    qsort(arr, n, sizeof(int), compare);
    int pos = floor(n/2);
    int mediana = arr[pos];
    return mediana;
  } else {
    int *aux_vec;
    int median = 0;
    int num_grupos = floor(n/5);
    int medianas[num_grupos];
    for (int i =0; i < num_grupos; i++) {
      aux_vec = (int [5]){arr[(i*5)], arr[(i*5)+1], arr[(i*5)+2], arr[(i*5)+3], arr[(i*5)+4]};

      median = retorna_mediana(aux_vec);
      medianas[i] = median;

    }
    mom(medianas, num_grupos);
  }
}

void troca_elemento(int *a, int *b) {
  int aux = *a;
  *a = *b;
  *b = aux;
}

void partition(int arr[], int n, int pivot, int *pos_pivot) {
  //Troca o pivô de lugar com a primeira posição do vetor
  for(int i = 0; i < n; i++) {
    if(arr[i] == pivot) {
      troca_elemento(&arr[i], &arr[0]);
    }
  }

  int pos_pivo = 0;
  int pos_final = n - 1;
  int aux_pivo = 0;

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
    //printf("pivo %d pos pivo %d pos final %d\n", arr[pos_pivo], pos_pivo, pos_final);
  }

  *pos_pivot = pos_pivo;
}

void MBST(int arr[], int pivot, int n, int pos_pivot) {
  //Inicializa os vetores
  int tamanho_b = pos_pivot + 1;
  int tamanho_a = n - pos_pivot - 1;
  int a[tamanho_a];
  int b[tamanho_b];

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
  int pivot = mom(arr, n);
  partition(arr, n, pivot, &pos_pivot);
  for(int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n\nmaiores:\n");
  MBST(arr, pivot, n, pos_pivot);

}
