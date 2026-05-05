/**
 * @file    analizador.c
 * @brief   Analizador léxico de palabras reservadas del lenguaje C (C89/C90).
 *
 * Este programa lee el contenido de un archivo fuente en C, lo tokeniza
 * y, por cada token encontrado, verifica si corresponde a alguna de las
 * 32 palabras reservadas del estándar C89. Cuando halla una coincidencia,
 * imprime la palabra junto con su traducción al español.
 *
 * Flujo general:
 *   1. Cargar el archivo fuente en un búfer de memoria dinámica.
 *   2. Tokenizar el búfer usando los delimitadores habituales de C.
 *   3. Para cada token, buscarlo en el diccionario de palabras reservadas.
 *   4. Imprimir las coincidencias con su traducción.
 *   5. Liberar la memoria antes de terminar.
 */

/* =========================================================================
 * INCLUSIÓN DE BIBLIOTECAS ESTÁNDAR
 * ========================================================================= */

#include <ctype.h>  /* (reservado para posibles extensiones futuras)       */
#include <stdio.h>  /* fopen, fclose, fseek, ftell, fread, printf, perror */
#include <stdlib.h> /* malloc, free                                        */
#include <string.h> /* strtok, strcmp                                      */

/* =========================================================================
 * CONSTANTES DEL MÓDULO
 * ========================================================================= */

/** Nombre del archivo fuente que se analizará por defecto. */
#define NOMBRE_ARCHIVO_DEFECTO "Traducir.c"

/**
 * Delimitadores usados para separar tokens dentro del código fuente.
 * Incluyen espacios, tabuladores, saltos de línea y los operadores y
 * signos de puntuación más comunes de C.
 */
#define DELIMITADORES " \t\n{}();,+-*/=<>!&|[]"

/** Número total de palabras reservadas en el diccionario (C89). */
#define TOTAL_PALABRAS_RESERVADAS 32

/* =========================================================================
 * DEFINICIÓN DE TIPOS
 * ========================================================================= */

/**
 * @struct PalabraReservada
 * @brief  Par clave-valor que asocia una palabra reservada de C con su
 *         descripción en español.
 *
 * @var PalabraReservada::palabra_ingles
 *      Cadena con la palabra reservada tal como aparece en el código C.
 * @var PalabraReservada::traduccion_espanol
 *      Descripción funcional de la palabra reservada en español.
 */
typedef struct {
  const char *palabra_ingles;
  const char *traduccion_espanol;
} PalabraReservada;

/* =========================================================================
 * DICCIONARIO DE PALABRAS RESERVADAS (C89 — 32 entradas)
 * ========================================================================= */

/**
 * @brief Tabla de las 32 palabras reservadas del estándar C89/C90,
 *        cada una acompañada de una breve descripción en español.
 *
 * El orden alfabético facilita la lectura y el mantenimiento de la tabla.
 * Para búsqueda lineal (como la que realiza analizar_codigo) el orden
 * no afecta la corrección del resultado.
 */
const PalabraReservada diccionario[TOTAL_PALABRAS_RESERVADAS] = {
    {"auto", "Automático (especifica duracion de variables locales)"},
    {"break", "Romper (termina bucles y sentencias switch)"},
    {"case", "Caso (etiqueta para opciones en una sentencia switch)"},
    {"char", "Carácter (tipo de dato)"},
    {"const", "Constante (variable de solo lectura)"},
    {"continue", "Continuar (salta a la siguiente iteracion de un bucle)"},
    {"default", "Por defecto (caso predeterminado en una sentencia switch)"},
    {"do", "Hacer (primera parte de un bucle do-while)"},
    {"double", "Doble (tipo de dato de punto flotante de doble precision)"},
    {"else", "Sino (parte alternativa en una sentencia if-else)"},
    {"enum", "Enumeracion (define un tipo de dato con valores constantes)"},
    {"extern", "Externo (variable o funcion definida en otro archivo)"},
    {"float", "Flotante (tipo de dato de punto flotante de precision simple)"},
    {"for", "Para (bucle iterativo)"},
    {"goto", "Ir a (salta a una etiqueta)"},
    {"if", "Si (sentencia condicional)"},
    {"int", "Entero (tipo de dato)"},
    {"long", "Largo (calificador de tipo para enteros)"},
    {"register",
     "Registro (sugiere almacenar la variable en un registro de CPU)"},
    {"return", "Retornar (sale de una función y devuelve un valor)"},
    {"short", "Corto (calificador de tipo para enteros)"},
    {"signed", "Con signo (calificador de tipo para enteros)"},
    {"sizeof", "Tamaño de (operador que devuelve el tamaño en bytes)"},
    {"static", "Estático (duración o visibilidad de una variable)"},
    {"struct", "Estructura (define un tipo de dato compuesto)"},
    {"switch", "Conmutador (sentencia de control de flujo)"},
    {"typedef", "Definicion de tipo (crea un alias para un tipo)"},
    {"union", "Union (tipo de dato que puede almacenar diferentes tipos)"},
    {"unsigned", "Sin signo (calificador de tipo para enteros)"},
    {"void", "Vacio (funcion que no devuelve valor)"},
    {"volatile", "Volatil (variable que puede cambiar de forma inesperada)"},
    {"while", "Mientras (bucle condicional)"}};

/* =========================================================================
 * DECLARACIONES ADELANTADAS (PROTOTIPOS)
 * ========================================================================= */

char *cargar_codigo_desde_archivo(const char *nombre_archivo);
void analizar_codigo(char *codigo);

/* =========================================================================
 * IMPLEMENTACIÓN DE FUNCIONES
 * ========================================================================= */

/**
 * @brief  Carga el contenido completo de un archivo en un búfer dinámico.
 *
 * Abre el archivo en modo binario, determina su tamaño, reserva memoria
 * suficiente para almacenarlo y lo lee en su totalidad. Añade un
 * terminador nulo al final para que el búfer pueda tratarse como cadena.
 *
 * @param  nombre_archivo  Ruta (relativa o absoluta) al archivo a leer.
 * @return Puntero al búfer con el contenido del archivo, o NULL si:
 *           - El archivo no pudo abrirse.
 *           - No fue posible reservar la memoria necesaria.
 *
 * @note   El llamador es responsable de liberar la memoria devuelta
 *         con free() cuando ya no sea necesaria.
 */
char *cargar_codigo_desde_archivo(const char *nombre_archivo) {
  /* Abrir el archivo en modo binario para evitar conversiones de '\r\n' */
  FILE *archivo = fopen(nombre_archivo, "rb");
  if (!archivo) {
    perror("Error al abrir el archivo");
    return NULL;
  }

  /* Determinar el tamaño del archivo */
  fseek(archivo, 0, SEEK_END);
  long tamano_archivo = ftell(archivo);
  fseek(archivo, 0, SEEK_SET);

  /* Reservar memoria: tamaño del archivo + 1 byte para el '\0' final */
  char *buffer = (char *)malloc(tamano_archivo + 1);
  if (!buffer) {
    perror("Error al asignar memoria");
    fclose(archivo);
    return NULL;
  }

  /* Leer el contenido completo y cerrar el archivo */
  fread(buffer, 1, tamano_archivo, archivo);
  buffer[tamano_archivo] = '\0';
  fclose(archivo);

  return buffer;
}

/**
 * @brief  Tokeniza el código fuente e imprime las palabras reservadas
 *         de C que encuentra junto con su descripción en español.
 *
 * Recorre el búfer de código usando strtok con DELIMITADORES como
 * separadores. Para cada token resultante, realiza una búsqueda lineal
 * en el diccionario global. Si el token coincide con una palabra
 * reservada, imprime el par (palabra, traducción) en stdout.
 *
 * @param  codigo  Puntero al búfer de texto a analizar.
 *                 ¡Atención! strtok modifica la cadena en el lugar;
 *                 el contenido original queda alterado tras la llamada.
 *
 * @note   La función no distingue mayúsculas de minúsculas; el estándar
 *         C es sensible a ellas, por lo que "Int" no coincide con "int".
 */
void analizar_codigo(char *codigo) {
  /* Obtener el primer token */
  char *token = strtok(codigo, DELIMITADORES);

  while (token != NULL) {
    /* Buscar el token en el diccionario de palabras reservadas */
    for (size_t i = 0; i < TOTAL_PALABRAS_RESERVADAS; i++) {
      if (strcmp(token, diccionario[i].palabra_ingles) == 0) {
        printf("Palabra reservada encontrada: '%s'\n",
               diccionario[i].palabra_ingles);
        printf("  -> Traduccion: %s\n", diccionario[i].traduccion_espanol);
        break; /* Token ya identificado; pasar al siguiente */
      }
    }

    /* Avanzar al siguiente token */
    token = strtok(NULL, DELIMITADORES);
  }
}

/* =========================================================================
 * FUNCIÓN PRINCIPAL
 * ========================================================================= */

/**
 * @brief  Punto de entrada del programa.
 *
 * Carga el archivo fuente definido por NOMBRE_ARCHIVO_DEFECTO,
 * invoca el analizador y libera la memoria antes de terminar.
 *
 * @return 0 si el programa terminó sin errores.
 */
int main(void) {
  const char *nombre_archivo = NOMBRE_ARCHIVO_DEFECTO;

  /* Cargar el código fuente en memoria */
  char *codigo = cargar_codigo_desde_archivo(nombre_archivo);

  if (codigo) {
    printf("--- Analizando el codigo del archivo '%s' ---\n\n", nombre_archivo);

    analizar_codigo(codigo);

    /* Liberar el búfer una vez finalizado el análisis */
    free(codigo);
  }

  return 0;
}