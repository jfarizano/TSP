#ifndef __INPUTOUTPUTTSP_H__
#define __INPUTOUTPUTTSP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "matrizAdy.h"

///////////////////////////////////////////////////w////////////////////////////
// Constantes.
#define MAXCIUDADES 20

///////////////////////////////////////////////////////////////////////////////
// Prototipos de funciones.

/*
  MatrizAdyacencia leer_ciudades: FILE* char** -> MatrizAdyacencia
  Recibe el puntero al archivo de entrada posicionado en la segunda línea y un
  array bidimensional de caracteres, lee la línea donde se encuentran
  los nombres de las ciudades y se almacenan en el array, devuelve una matriz
  de adyacencia de tamaño N*N donde N es la cantidad de ciudades.
*/
MatrizAdyacencia leer_ciudades(FILE* archivoEntrada, char** ciudades);

/*
  int buscar_ciudad: char** MatrizAdyacencia char -> int
  Dado un array con los nombres de las ciudades, la cantidad de ciudades
  y el nombre de una ciudad.
  Se busca y se devuelve la posición de la ciudad en el array, 
  en el caso de que no se encuentre se devuelve -1.
*/
int buscar_ciudad(char** ciudades, int cantCiudades, char* ciudad);

/*
  void leer_costos: FILE* MatrizAdyacencia char** -> None
  Dado el puntero al archivo de entrada posicionado en la cuarta línea,
  una matriz de adyacencia y un array con los nombres de las ciudades.
  Lee hasta el final del archivo los costos de viajes entre ciudades 
  y los almacena en la matriz de adyacencia.
*/
void leer_costos(FILE* archivoEntrada, MatrizAdyacencia matrizAdy, 
                  char** ciudades);

/*
  cargar_datos: char* char** -> MatrizAdyacencia
  Dado el nombre del archivo de entrada y el array donde se almacenarán
  los nombres de las ciudades.
  Realiza la carga de los datos del archivo de entrada mediante las funciones
  leer_ciudades y leer_costos, devuelve la matriz de adyacencia resultante.
*/
MatrizAdyacencia cargar_datos(char* nombreArchivoEntrada, char** ciudades);

/*
  salida_archivo: char** MatrizAdyacencia char** int* -> None
  Dado el nombre del archivo de salida, una matriz de adyacencia, un array
  con los nombres de las ciudades, un array de enteros que representa
  el recorrido de la solución del problema, y el costo total del recorrido.
  Realiza la escritura correspondiente al archivo de salida.
*/
void salida_archivo(char* nombreArchivoSalida, MatrizAdyacencia matrizAdy, 
                    char** ciudades, int* recorrido, int costoFinal);

/*
  destruir_ciudades: char** int -> NOne
  Dado un array con los nombres de las ciudades y la cantidad que hay guardadas,
  libera de la memoria los nombres y el array en sí.
*/
void destruir_ciudades(char** ciudades, int cantCiudades);                   

#endif /* __INPUTOUTPUTTSP_H__ */