#ifndef __INPUTOUTPUTTSP_H__
#define __INPUTOUTPUTTSP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "matrizAdy.h"

///////////////////////////////////////////////////////////////////////////////
// Constantes.
#define MAXCIUDADES 20

///////////////////////////////////////////////////////////////////////////////
// Prototipos de funciones.

MatrizAdyacencia leer_ciudades(FILE* archivoEntrada, char** ciudades);

int buscar_ciudad(char** ciudades, MatrizAdyacencia matrizAdy, char* ciudad);

void leer_costos(FILE* archivoEntrada, MatrizAdyacencia matrizAdy, 
                  char** ciudades);

MatrizAdyacencia cargar_datos(char* nombreArchivoEntrada, char** ciudades);

void salida_archivo(char* nombreArchivoSalida, MatrizAdyacencia matrizAdy, 
                    char** ciudades, int* recorrido);

void destruir_ciudades(char** ciudades, int cantCiudades);                   

#endif /* __INPUTOUTPUTTSP_H__ */