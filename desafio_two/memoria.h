#ifndef MEMORIA_H
#define MEMORIA_H
#ifndef METRICAS_H
#define METRICAS_H

#include <iostream>
#include <string>
#include "clases.h"
class memoria {
    // Mateo, estas variables guardan los conteos.
    // Les puse 'inline' para poder definirlas acá mismo sin que el programa se confunda
    // si usamos este archivo en varias partes del proyecto.
    inline static long long iteraciones = 0;   // Las vueltas de la función que estamos revisando ahora
    inline static long long iter_total = 0;    // Las vueltas acumuladas de todo el torneo

public:

    // Mateo, llamamos a esto ANTES de iniciar una tarea nueva para que el contador arranque de cero.
    static void iniciar() {
        iteraciones = 0;
    }

    // Mateo, pon esto dentro de los ciclos (como los 'for' o 'while') para ir sumando las vueltas.
    // Ejemplo: Metricas::sumar(1);
    static void sumar(long long n = 1) {
        iteraciones += n;
        iter_total  += n;
    }

    // Mateo, esto calcula cuánta memoria (en bytes) están ocupando los equipos y jugadores.
    // Le pasamos la lista del torneo y cuántos equipos hay en total.
    static long long calcularMemoria(equipo* torneo[], int cantidad) {
        long long total = 0;

        // Sumamos el "peso" del arreglo que guarda a los equipos
        total += cantidad * sizeof(equipo*);

        for (int i = 0; i < cantidad; i++) {
            // Si el espacio está vacío, lo saltamos
            if (torneo[i] == nullptr) continue;

            // Sumamos lo que pesa la estructura del equipo en sí
            total += sizeof(equipo);

            // Y acá sumamos lo que pesa cada jugador que esté en la plantilla de este equipo
            for (int j = 0; j < torneo[i]->contador_jugadores; j++) {
                if (torneo[i]->plantilla[j] != nullptr) {
                    total += sizeof(jugador);
                }
            }
        }

        // Mateo, acá sumamos la memoria extra de los árbitros y las sedes que usamos en el main.
        // Asumimos un tamaño promedio para los textos (strings).
        total += 150 * sizeof(std::string);
        total += 20  * sizeof(std::string);
        return total;
    }

    // Mateo, esto simplemente imprime el resumen en pantalla al final de cada tarea que evaluamos.
    static void reportar(const std::string& nombre, equipo* torneo[], int cantidad) {
        long long mem = calcularMemoria(torneo, cantidad);
        std::cout << "  [Consuma de memoria] Fase actual: " << nombre << "\n";
        std::cout << "  Vueltas en esta fase  : " << iteraciones << "\n";
        std::cout << "  Vueltas en total      : " << iter_total  << "\n";
        std::cout << "  Memoria consumida     : " << mem         << " bytes";
        std::cout << "  (~" << mem / 1024 << " KB)\n";
    }
};

#endif // METRICAS_H

#endif // MEMORIA_H
