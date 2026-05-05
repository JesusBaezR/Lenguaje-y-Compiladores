#include <stdio.h>

// Ejemplo de uso de palabras reservadas en C
int main() {
    int i;
    for (i = 0; i < 5; i++) {
        if (i == 2) {
            continue;
        }
        printf("i es: %d\n", i);
    }
    return 0;
}
