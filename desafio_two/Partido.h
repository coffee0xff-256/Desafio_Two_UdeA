#ifndef PARTIDO_H
#define PARTIDO_H

#include <iostream>
#include <cstdlib>
#include "clases.h"

using namespace std;

// Mate, esta función la dejé afuera para que cualquier parte del código
// pueda sortear tres números distintos sin repetir, así no se nos cruzan los árbitros.
inline void elegir_arbitros(int& a1, int& a2, int& a3, int total_arb) {
    a1 = rand() % total_arb;
    do { a2 = rand() % total_arb; } while (a2 == a1);
    do { a3 = rand() % total_arb; } while (a3 == a1 || a3 == a2);
}

class partido {
public:
    int dia;
    int hora;
    char grupo;
    int sede;
    int arb1, arb2, arb3;

    // Mate, uso punteros a 'equipo' para que el partido sepa a quiénes apunta
    // sin tener que crear copias de los países cada vez.
    equipo* e1;
    equipo* e2;

    int goles_e1;
    int goles_e2;

    partido() :
        dia(-1), hora(0), grupo('?'),
        sede_idx(-1), arb1(-1), arb2(-1), arb3(-1),
        e1(nullptr), e2(nullptr),
        goles_e1(-1), goles_e2(-1) {}

    // Aquí solo guardamos los índices. Mate, cuando cargues el CSV,
    // estos números te dirán qué nombre mostrar de tu lista de sedes y árbitros.
    void asignar_recursos(int total_sedes, int total_arbitros) {
        if (total_sedes > 0) sede_idx = rand() % total_sedes;
        if (total_arbitros >= 3) elegir_arbitros(arb1, arb2, arb3, total_arbitros);
    }

    // Un chequeo rápido: si los goles no son -1, es que ya se jugó.
    bool jugado() const {
        return goles_e1 >= 0;
    }

    // Mate, en el imprimir ahora solo muestro los IDs de árbitros y sedes.
    // Para que salgan los nombres reales, habría que pasarle los arreglos del CSV.
    void imprimir() const {
        cout << "\n[Grupo " << grupo << "] Dia: " << (20 + dia) << "-Jun"
             << " | Sede ID: " << sede_idx << "\n";

        if (e1 && e2) {
            cout << "  " << e1->pais << " vs " << e2->pais;
            if (jugado()) cout << " (" << goles_e1 << " - " << goles_e2 << ")\n";
            else cout << " (Pendiente)\n";
        }

        cout << "  Arbitros IDs: " << arb1 << ", " << arb2 << ", " << arb3 << "\n";
    }
};

#endif