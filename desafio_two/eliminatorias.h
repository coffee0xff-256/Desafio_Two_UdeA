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

        this_thread::sleep_for(chrono::seconds(2));


    }
}


//pongo esto aqui khanboy para no ensuciar mucho el main

void jugar_fases_finales(equipo* clasificados_r16[32], string arbitros[], int cantidad_arbitros, string sedes[],int cantidad_sedes) {
    equipo* octavos[16];
    equipo* cuartos[8];
    equipo* semis[4];
    equipo* finalistas[2];
    equipo* campeon[1];

    cout << "\n=======================================" << endl;
    cout << "           DIECISEISAVOS DE FINAL      " << endl;
    cout << "=======================================" << endl;
    simular_ronda(clasificados_r16, 32, octavos, "dieciseisavos", arbitros, cantidad_arbitros,sedes,cantidad_sedes);

    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n=======================================" << endl;
    cout << "             OCTAVOS DE FINAL          " << endl;
    cout << "=======================================" << endl;
    simular_ronda(octavos, 16, cuartos, "octavos", arbitros, cantidad_arbitros,sedes,cantidad_sedes);

    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n=======================================" << endl;
    cout << "             CUARTOS DE FINAL          " << endl;
    cout << "=======================================" << endl;
    simular_ronda(cuartos, 8, semis, "cuartos", arbitros, cantidad_arbitros,sedes,cantidad_sedes);

    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n=======================================" << endl;
    cout << "               SEMIFINALES             " << endl;
    cout << "=======================================" << endl;
    simular_ronda(semis, 4, finalistas, "semifinal", arbitros, cantidad_arbitros,sedes,cantidad_sedes);

    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n=======================================" << endl;
    cout << "               GRAN FINAL              " << endl;
    cout << "=======================================" << endl;
    simular_ronda(finalistas, 2, campeon, "gran Final", arbitros, cantidad_arbitros,sedes,cantidad_sedes);

    this_thread::sleep_for(chrono::seconds(2));

    cout << "\n**************************************************" << endl;
    cout << "  EL CAMPEON DEL MUNDIAL UdeAWorldCup ES : " << campeon[0]->pais << " !!!" << endl;
    cout << "**************************************************\n" << endl;
}

#endif // ELIMINATORIAS_H