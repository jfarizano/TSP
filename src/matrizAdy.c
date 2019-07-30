#include "matrizAdy.h"

MatrizAdyacencia matriz_ady_crear(int cantCiudades) {
  MatrizAdyacencia matrizAdy = malloc(sizeof(_MatrizAdyacencia));
  matrizAdy->cantCiudades = cantCiudades;
  matrizAdy->costos = malloc(sizeof(int*) * cantCiudades);
  
  for (int i = 0; i < cantCiudades; i++) {
    matrizAdy->costos[i] = malloc(sizeof(int) * cantCiudades);
    for (int j = 0; j < cantCiudades; j++) {
      matrizAdy->costos[i][j] = 0;
    }
  }

  return matrizAdy;
}

void matriz_ady_destruir(MatrizAdyacencia matrizAdy) {
  for (int i = 0; i < matrizAdy->cantCiudades; i++) {
    free(matrizAdy->costos[i]);
  }
  free(matrizAdy->costos);
  free(matrizAdy);
}

void matriz_ady_modificar_costo(MatrizAdyacencia matrizAdy, int i, int j,
                                int costo) {
  matrizAdy->costos[i][j] = costo;
  matrizAdy->costos[j][i] = costo;
}

void matriz_ady_imprimir(MatrizAdyacencia matrizAdy) {
  for (int i = 0; i < matrizAdy->cantCiudades; i++) {
    for (int j = 0; j < matrizAdy->cantCiudades; j++) {
      printf("%d ", matrizAdy->costos[i][j]);
    }
    printf("\n");
  }
}