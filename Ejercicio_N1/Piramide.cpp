/**
 * @file    Piramide.cpp
 * @brief   Expansión Binomial y Evaluación de Polinomio
 *
 * Este programa genera los coeficientes del binomio (1 + x)^n mediante
 * la fórmula de combinaciones C(n, k), construye la representación
 * textual del polinomio resultante, evalúa f(x) paso a paso para un
 * valor dado de x, y mide los tiempos de ejecución de cada etapa.
 *
 * Los resultados de tiempo se escriben adicionalmente en el archivo
 * 'resultado.txt' para comparación con la versión en Python.
 *
 * @author  Jesús Báez
 * @date    2026
 */

#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace std::chrono;

// ===========================================================================
//  Prototipos de funciones
// ===========================================================================

double *generarCoeficientes(int n);
void mostrarPolinomio(const double *coeficientes, int n);
double calcularFx(const double *coeficientes, int n, double x);
void guardarResultados(int n, double x, const double *coeficientes,
                       double resultadoFx, double tiempoGeneracion,
                       double tiempoTotal);

// ===========================================================================
//  Implementación de funciones
// ===========================================================================

/**
 * @brief Genera los coeficientes binomiales C(n, 0) a C(n, n).
 *
 * Utiliza la relación recursiva:
 *     C(n, k) = C(n, k-1) * (n - k + 1) / k
 *
 * @param  n  Exponente del binomio (entero no negativo).
 * @return    Arreglo dinámico de tamaño n+1 con los coeficientes.
 *
 * @note El llamador es responsable de liberar la memoria con delete[].
 */
double *generarCoeficientes(int n) {
  double *coeficientes = new double[n + 1];
  coeficientes[0] = 1; // C(n, 0) = 1

  for (int k = 1; k <= n; k++) {
    coeficientes[k] = coeficientes[k - 1] * (n - k + 1) / k;
  }

  return coeficientes;
}

/**
 * @brief Muestra la representación textual del polinomio en consola.
 *
 * El formato generado es:
 *     C(n,0)*x^0 + C(n,1)*x^1 + ... + C(n,n)*x^n
 *
 * @param coeficientes  Arreglo de coeficientes binomiales.
 * @param n             Grado del polinomio.
 */
void mostrarPolinomio(const double *coeficientes, int n) {
  cout << "\nPolinomio resultante:" << endl;

  for (int i = 0; i <= n; i++) {
    if (i == 0) {
      cout << coeficientes[i] << "*x^" << i;
    } else {
      cout << " + " << coeficientes[i] << "*x^" << i;
    }
  }

  cout << endl;
}

/**
 * @brief Evalúa el polinomio f(x) = Σ C(n, k) * x^k mostrando cada paso.
 *
 * @param coeficientes  Arreglo de coeficientes binomiales.
 * @param n             Grado del polinomio.
 * @param x             Valor en el que se evalúa el polinomio.
 * @return              Resultado numérico de f(x).
 */
double calcularFx(const double *coeficientes, int n, double x) {
  cout << "\nCalculo paso a paso de f(x):" << endl;

  double resultado = 0;

  for (int i = 0; i <= n; i++) {
    double termino = coeficientes[i] * pow(x, i);
    resultado += termino;

    cout << fixed << setprecision(0);
    cout << "Paso " << i << ": " << coeficientes[i] << "*(" << x << "^" << i
         << ") = " << termino << endl;
  }

  return resultado;
}

/**
 * @brief Escribe todos los resultados al archivo 'resultado.txt'.
 *
 * @param n                Exponente del binomio.
 * @param x                Valor en el que se evaluó el polinomio.
 * @param coeficientes     Arreglo de coeficientes binomiales.
 * @param resultadoFx      Resultado numérico de f(x).
 * @param tiempoGeneracion Tiempo de generación de coeficientes en segundos.
 * @param tiempoTotal      Tiempo total de ejecución en segundos.
 */
void guardarResultados(int n, double x, const double *coeficientes,
                       double resultadoFx, double tiempoGeneracion,
                       double tiempoTotal) {
  ofstream archivo("resultado.txt", ios::app);

  archivo << "==================================================" << endl;
  archivo << "--- Resultados C++ ---" << endl;
  archivo << "==================================================" << endl
          << endl;

  archivo << fixed << setprecision(0);
  archivo << "n = " << n << endl;
  archivo << "x = " << x << endl << endl;

  // Polinomio
  archivo << "Polinomio resultante:" << endl;
  for (int i = 0; i <= n; i++) {
    if (i == 0) {
      archivo << coeficientes[i] << "*x^" << i;
    } else {
      archivo << " + " << coeficientes[i] << "*x^" << i;
    }
  }
  archivo << endl << endl;

  // Calculo paso a paso
  archivo << "Calculo paso a paso de f(x):" << endl;
  for (int i = 0; i <= n; i++) {
    double termino = coeficientes[i] * pow(x, i);
    archivo << "Paso " << i << ": " << coeficientes[i] << "*(" << x << "^" << i
            << ") = " << termino << endl;
  }

  // Resultado final
  archivo << endl;
  archivo << scientific << setprecision(15);
  archivo << "Resultado final: f(" << fixed << setprecision(0) << x
          << ") = " << scientific << setprecision(15) << resultadoFx << endl
          << endl;

  // Tiempos
  archivo << fixed << setprecision(6);
  archivo << "Tiempo generacion coeficientes: " << tiempoGeneracion
          << " segundos" << endl;
  archivo << "Tiempo total calculo + evaluacion: " << tiempoTotal << " segundos"
          << endl;

  archivo.close();

  cout << "\nResultados guardados en 'resultado.txt'" << endl;
}

// ===========================================================================
//  Programa principal
// ===========================================================================

/**
 * @brief Punto de entrada del programa.
 */
int main() {
  int n;
  double x;

  // ---- Lectura de datos de entrada ------------------------------------
  cout << "Ingrese un numero entero no negativo n: ";
  cin >> n;

  // ---- Generación de coeficientes (con medición de tiempo) ------------
  auto inicioGeneracion = high_resolution_clock::now();
  double *coeficientes = generarCoeficientes(n);
  auto finGeneracion = high_resolution_clock::now();

  double tiempoGeneracion =
      duration_cast<duration<double>>(finGeneracion - inicioGeneracion).count();

  // ---- Lectura del valor de x ----------------------------------------
  cout << "Ingrese el valor de x: ";
  cin >> x;

  // ---- Mostrar el polinomio resultante --------------------------------
  mostrarPolinomio(coeficientes, n);

  // ---- Evaluación de f(x) (con medición de tiempo) --------------------
  auto inicioEval = high_resolution_clock::now();
  double resultado = calcularFx(coeficientes, n, x);
  auto finEval = high_resolution_clock::now();

  double tiempoTotal =
      duration_cast<duration<double>>(finEval - inicioEval).count();

  // ---- Mostrar resultados en consola ----------------------------------
  cout << fixed << setprecision(15);
  cout << "\nf(" << x << ") = " << resultado << endl;
  cout << "\nTiempo generaci  on coeficientes: " << tiempoGeneracion
       << " segundos";
  cout << "\nTiempo total calculo + evaluacion: " << tiempoTotal << " segundos"
       << endl;

  // ---- Guardar resumen en archivo -------------------------------------
  guardarResultados(n, x, coeficientes, resultado, tiempoGeneracion,
                    tiempoTotal);

  // ---- Liberar memoria dinámica ---------------------------------------
  delete[] coeficientes;

  return 0;
}
