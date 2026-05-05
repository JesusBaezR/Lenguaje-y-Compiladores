"""
Piramide.py - Expansión Binomial y Evaluación de Polinomio

Este módulo genera los coeficientes del binomio (1 + x)^n mediante la
fórmula de combinaciones C(n, k), construye la representación textual
del polinomio resultante, evalúa f(x) paso a paso para un valor dado
de x, y mide los tiempos de ejecución de cada etapa.

Los resultados de tiempo se escriben adicionalmente en el archivo
'resultado.txt' para comparación con la versión en C++.

Autor:  Jesús Báez
Fecha:  2026
"""

import time


# ===========================================================================
#  Funciones auxiliares
# ===========================================================================

def generar_coeficientes(n):
    """
    Genera la lista de coeficientes binomiales C(n, 0), C(n, 1), ..., C(n, n).

    Utiliza la relacion recursiva:
        C(n, k) = C(n, k-1) * (n - k + 1) / k

    Parámetros
    ----------
    n : int
        Exponente del binomio (entero no negativo).

    Retorna
    -------
    list[int]
        Lista con n+1 coeficientes binomiales.
    """
    coeficientes = [1]  # C(n, 0) = 1

    for k in range(1, n + 1):
        siguiente = coeficientes[k - 1] * (n - k + 1) // k
        coeficientes.append(siguiente)

    return coeficientes


def mostrar_polinomio(coeficientes):
    """
    Construye la representacion textual del polinomio.

    El formato generado es:
        C(n,0)*x^0 + C(n,1)*x^1 + ... + C(n,n)*x^n

    Parámetros
    ----------
    coeficientes : list[int]
        Lista de coeficientes binomiales.

    Retorna
    -------
    str
        Cadena con la representacion del polinomio.
    """
    terminos = []

    for exponente, coef in enumerate(coeficientes):
        if exponente == 0:
            terminos.append(f"{coef}*x^{exponente}")
        else:
            terminos.append(f" + {coef}*x^{exponente}")

    return "".join(terminos)


def calcular_fx(coeficientes, x):
    """
    Evalúa el polinomio f(x) = Σ C(n, k) * x^k  mostrando cada paso.

    Parámetros
    ----------
    coeficientes : list[int]
        Lista de coeficientes binomiales.
    x : int
        Valor en el que se evalúa el polinomio.

    Retorna
    -------
    int | float
        Resultado numérico de f(x).
    """
    print("\nCalculo paso a paso de f(x):")

    resultado = 0

    for exponente, coef in enumerate(coeficientes):
        termino = coef * (x ** exponente)
        resultado += termino
        print(f"Paso {exponente}: {coef}*({x}^{exponente}) = {termino}")

    return resultado


def guardar_resultados(n, x, coeficientes, resultado_fx,
                       tiempo_generacion, tiempo_total):
    """
    Escribe todos los resultados al archivo 'resultado.txt'.

    Parámetros
    ----------
    n : int
        Exponente del binomio.
    x : int
        Valor en el que se evaluó el polinomio.
    coeficientes : list[int]
        Lista de coeficientes binomiales.
    resultado_fx : int | float
        Resultado numérico de f(x).
    tiempo_generacion : float
        Tiempo de generación de coeficientes en segundos.
    tiempo_total : float
        Tiempo total de ejecucion en segundos.
    """
    with open("resultado.txt", "a", encoding="utf-8") as archivo:
        archivo.write("=" * 50 + "\n")
        archivo.write("--- Resultados Python ---\n")
        archivo.write("=" * 50 + "\n\n")

        archivo.write(f"n = {n}\n")
        archivo.write(f"x = {x}\n\n")

        # Polinomio
        archivo.write("Polinomio resultante:\n")
        archivo.write(mostrar_polinomio(coeficientes) + "\n\n")

        # Cálculo paso a paso
        archivo.write("Calculo paso a paso de f(x):\n")
        for exponente, coef in enumerate(coeficientes):
            termino = coef * (x ** exponente)
            archivo.write(
                f"Paso {exponente}: {coef}*({x}^{exponente}) = {termino}\n"
            )

        # Resultado final
        archivo.write(f"\nResultado final: f({x}) = {resultado_fx:.15e}\n\n")

        # Tiempos
        archivo.write(
            f"Tiempo generacion coeficientes: {tiempo_generacion:.6f} segundos\n"
        )
        archivo.write(
            f"Tiempo total calculo + evaluacion: {tiempo_total:.6f} segundos\n"
        )

    print("\nResultados guardados en 'resultado.txt'")


# ===========================================================================
#  Programa principal
# ===========================================================================

def main():
    """Punto de entrada del programa."""

    # ---- Lectura de datos de entrada ------------------------------------
    n = int(input("Ingrese un numero entero no negativo n: "))
    x = int(input("Ingrese el valor de x: "))

    # ---- Generacion de coeficientes (con medicion de tiempo) ------------
    inicio_generacion = time.time()
    coeficientes = generar_coeficientes(n)
    fin_generacion = time.time()

    tiempo_generacion = fin_generacion - inicio_generacion

    # ---- Mostrar el polinomio resultante --------------------------------
    print("\nPolinomio resultante:")
    print(mostrar_polinomio(coeficientes))

    # ---- Evaluacion de f(x) (con medicion de tiempo total) --------------
    resultado_fx = calcular_fx(coeficientes, x)
    fin_total = time.time()

    tiempo_total = fin_total - inicio_generacion  # Desde el inicio de generacion

    # ---- Mostrar resultados en consola ----------------------------------
    print(f"\nResultado final: f({x}) = {resultado_fx:.15e}")
    print(f"\nTiempo generacion coeficientes: {tiempo_generacion:.6f} segundos")
    print(f"Tiempo total calculo + evaluacion: {tiempo_total:.6f} segundos")

    # ---- Guardar resumen en archivo -------------------------------------
    guardar_resultados(n, x, coeficientes, resultado_fx,
                       tiempo_generacion, tiempo_total)


if __name__ == "__main__":
    main()
