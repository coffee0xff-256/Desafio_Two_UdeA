#include <iostream>
#include <ctime>
#include <cstdlib>
#include "clases.h"
#include "funciones_auxiliares.h"
#include "bombos.h"
#include "math.h"
#include "simulacion.h"

using namespace std;

int main() {

    srand(time(0));

    equipo* torneo[48];
    int cantidad_equipos = 0;


    string arbitros[150];
    int cantidad_arbitros = 0;

    cout << "=== CARGA DE DATOS ===" << endl;
    cargar_datos(torneo, cantidad_equipos);
    cargar_confederaciones(torneo, cantidad_equipos);
    cargar_arbitros(arbitros, cantidad_arbitros);

    cout << "Datos cargados correctamente.\n" << endl;

    cout << "=== SORTEO DEL MUNDIAL UdeA WorldCup ===" << endl;
    Sorteo sorteo;
    sorteo.conformarbombos(torneo, cantidad_equipos);
    sorteo.realizarSorteo();
    sorteo.imprimirGrupos();

    cout << "\n==================================================" << endl;
    cout << "       INICIANDO FASE DE GRUPOS (18 DIAS)         " << endl;
    cout << "==================================================" << endl;

    int dia_actual = 1;

    //  las 3 jornadas de la fase de grupos
    for(int jornada = 0; jornada < 3; jornada++) {

        // ema iteramos sobre los 12 grupos, avanzando de 2 en 2 (A-B, C-D, E-F...)
        for(int g = 0; g < 12; g += 2) {
            cout << "\n////////////////////////////////////////" << endl;
            cout << "               DIA " << dia_actual << endl;
            cout << "////////////////////////////////////////\n" << endl;

            //  los 2 grupos programados para este dia
            for(int offset = 0; offset < 2; offset++) {
                Grupo* grupo_actual = sorteo.getGrupo(g + offset);

                //  4 equipos del grupo
                equipo* eq1 = grupo_actual->getEquipo(0);
                equipo* eq2 = grupo_actual->getEquipo(1);
                equipo* eq3 = grupo_actual->getEquipo(2);
                equipo* eq4 = grupo_actual->getEquipo(3);

                equipo* local1; equipo* vis1;
                equipo* local2; equipo* vis2;

                //  cruces dependiendo de la jornada
                if (jornada == 0) { // Fecha 1
                    local1 = eq1; vis1 = eq2;
                    local2 = eq3; vis2 = eq4;
                } else if (jornada == 1) { // Fecha 2
                    local1 = eq1; vis1 = eq3;
                    local2 = eq2; vis2 = eq4;
                } else { // Fecha 3
                    local1 = eq1; vis1 = eq4;
                    local2 = eq2; vis2 = eq3;
                }

                // --- PARTIDO 1 DEL GRUPO ---
                // ema  3 arbitros al azar de arbitros.csv
                string a1 = arbitros[rand() % cantidad_arbitros];
                string a2 = arbitros[rand() % cantidad_arbitros];
                string a3 = arbitros[rand() % cantidad_arbitros];

                Partido p1(local1, vis1, "Dia " + to_string(dia_actual), a1, a2, a3);
                cout << "--- Grupo " << grupo_actual->getLetra() << " ---" << endl;
                p1.simular();

                // --- PARTIDO 2 DEL GRUPO ---
                string b1 = arbitros[rand() % cantidad_arbitros];
                string b2 = arbitros[rand() % cantidad_arbitros];
                string b3 = arbitros[rand() % cantidad_arbitros];

                Partido p2(local2, vis2, "Dia " + to_string(dia_actual), b1, b2, b3);
                p2.simular();
            }
            // avanzamos al siguiente dia una vez que juegan los 2 grupos
            dia_actual++;
        }
    }


    cout << "\n=== GUARDANDO RESULTADOS EN ARCHIVOS CSV ===" << endl;

    //estadisticas de los jugadores
    guardar_datos(torneo, cantidad_equipos);

    //  las estadisticas de los equipos
    actualizar_equipos_csv(torneo, cantidad_equipos);

    cout << "Simulacion terminada y datos persistidos." << endl;

    // Liberar memoria
    for(int i = 0; i < cantidad_equipos; i++) {
        delete torneo[i];
    }

    // emma de aqui empezare a trabajar con las otras pertes del torneo R

    return 0;}
