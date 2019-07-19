#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct {
  int** costos;
  size_t cantCiudades;
} _MatrizAdyacencia;

typedef _MatrizAdyacencia* MatrizAdyacencia;

MatrizAdyacencia matriz_ady_crear(size_t cantCiudades) {
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

void matriz_ady_modificar_costo(MatrizAdyacencia matrizAdy, size_t i, size_t j,
                                int costo) {
  matrizAdy->costos[i][j] = costo;
  matrizAdy->costos[j][i] = costo;
}

void matriz_ady_imprimir(MatrizAdyacencia matrizAdy) {
  for (size_t i = 0; i < matrizAdy->cantCiudades; i++) {
    for (size_t j = 0; j < matrizAdy->cantCiudades; j++) {
      printf("%d ", matrizAdy->costos[i][j]);
    }
    printf("\n");
  }
}

MatrizAdyacencia leer_ciudades(FILE* archivoEntrada, char** ciudades) {
  size_t cantCiudades = 0;
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

size_t buscar_ciudad(char** ciudades, MatrizAdyacencia matrizAdy, char* ciudad) {
  for (size_t i = 0; i < matrizAdy->cantCiudades; i++) {
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
  size_t ciudad1, ciudad2;

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
          charBuff = getc(archivoEntrada);
          break;
        case 2:
          strBuff[i] = '\0';
          ciudad2 = buscar_ciudad(ciudades, matrizAdy, strBuff);
          assert(ciudad2 != -1);
          nCiudad = 1;
          i = 0;
          charBuff = getc(archivoEntrada);
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

int main(int argc, char **argv ) {
  if (argc == 3) {
    char** ciudades = malloc(sizeof(char*) * 15);
    MatrizAdyacencia matrizAdy = cargar_datos(argv[1], ciudades); 
    
    return 0;
  } else {
    printf("Cantidad de argumentos inválida.\n");
    return -1;
  } 
}