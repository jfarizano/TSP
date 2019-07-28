#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct {
  int** costos;
  int cantCiudades;
} _MatrizAdyacencia;

typedef _MatrizAdyacencia* MatrizAdyacencia;

MatrizAdyacencia matriz_ady_crear(int cantCiudades) {
  MatrizAdyacencia matrizAdy = malloc(sizeof(_MatrizAdyacencia));
  matrizAdy->cantCiudades = cantCiudades;
  matrizAdy->costos = malloc(sizeof(int*) * cantCiudades);
  
  // Inicializar la matriz de adyacencia con todas sus entradas iguales a 0
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

MatrizAdyacencia leer_ciudades(FILE* archivoEntrada, char** ciudades) {
  int cantCiudades = 0;
  char charBuff, strBuff[80];

  for (int i = 0, seguir = 1; seguir;) {
    charBuff = getc(archivoEntrada);
    if (charBuff == '\n') {
      if (i > 0) {
        strBuff[i] = '\0';
        ciudades[cantCiudades] = malloc(sizeof(char) * (strlen(strBuff) + 1));
        strcpy(ciudades[cantCiudades], strBuff);
        cantCiudades++;
      }
      seguir = 0;
    } else if (charBuff == ',') {
      strBuff[i] = '\0';
      ciudades[cantCiudades] = malloc(sizeof(char) * (strlen(strBuff) + 1));
      strcpy(ciudades[cantCiudades], strBuff);
      cantCiudades++;
      i = 0;
      charBuff = getc(archivoEntrada);
    } else {
      strBuff[i] = charBuff;
      i++;
    }
  }

  return matriz_ady_crear(cantCiudades);
}

void destruir_ciudades(char** ciudades, int cantCiudades) {
  for (int i = 0; i < cantCiudades; i++) {
    free(ciudades[i]);
  }
  free(ciudades);
}

int buscar_ciudad(char** ciudades, MatrizAdyacencia matrizAdy, char* ciudad) {
  for (int i = 0; i < matrizAdy->cantCiudades; i++) {
    if (strcmp(ciudades[i], ciudad) == 0) {
      return i;
    }
  }

  return -1;
}

void leer_costos(FILE* archivoEntrada, MatrizAdyacencia matrizAdy, 
                  char** ciudades) {
  char charBuff, strBuff[30];
  int costo; 
  int ciudad1, ciudad2;

  for (int i = 0, nCiudad = 1; (charBuff = getc(archivoEntrada)) != EOF;) {
    if (charBuff == '\n') {
      costo = atoi(strBuff); 
      assert(costo > 0);
      matriz_ady_modificar_costo(matrizAdy, ciudad1, ciudad2, costo);
      i = 0;
    } else if (charBuff == ',') {
      switch (nCiudad) {
        case 1:
          strBuff[i] = '\0';
          ciudad1 = buscar_ciudad(ciudades, matrizAdy, strBuff);
          assert(ciudad1 != -1);
          nCiudad = 2;
          i = 0;
          // charBuff = getc(archivoEntrada);
          break;
        case 2:
          strBuff[i] = '\0';
          ciudad2 = buscar_ciudad(ciudades, matrizAdy, strBuff);
          assert(ciudad2 != -1);
          nCiudad = 1;
          i = 0;
          // charBuff = getc(archivoEntrada);
          break;
      }
    } else {
      strBuff[i] = charBuff;
      i++;
    }
  }
}

MatrizAdyacencia cargar_datos(char* nombreArchivoEntrada, char** ciudades) {
  FILE* archivoEntrada = fopen(nombreArchivoEntrada, "r");
  assert(archivoEntrada != NULL);
  
  char charBuff;

  // Adelantar una línea
  for (; (charBuff = getc(archivoEntrada)) != '\n';);

  // Leer nombres de las ciudades y crear su respectiva matriz de adyacencia
  MatrizAdyacencia matrizAdy = leer_ciudades(archivoEntrada, ciudades);

  // Adelantar una línea
  for (; (charBuff = getc(archivoEntrada)) != '\n';);

  // Leer costos de viajes entre ciudades
  leer_costos(archivoEntrada, matrizAdy, ciudades);

  fclose(archivoEntrada);
  return matrizAdy;
}

int no_visitada(int* recorrido, int ciudadActual, int cantVisitadas) {
  for (int i = 0; i < cantVisitadas; i++) {
    if (recorrido[i] == ciudadActual) {
      return 0;
    }
  }

  return 1;
}

void TSP(MatrizAdyacencia matrizAdy, int* costoFinal, int costoActual, 
          int ciudadActual, int cantVisitadas) {
  if (*costoFinal == -1 || costoActual < *costoFinal) {
    if (ciudadActual == 0 && cantVisitadas == matrizAdy->cantCiudades) {
      *costoFinal = costoActual;
      // printf("%d %d %d\n", costoActual, ciudadActual, cantVisitadas);
    } else if (matrizAdy->costos[ciudadActual][ciudadActual] == 0) {
      matrizAdy->costos[ciudadActual][ciudadActual] = -1;

      for (int i = 0, nuevoCosto; i < matrizAdy->cantCiudades; i++) {
        if (matrizAdy->costos[ciudadActual][i] != 0 &&
            (matrizAdy->costos[i][i] != -1 || i == 0)) {
          nuevoCosto = costoActual + matrizAdy->costos[ciudadActual][i];
          TSP(matrizAdy, costoFinal, nuevoCosto, i, cantVisitadas + 1);
        }
      }

      matrizAdy->costos[ciudadActual][ciudadActual] = 0;

    }
  }
}

int main(int argc, char **argv ) {
  if (argc == 3) {
    char** ciudades = malloc(sizeof(char*) * 25);
    MatrizAdyacencia matrizAdy = cargar_datos(argv[1], ciudades);

    int costoFinal = -1;
    // matriz_ady_imprimir(matrizAdy);

    TSP(matrizAdy, &costoFinal, 0, 0, 0);
    printf("%d", costoFinal);
    printf("\n");

    destruir_ciudades(ciudades, matrizAdy->cantCiudades);
    matriz_ady_destruir(matrizAdy);

    return 0;

  } else {
    printf("Cantidad de argumentos inválida.\n");
    return -1;
  } 
}