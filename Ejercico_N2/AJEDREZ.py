"""
AJEDREZ.py
============
Validador de cadenas en notación FEN (Forsyth-Edwards Notation).

La notación FEN es el estándar utilizado para representar la posición de
un tablero de ajedrez en un momento dado. Una cadena FEN válida contiene
exactamente 6 campos separados por espacios:

    1. Disposición de piezas  – 8 filas separadas por '/'
    2. Turno                  – 'w' (blancas) o 'b' (negras)
    3. Disponibilidad enroque – combinación de 'K', 'Q', 'k', 'q' o '-'
    4. Casilla de captura al paso – casilla destino (p.ej. 'e3') o '-'
    5. Contador de semimovimientos – número entero >= 0 (regla de 50 mov.)
    6. Número de movimiento completo – número entero >= 1

Uso:
    python AJEDREZ.py
    > <cadena FEN>
"""

import re

# ---------------------------------------------------------------------------
# CONSTANTES
# ---------------------------------------------------------------------------

# Piezas válidas tal como aparecen en la notación FEN
PIEZAS_VALIDAS = "prnbqkPRNBQK"

# Expresión regular para un campo de enroque válido (una o más de estas letras)
PATRON_ENROQUE = r"[KQkq]+"

# Expresión regular para una casilla de captura al paso válida
# (columna a-h + fila 3 o 6, única posibilidad según las reglas)
PATRON_AL_PASO = r"[a-h][36]"


# ---------------------------------------------------------------------------
# FUNCIONES AUXILIARES
# ---------------------------------------------------------------------------

def _validar_tablero(tablero: str) -> tuple[bool, str]:
    """Verifica que la sección de disposición de piezas sea correcta.

    Comprueba que existan exactamente 8 filas separadas por '/' y que
    cada fila sume exactamente 8 casillas, usando dígitos para espacios
    vacíos consecutivos y letras de PIEZAS_VALIDAS para las piezas.

    Args:
        tablero: El primer campo de la cadena FEN (p.ej.
                 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR').

    Returns:
        Tupla ``(válido, mensaje)`` donde ``válido`` es un booleano y
        ``mensaje`` describe el resultado o el primer error encontrado.
    """
    filas = tablero.split("/")
    if len(filas) != 8:
        return False, "El tablero debe tener 8 filas separadas por '/'."

    for fila in filas:
        conteo = 0
        for c in fila:
            if c.isdigit():
                conteo += int(c)
            elif c.isalpha():
                if c not in PIEZAS_VALIDAS:
                    return False, f"Carácter inválido '{c}' en el tablero."
                conteo += 1
            else:
                return False, f"Carácter no válido '{c}' en fila."

        if conteo != 8:
            return False, f"La fila '{fila}' no suma 8 casillas."

    return True, ""


def _validar_turno(turno: str) -> tuple[bool, str]:
    """Verifica que el campo de turno sea 'w' o 'b'.

    Args:
        turno: El segundo campo de la cadena FEN.

    Returns:
        Tupla ``(válido, mensaje)``.
    """
    if turno not in ("w", "b"):
        return False, "El turno debe ser 'w' o 'b'."
    return True, ""


def _validar_enroque(enroque: str) -> tuple[bool, str]:
    """Verifica que el campo de disponibilidad de enroque sea válido.

    Acepta '-' (ningún enroque disponible) o cualquier combinación no
    repetida de las letras K, Q, k, q.

    Args:
        enroque: El tercer campo de la cadena FEN.

    Returns:
        Tupla ``(válido, mensaje)``.
    """
    if enroque != "-" and not re.fullmatch(PATRON_ENROQUE, enroque):
        return False, "El campo de enroque no es válido."
    return True, ""


def _validar_al_paso(al_paso: str) -> tuple[bool, str]:
    """Verifica que la casilla de captura al paso sea válida.

    Acepta '-' (sin captura al paso posible) o una casilla de la forma
    [a-h][3|6] que indica el destino del peón capturado al paso.

    Args:
        al_paso: El cuarto campo de la cadena FEN.

    Returns:
        Tupla ``(válido, mensaje)``.
    """
    if al_paso != "-" and not re.fullmatch(PATRON_AL_PASO, al_paso):
        return False, "La posición de captura al paso no es válida."
    return True, ""


def _validar_contadores(medio: str, mov: str) -> tuple[bool, str]:
    """Verifica que los contadores de movimientos sean números enteros.

    El quinto campo es el contador de semimovimientos (regla de los
    50 movimientos) y el sexto es el número de movimiento completo.

    Args:
        medio: El quinto campo de la cadena FEN.
        mov:   El sexto campo de la cadena FEN.

    Returns:
        Tupla ``(válido, mensaje)``.
    """
    if not medio.isdigit() or not mov.isdigit():
        return False, "Los últimos dos campos deben ser números."
    return True, ""


# ---------------------------------------------------------------------------
# FUNCIÓN PRINCIPAL DE VALIDACIÓN
# ---------------------------------------------------------------------------

def validar_fen(cadena: str) -> tuple[bool, str]:
    """Valida una cadena en notación FEN completa.

    Divide la cadena en sus 6 campos y delega la validación de cada uno
    en las funciones auxiliares correspondientes. Retorna al primer error
    encontrado sin continuar con los campos restantes.

    Args:
        cadena: La cadena FEN a validar (puede tener espacios al inicio
                o al final; serán ignorados mediante ``strip()``).

    Returns:
        Tupla ``(válido, mensaje)`` donde:
            - ``válido`` es ``True`` si la cadena es un FEN correcto.
            - ``mensaje`` es ``'Cadena FEN válida.'`` en ese caso, o una
              descripción del primer error encontrado si no lo es.

    Examples:
        >>> validar_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
        (True, 'Cadena FEN válida.')
        >>> validar_fen("cadena_invalida")
        (False, 'Debe tener exactamente 6 secciones separadas por espacios.')
    """
    partes = cadena.strip().split()

    if len(partes) != 6:
        return False, "Debe tener exactamente 6 secciones separadas por espacios."

    tablero, turno, enroque, al_paso, medio, mov = partes

    # Validar cada campo en orden; retornar ante el primer error
    for validar, args in [
        (_validar_tablero,    (tablero,)),
        (_validar_turno,      (turno,)),
        (_validar_enroque,    (enroque,)),
        (_validar_al_paso,    (al_paso,)),
        (_validar_contadores, (medio, mov)),
    ]:
        ok, mensaje = validar(*args)
        if not ok:
            return False, mensaje

    return True, "Cadena FEN válida."


# ---------------------------------------------------------------------------
# PUNTO DE ENTRADA
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    fen = input("Introduce la cadena FEN a validar:\n> ")
    valido, mensaje = validar_fen(fen)
    print(mensaje)
