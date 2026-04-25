#ifndef MATH_H
#define MATH_H
#include <cstdlib>

//emma aqui estarán las funciones para la simulación de goles que estan en esa formula del desafio

bool probabilidad(double porcentaje) {
    int random = rand() % 10000;
    return random < (porcentaje * 100);
}

double raiz_quinta(double x) {
    if (x <= 0) return 0;
    double bajo = 0.0;
    double alto = x;

    if (x < 1.0) alto = 1.0;

    double medio;
    for (int i = 0; i < 50; i++) {
        medio = bajo + (alto - bajo) / 2.0;
        double potencia_5 = medio * medio * medio * medio * medio;

        if (potencia_5 < x) bajo = medio;
        else alto = medio;
    }
    return medio;
}

//  emma x^0.6 es lo mismo que la raiz quinta de x^3
double raiz_0_6(double x) {
    return raiz_quinta(x * x * x);
}

// y aqui tambien x^0.4 es lo mismo que la raiz quinta de x^2
double raiz_0_4(double x) {
    return raiz_quinta(x * x);
}


#endif // MATH_H
