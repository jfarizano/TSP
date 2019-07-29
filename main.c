#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAXCIUDADES 20

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

void imprimir_costos(MatrizAdyacencia matrizAdy, char** ciudades) {
  for (int i = 0; i < matrizAdy->cantCiudades; i++) {
    for (int j = i; j < matrizAdy->cantCiudades; j++) {
      if (matrizAdy->costos[i][j] != 0) {
        printf("%s %s %d\n", ciudades[i], ciudades[j], matrizAdy->costos[i][j]);
      }
    }
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
        assert(cantCiudades <= MAXCIUDADES);
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

int buscar_ciudad(char** ciudades, MatrizAdyacencia matrizAdy, char* ciudad) {
  for (int i = 0; i < matrizAdy->cantCiudades; i++) {
    if (strcmp(ciudades[i], ciudad) == 0) {
      return i;
    }
  }

  return -1;
}

void destruir_ciudades(char** ciudades, int cantCiudades) {
  for (int i = 0; i < cantCiudades; i++) {
    free(ciudades[i]);
  }
  free(ciudades);
}

void leer_costos(FILE* archivoEntrada, MatrizAdyacencia matrizAdy, 
                  char** ciudades) {
  char charBuff, strBuff[30];
  int costo; 
  int ciudad1, ciudad2;

  for (int i = 0, nCiudad = 1; (charBuff = getc(archivoEntrada)) != EOF;) {
    if (charBuff == '\n') {
      strBuff[i] = '\0';
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

int TSP(MatrizAdyacencia matrizAdy, int* costoFinal, int costoActual, 
          int ciudadActual, int cantVisitadas, int* recorrido) {

  int flag = 0;

  if (*costoFinal == -1 || costoActual < *costoFinal) {
    if (ciudadActual == 0 && cantVisitadas == matrizAdy->cantCiudades) {

      *costoFinal = costoActual;
      flag = 1;

    } else if (matrizAdy->costos[ciudadActual][ciudadActual] != -1) {

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

      matriz_ady_modificar_costo(matrizAdy, ciudadActual, ciudadActual, 0);

      if (flag) {
        recorrido[cantVisitadas] = ciudadActual;
      }

    }
  }

  return flag;
}

void salida_archivo(char* nombreArchivoSalida, MatrizAdyacencia matrizAdy, 
                    char** ciudades, int* recorrido) {

  FILE* archivoSalida = fopen(nombreArchivoSalida, "w");

  for (int i = 0; i < matrizAdy->cantCiudades; i++) {
    fprintf(archivoSalida, "%s,%s,", ciudades[recorrido[i]], 
            ciudades[recorrido[i + 1]]);
    fprintf(archivoSalida, "%d\n", 
            matrizAdy->costos[recorrido[i]][recorrido[i+1]]);
  }

  fclose(archivoSalida);
}

int main(int argc, char **argv) {
  if (argc == 3) {
    char** ciudades = malloc(sizeof(char*) * MAXCIUDADES);
    MatrizAdyacencia matrizAdy = cargar_datos(argv[1], ciudades);
    int recorrido[matrizAdy->cantCiudades + 1];
    recorrido[matrizAdy->cantCiudades] = 0;
    int costoFinal = -1;

    if (TSP(matrizAdy, &costoFinal, 0, 0, 0, recorrido)) {
      salida_archivo(argv[2], matrizAdy, ciudades, recorrido);
    } else {
      printf("No se encontró una solución.\n");
    }

    destruir_ciudades(ciudades, matrizAdy->cantCiudades);
    matriz_ady_destruir(matrizAdy);

    return 0;

  } else {
    printf("Cantidad de argumentos inválida.\n");
    return -1;
  } 
}