#ifndef __MATRIZADY_H__
#define __MATRIZADY_H__

#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// Definiciones de estructuras.

typedef struct {
  int** costos;
  int cantCiudades;
} _MatrizAdyacencia;

typedef _MatrizAdyacencia* MatrizAdyacencia;

///////////////////////////////////////////////////////////////////////////////
// Prototipos de funciones.

MatrizAdyacencia matriz_ady_crear(int cantCiudades);

void matriz_ady_destruir(MatrizAdyacencia matrizAdy);

void matriz_ady_modificar_costo(MatrizAdyacencia matrizAdy, int i, int j,
                                int costo);

void matriz_ady_imprimir(MatrizAdyacencia matrizAdy);

#endif /* __MATRIZADY_H__ */