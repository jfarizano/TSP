#include "inputOutput.h"

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

int buscar_ciudad(char** ciudades, int cantCiudades, char* ciudad) {
  for (int i = 0; i < cantCiudades; i++) {
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
  int ciudad1 = -1, ciudad2 = -1;

  for (int i = 0, nCiudad = 1; (charBuff = getc(archivoEntrada)) != EOF;) {
    if (charBuff == '\n') {
      strBuff[i] = '\0';
      costo = atoi(strBuff); 
      assert(costo > 0);
      matriz_ady_modificar_costo(matrizAdy, ciudad1, ciudad2, costo);
      i = 0;
    } else if (charBuff == '\r') {
      i--; // En caso de que el archivo de entrada haya sido creado en Windows
    } else if (charBuff == ',') {
      switch (nCiudad) {
        case 1:
          strBuff[i] = '\0';
          ciudad1 = buscar_ciudad(ciudades, matrizAdy->cantCiudades, strBuff);
          assert(ciudad1 != -1);
          nCiudad = 2;
          i = 0;
          break;
        case 2:
          strBuff[i] = '\0';
          ciudad2 = buscar_ciudad(ciudades, matrizAdy->cantCiudades, strBuff);
          assert(ciudad2 != -1);
          nCiudad = 1;
          i = 0;
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

void salida_archivo(char* nombreArchivoSalida, MatrizAdyacencia matrizAdy, 
                    char** ciudades, int* recorrido, int costoFinal) {

  FILE* archivoSalida = fopen(nombreArchivoSalida, "w");

  fprintf(archivoSalida, "Costo final del recorrido: %d\n", costoFinal);
  fprintf(archivoSalida, "Recorrido\n");
  for (int i = 0; i < matrizAdy->cantCiudades; i++) {
    fprintf(archivoSalida, "%s,%s,", ciudades[recorrido[i]], 
            ciudades[recorrido[i + 1]]);
    fprintf(archivoSalida, "%d\n", 
            matrizAdy->costos[recorrido[i]][recorrido[i+1]]);
  }

  fclose(archivoSalida);
}

void destruir_ciudades(char** ciudades, int cantCiudades) {
  for (int i = 0; i < cantCiudades; i++) {
    free(ciudades[i]);
  }
  free(ciudades);
}