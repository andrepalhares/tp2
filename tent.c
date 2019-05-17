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
  int tamanho_b = pos_pivot;
  int tamanho_a = n - pos_pivot - 1;
  int a[tamanho_a];
  int b[tamanho_b];

  //Ao criar o vetor, setar a posição que a mediana devera estar
  //e ir colocando os elementos (if =/= de 0 atribui o valor)
  //usar a (aux_pivot - pos_pivo)/2 (adicionar esse valor à posicao do pivo)
  //para dividir as medianas

  //ex se as posicoes forem 0 e 6, ao dividir vai dar 3
  //a posicao nova do pivo sera 0 + 3 e metade vai pra um lado
  // e o restante vai para o outro

  //Inicializa o A (maiores)
  for(int k = n - 1; k >= tamanho_b; k--) {
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
  int arr[9] = {7,7,7,7,7,7,7,10, 10};
  int n = 9;
  int pos_pivot = 0;
  int pivot = mom(arr, n);
  partition(arr, n, pivot, &pos_pivot);
  for(int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n\nmaiores:\n");
  MBST(arr, pivot, n, pos_pivot);

}
