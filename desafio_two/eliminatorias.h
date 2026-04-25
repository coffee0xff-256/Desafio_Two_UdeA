#ifndef ELIMINATORIAS_H
#define ELIMINATORIAS_H

#include <iostream>
#include <clases.h>
#include <bombos.h>
#include <simulacion.h>

//librerias externa unicamente para que en terminal se vea mejor que es para ponerle un delay como en arduino

#include <thread>
#include <chrono>

using namespace std;


// aqui van las clasificaciones emma

bool es_mejor(equipo* a, equipo* b) {
    int puntosa = (a->pg * 3) + a->pe;
    int puntosb = (b->pg * 3) + b->pe;
    if (puntosa != puntosb) return puntosa > puntosb;

    int difa = a->ga - a->gc;
    int difb = b->ga - b->gc;
    if (difa != difb) return difa > difb;

    return a->ga > b->ga;
}


// aqui van las clasificaciones emma, es decir los 32 equipos

void clasificar_r16(Sorteo& sorteo, equipo* clasificados_r16[32]) {
    int indice_clasificados = 0;
    equipo* terceros[12];
    int indice_terceros = 0;

    for (int g = 0; g < 12; g++) {
        Grupo* grupo = sorteo.getGrupo(g);
        equipo* tabla[4];

        for (int i = 0; i < 4; i++) tabla[i] = grupo->getEquipo(i);

        // necesitamos organizar esto, utilizare bubbleshort
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3 - i; j++) {
                if (!es_mejor(tabla[j], tabla[j+1])) {
                    equipo* temp = tabla[j];
                    tabla[j] = tabla[j+1];
                    tabla[j+1] = temp;
                }
            }
        }
        // pasan 1ro y 2do directo

        clasificados_r16[indice_clasificados++] = tabla[0];
        clasificados_r16[indice_clasificados++] = tabla[1];

        // 3ro va a lista de espera
        terceros[indice_terceros++] = tabla[2];
    }

    // Ordenar terceros y sacar los 8 mejores
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 11 - i; j++) {
            if (!es_mejor(terceros[j], terceros[j+1])) {
                equipo* temp = terceros[j];
                terceros[j] = terceros[j+1];
                terceros[j+1] = temp;
            }
        }
    }
    for(int i = 0; i < 8; i++) {
        clasificados_r16[indice_clasificados++] = terceros[i];
    }
}

//aqui emma aplico el polimorfismo que te hable de las eliminatorias


void simular_ronda(equipo* equipos_in[], int num_equipos, equipo* equipos_out[], string nombre_fase, string arbitros[], int cantidad_arbitros, string sedes[],int cantidad_sedes) {
    int indice_out = 0;
    for (int i = 0; i < num_equipos; i += 2) {
        string a1 = arbitros[rand() % cantidad_arbitros];
        string a2 = arbitros[rand() % cantidad_arbitros];
        string a3 = arbitros[rand() % cantidad_arbitros];
        string s = sedes[rand() % cantidad_sedes];
        Partido p(equipos_in[i], equipos_in[i+1], nombre_fase, a1, a2, a3,s);
        p.simular_eliminatoria();

        equipos_out[indice_out++] = p.ganador;



    }
}


//pongo esto aqui khanboy para no ensuciar mucho el main

void jugar_fases_finales(equipo* r16[], string arbitros[], int cantA, string sedes[], int cantS,
                         equipo*& puesto1, equipo*& puesto2, equipo*& puesto3, equipo*& puesto4,
                         equipo* r8[], equipo* r4[]) {

    cout << "\n=======================================";
    cout << "\n        INICIAN LOS OCTAVOS            ";
    cout << "\n=======================================" << endl;
    simular_ronda(r16, 16, r8, "OCTAVOS DE FINAL", arbitros, cantA, sedes, cantS);

    cout << "\n=======================================";
    cout << "\n        INICIAN LOS CUARTOS            ";
    cout << "\n=======================================" << endl;
    simular_ronda(r8, 8, r4, "CUARTOS DE FINAL", arbitros, cantA, sedes, cantS);

    cout << "\n=======================================";
    cout << "\n          SEMIFINALES                  ";
    cout << "\n=======================================" << endl;
    equipo* finalistas[2];
    simular_ronda(r4, 4, finalistas, "SEMIFINALES", arbitros, cantA, sedes, cantS);


    equipo* perdedor_semi1 = (finalistas[0] == r4[0]) ? r4[1] : r4[0];
    equipo* perdedor_semi2 = (finalistas[1] == r4[2]) ? r4[3] : r4[2];

    cout << "\n=======================================";
    cout << "\n     PARTIDO POR EL TERCER PUESTO      ";
    cout << "\n=======================================" << endl;
    Partido p_tercero(perdedor_semi1, perdedor_semi2, "TERCER PUESTO",
                      arbitros[rand() % cantA], arbitros[rand() % cantA], arbitros[rand() % cantA],
                      sedes[rand() % cantS]);

    p_tercero.simular_eliminatoria(); // Usa la función que editamos anteriormente

    puesto3 = p_tercero.ganador;
    puesto4 = (puesto3 == perdedor_semi1) ? perdedor_semi2 : perdedor_semi1;

    cout << "\n=======================================";
    cout << "\n             GRAN FINAL                ";
    cout << "\n=======================================" << endl;
    Partido p_final(finalistas[0], finalistas[1], "GRAN FINAL",
                    arbitros[rand() % cantA], arbitros[rand() % cantA], arbitros[rand() % cantA],
                    sedes[rand() % cantS]);

    p_final.simular_eliminatoria();

    puesto1 = p_final.ganador;
    puesto2 = (puesto1 == finalistas[0]) ? finalistas[1] : finalistas[0];
}

void mostrarResumenMundial(
    equipo* campeon, equipo* subcampeon, equipo* tercero, equipo* cuarto,
    equipo* todos[], int total_equipos
    ) {
    cout << "\n==========================================================" << endl;
    cout << "           RESUMEN FINAL: COPA MUNDIAL UdeAWorldCup               " << endl;
    cout << "==========================================================" << endl;

    cout << "1. CAMPEON: " << campeon->pais << endl;
    cout << "2. SUB-CAMPEON: " << subcampeon->pais << endl;
    cout << "3. TERCER PUESTO: " << tercero->pais << endl;
    cout << "4. CUARTO PUESTO: " << cuarto->pais << endl;


    jugador* mejor_campeon = campeon->plantilla[0];
    for (int i = 1; i < campeon->contador_jugadores; i++) {
        if (campeon->plantilla[i]->goles > mejor_campeon->goles) {
            mejor_campeon = campeon->plantilla[i];
        }
    }
    cout << "\nGoleador del Campeon: " << mejor_campeon->nombre << " (" << mejor_campeon->goles << " goles)" << endl;

    //emma cogemos los 3 mayores goleadores del torneo

    jugador* top1 = nullptr; jugador* top2 = nullptr; jugador* top3 = nullptr;
    int g1 = -1, g2 = -1, g3 = -1;

    for (int i = 0; i < total_equipos; i++) {
        for (int j = 0; j < todos[i]->contador_jugadores; j++) {
            jugador* act = todos[i]->plantilla[j];
            if (act->goles > g1) {
                g3 = g2; top3 = top2;
                g2 = g1; top2 = top1;
                g1 = act->goles; top1 = act;
            } else if (act->goles > g2) {
                g3 = g2; top3 = top2;
                g2 = act->goles; top2 = act;
            } else if (act->goles > g3) {
                g3 = act->goles; top3 = act;
            }
        }
    }
    cout << "\nTOP 3 GOLEADORES DEL MUNDIAL:" << endl;
    if (top1) cout << "1. " << top1->nombre << " " << top1->apellido << " (" << g1 << " goles)" << endl;
    if (top2) cout << "2. " << top2->nombre << " " << top2->apellido << " (" << g2 << " goles)" << endl;
    if (top3) cout << "3. " << top3->nombre << " " << top3->apellido << " (" << g3 << " goles)" << endl;

    equipo* historico = todos[0];
    for (int i = 1; i < total_equipos; i++) {
        if (todos[i]->ga > historico->ga) historico = todos[i];
    }
    cout << "\nEquipo con mas goles historicos totales: " << historico->pais << " (" << historico->ga << " goles)" << endl;


}



#endif // ELIMINATORIAS_H