#ifndef PARTIDO_H
#define PARTIDO_H

#include <iostream>
#include <cstdlib>
#include "clases.h"

using namespace std;

// Mate, esta función la dejé afuera para que cualquier parte del código
// pueda sortear tres números distintos sin repetir, así no se nos cruzan los árbitros.
struct convocados{
    jugador* jugador;
    int goles;
    convocados(): jugador(nullptr), goles(0){}
};

class partido {
public:
    int dia;
    int hora;
    char grupo;
    int sede_partido;
    int arb1, arb2, arb3;

    // Mate, uso punteros a los equipos para que el partido sepa a quiénes apunta
    // sin tener que crear copias de los países cada vez.
    equipo* e1;
    equipo* e2;

    int goles_e1;
    int goles_e2;

    convocados convocados_e1[11];
    convocados convocados_e2[11];

    partido() :
        dia(-1), grupo('?'),
        sede_patido(-1), arb1(-1), arb2(-1), arb3(-1),
        e1(nullptr), e2(nullptr),
        goles_e1(-1), goles_e2(-1) {}

    bool jugado() const{return goles_e1 >= 0;}

    // Aquí solo guardamos los índices. Mate, cuando cargues el CSV,
    // estos números te dirán qué nombre mostrar de tu lista de sedes y árbitros.
    void asignar_recursos(int total_sedes, int total_arbitros) {
        if (total_sedes > 0)
            sede_partido = rand() % total_sedes;
        if (total_arbitros >= 3) {
            arb1 = rand() % total_arbitros;
            do { arb2 = rand() % total_arbitros; } while (arb2 == arb1);
            do { arb3 = rand() % total_arbitros; } while (arb3 == arb1 || arb3 == arb2);
        }
    }

    static int goles_por_ranking(int ranking){
        int max_goles = 4 - (ranking*3)/60;
        if (max_goles < 1) max_goles = 1;
        if (max_goles > 4) max_goles = 4;
        return rand() % (max_goles + 1);

    }

    static void elegir_once(equipo* eq, int indices[11]) {
        int n = eq->contador_jugadores < 26 ? eq->contador_jugadores : 26;
        int disponibles[26];
        for (int i = 0; i < n; i++) disponibles[i] = i;

        for (int i = 0; i < 11 && i < n; i++) {
            int j = i + rand() % (n - i);
            int tmp     = disponibles[i];
            disponibles[i] = disponibles[j];
            disponibles[j] = tmp;
            indices[i]  = disponibles[i];
        }
        for (int i = n; i < 11; i++) indices[i] = 0;
    }

    static void repartir_goles_conv(convocados conv[11], int total_goles) {
        for (int g = 0; g < total_goles; g++)
            conv[rand() % 11].goles++;
    }

    void simular(bool eliminatoria, int total_sedes, int total_arbitros) {
        asignar_recursos(total_sedes, total_arbitros);

        goles_e1 = goles_por_ranking(e1->ranking);
        goles_e2 = goles_por_ranking(e2->ranking);

        if (eliminatoria && goles_e1 == goles_e2) {
            if      (e1->ranking < e2->ranking) goles_e1++;
            else if (e2->ranking < e1->ranking) goles_e2++;
            else    (rand() % 2 == 0) ? goles_e1++ : goles_e2++;
        }

        int idx1[11], idx2[11];
        elegir_once(e1, idx1);
        elegir_once(e2, idx2);
        for (int i = 0; i < 11; i++) convocados_e1[i].jugador= e1->plantilla[idx1[i]];
        for (int i = 0; i < 11; i++) convocados_e2[i].jugador = e2->plantilla[idx2[i]];

        repartir_goles_conv(convocados_e1, goles_e1);
        repartir_goles_conv(convocados_e2, goles_e2);
    }


    // Mate, en el imprimir ahora solo muestro los IDs de árbitros y sedes.
    // Para que salgan los nombres reales, habría que pasarle los arreglos del CSV.
    void imprimir(const string sedes[], int total_sedes,
                  const string arbitros[], int total_arbitros) const {

        if (grupo != '?')
            cout << "[Grupo " << grupo << "] ";

        if (dia >= 0)
            cout << "Fecha: " << (20 + dia) << "-Jun-2026 | ";
        else
            cout << "Fecha: 01-01-2026 | ";

        if (sede_partido >= 0 && sede_partido < total_sedes)
            cout << "Sede: " << sedes[sede_partido] << "\n";
        else
            cout << "Sede: -\n";

        if (e1 && e2) {
            cout << "  " << e1->pais << " vs " << e2->pais;
            if (jugado())
                cout << "  ->  " << goles_e1 << " - " << goles_e2 << "\n";
            else
                cout << "  (Pendiente)\n";
        }

        if (arb1 >= 0 && arb1 < total_arbitros)
            cout << "  Arbitros: " << arbitros[arb1]
                 << ", "           << arbitros[arb2]
                 << ", "           << arbitros[arb3] << "\n";

        if (jugado()) {
            // Goleadores equipo 1
            cout << "  Goleadores " << e1->pais << ": ";
            bool alguno = false;
            for (int i = 0; i < 11; i++) {
                if (convocados_e1[i].goles > 0 && convocados_e1[i].jugador) {
                    cout << "#" << convocados_e1[i].jugador->dorsal
                         << "(" << convocados_e1[i].goles << ") ";
                    alguno = true;
                }
            }
            if (!alguno) cout << "-";
            cout << "\n";

            // Goleadores equipo 2
            cout << "  Goleadores " << e2->pais << ": ";
            alguno = false;
            for (int i = 0; i < 11; i++) {
                if (convocados_e2[i].goles > 0 && convocados_e2[i].jugador) {
                    cout << "#" << convocados_e2[i].jugador->dorsal
                         << "(" << convocados_e2[i].goles << ") ";
                    alguno = true;
                }
            }
            if (!alguno) cout << "-";
            cout << "\n";
        }
    }
};

#endif