#ifndef __MATRIZADY_H__
#define __MATRIZADY_H__

#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// Definiciones de estructuras.

typedef struct {
  int cantCiudades;
  int** costos;
} _MatrizAdyacencia;

typedef _MatrizAdyacencia* MatrizAdyacencia;

///////////////////////////////////////////////////////////////////////////////
// Prototipos de funciones.

/*
  matriz_ady_crear: int -> MatridAdyacencia
  Dado N, crea y devuelve una estructura que contendrá una matriz de tamaño N*N
  y el valor N.
*/
MatrizAdyacencia matriz_ady_crear(int cantCiudades);

/*
  matriz_ady_destruir: MatrizAdyacencia -> None
  Dada una matriz de adyacencia, la libera de la memoria.
*/
void matriz_ady_destruir(MatrizAdyacencia matrizAdy);

/*
  matriz_ady_modificar_costo: MatrizAdyacencia int int int
  Dada una matriz de adyacencia y 3 enteros i, j y costo. Modifica las entradas
  en las posiciones (i,j) y (j,i) y se igualan a costo.
*/
void matriz_ady_modificar_costo(MatrizAdyacencia matrizAdy, int i, int j,
                                int costo);

/*
  matriz_ady_imprimir: MatrizAdyacencia -> None
  Dada una matriz de adyacencia, imprime sus entradas en consola.
  (Función utilizada solo para testeo).
*/
void matriz_ady_imprimir(MatrizAdyacencia matrizAdy);

#endif /* __MATRIZADY_H__ */