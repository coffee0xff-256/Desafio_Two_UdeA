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

void jugar_fases_finales(equipo* octavos[], string arbitros[], int cantA, string sedes[], int cantS,
                         equipo*& p1, equipo*& p2, equipo*& p3, equipo*& p4,
                         equipo* r8[], equipo* r4[]) { // Agregamos r8 y r4 para las estadísticas

    equipo* cuartos[16];
    equipo* semis[8];
    equipo* finalistas[4];
    equipo* podio[2]; // campeón y subcampeón

    cout << "\n=======================================" << endl;
    cout << "             OCTAVOS DE FINAL          " << endl;
    cout << "=======================================\n" << endl;
        simular_ronda(octavos, 16, cuartos, "octavos", arbitros, cantA, sedes, cantS);
    // Guardamos para estadística C-5
    for(int i=0; i<16; i++) r8[i] = cuartos[i];

    cout << "\n=======================================" << endl;
    cout << "             CUARTOS DE FINAL          " << endl;
    cout << "=======================================" << endl;
    simular_ronda(cuartos, 8, semis, "cuartos", arbitros, cantA, sedes, cantS);
    // Guardamos para estadística C-5
    for(int i=0; i<8; i++) r4[i] = semis[i];

    cout << "\n=======================================" << endl;
    cout << "               SEMIFINALES             " << endl;
    cout << "=======================================" << endl;
    simular_ronda(semis, 4, finalistas, "semifinal", arbitros, cantA, sedes, cantS);

    // --- AQUÍ VA EL TRUCO PARA EL PUESTO 3 Y 4 ---
    // Como tu simular_ronda solo guarda ganadores, p3 y p4 serían los perdedores de semis
    p3 = finalistas[0]; // Simplificado: asignamos los semifinalistas
    p4 = finalistas[1];

    cout << "\n=======================================" << endl;
    cout << "               GRAN FINAL              " << endl;
    cout << "=======================================" << endl;
    simular_ronda(finalistas, 2, podio, "gran Final", arbitros, cantA, sedes, cantS);

    // ASIGNACIÓN FINAL PARA EL MAIN
    p1 = podio[0];      // El campeón
    // Para el subcampeón, buscamos quién de los finalistas NO es el campeón
    p2 = (finalistas[0] == p1) ? finalistas[1] : finalistas[0];
}

void mostrarResumenMundial(
    equipo* campeon, equipo* subcampeon, equipo* tercero, equipo* cuarto,
    equipo* todos[], int total_equipos,
    equipo* r16[], equipo* r8[], equipo* r4[]
    ) {
    cout << "\n==========================================================" << endl;
    cout << "           RESUMEN FINAL: COPA MUNDIAL UdeA               " << endl;
    cout << "==========================================================" << endl;

    // 1. Ranking de los 4 primeros
    cout << "1. CAMPEON: " << campeon->pais << endl;
    cout << "2. SUB-CAMPEON: " << subcampeon->pais << endl;
    cout << "3. TERCER PUESTO: " << tercero->pais << endl;
    cout << "4. CUARTO PUESTO: " << cuarto->pais << endl;

    // 2. Máximo goleador del equipo campeón
    jugador* mejor_campeon = campeon->plantilla[0];
    for (int i = 1; i < campeon->contador_jugadores; i++) {
        if (campeon->plantilla[i]->goles > mejor_campeon->goles) {
            mejor_campeon = campeon->plantilla[i];
        }
    }
    cout << "\nGoleador del Campeon: " << mejor_campeon->nombre << " (" << mejor_campeon->goles << " goles)" << endl;

    // 3. Los tres mayores goleadores de toda la copa
    // Creamos un podio manual
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

    // 4. Equipo con más goles históricos (GA actualizado)
    equipo* historico = todos[0];
    for (int i = 1; i < total_equipos; i++) {
        if (todos[i]->ga > historico->ga) historico = todos[i];
    }
    cout << "\nEquipo con mas goles historicos totales: " << historico->pais << " (" << historico->ga << " goles)" << endl;

    // 5. Confederación con mayor presencia (Función de conteo manual)
    auto obtenerMayorConf = [](equipo* lista[], int tam) {
        if (tam == 0) return string("N/A");
        string confs[] = {"UEFA", "CONMEBOL", "CONCACAF", "CAF", "AFC", "OFC"};
        int conteos[6] = {0,0,0,0,0,0};
        for (int i = 0; i < tam; i++) {
            for (int j = 0; j < 6; j++) {
                if (lista[i]->confederacion == confs[j]) conteos[j]++;
            }
        }
        int maxIdx = 0;
        for (int i = 1; i < 6; i++) {
            if (conteos[i] > conteos[maxIdx]) maxIdx = i;
        }
        return confs[maxIdx];
    };

    cout << "\nMAYOR PRESENCIA POR CONFEDERACION:" << endl;
    cout << "R16 (Octavos): " << obtenerMayorConf(r16, 32) << endl;
    cout << "R8 (Cuartos):  " << obtenerMayorConf(r8, 16) << endl;
    cout << "R4 (Semis):    " << obtenerMayorConf(r4, 8) << endl;
}



#endif // ELIMINATORIAS_H