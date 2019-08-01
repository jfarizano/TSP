#include <stdio.h>
#include "src/matrizAdy.h"
#include "src/inputOutput.h"

/*
  TSP: MatrizAdyacencia int* int int int int* -> int
  Resuelve recursivamente el problema TSP (el algoritmo se encuentra explicado
  en el archivo README.md).
  Devuelve 1 si a partir de el llamado recursivo actual se encontró un primer
  camino válido si no se encontró uno ya o si se encontró un nuevo camino válido
  con costo menor al anterior.
  Devuelve 0 en caso contrario.
*/
int TSP(MatrizAdyacencia matrizAdy, int* costoFinal, int costoActual, 
          int ciudadActual, int cantVisitadas, int* recorrido) {

  int flag = 0;

  if (*costoFinal == -1 || costoActual < *costoFinal) {
    if (ciudadActual == 0 && cantVisitadas == matrizAdy->cantCiudades) {

      *costoFinal = costoActual;
      flag = 1;

    } else if (matrizAdy->costos[ciudadActual][ciudadActual] != -1) {

      // Se marca la ciudad como visitada usando la diagonal de la matriz de 
      // adyacencia ya que no se utiliza con otro motivo.
      matriz_ady_modificar_costo(matrizAdy, ciudadActual, ciudadActual, -1);

      for (int i = 0, nuevoCosto; i < matrizAdy->cantCiudades; i++) {
        if (matrizAdy->costos[ciudadActual][i] != 0 &&
            (matrizAdy->costos[i][i] != -1 || i == 0)) {
          nuevoCosto = costoActual + matrizAdy->costos[ciudadActual][i];
          if (TSP(matrizAdy, costoFinal, nuevoCosto, i, cantVisitadas + 1, 
                  recorrido)) {
            flag = 1;
          }
        }
      }

      // Se desmarca la ciudad como visitada.
      matriz_ady_modificar_costo(matrizAdy, ciudadActual, ciudadActual, 0);

      if (flag) {
        recorrido[cantVisitadas] = ciudadActual;
      }

    }
  }

  return flag;
}

int main(int argc, char **argv) {
  if (argc == 3) {
    
    char** ciudades = malloc(sizeof(char*) * MAXCIUDADES);
    MatrizAdyacencia matrizAdy = cargar_datos(argv[1], ciudades);
    int recorrido[matrizAdy->cantCiudades + 1];
    recorrido[matrizAdy->cantCiudades] = 0;
    int costoFinal = -1;

    int solucionEncontrada = TSP(matrizAdy, &costoFinal, 0, 0, 0, recorrido);
    
    if (solucionEncontrada) {
      salida_archivo(argv[2], matrizAdy, ciudades, recorrido, costoFinal);
    } else {
      printf("No se encontró una solución.\n");
    }

    destruir_ciudades(ciudades, matrizAdy->cantCiudades);
    matriz_ady_destruir(matrizAdy);

    return solucionEncontrada;

  } else {
    printf("Cantidad de argumentos inválida.\n");
    return -1;
  } 
}