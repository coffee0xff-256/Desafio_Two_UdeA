#include <iostream>
#include <ctime>
#include <cstdlib>
#include <clases.h>
#include <funciones_auxiliares.h>
#include <bombos.h>
#include <math.h>
#include <simulacion.h>
#include <eliminatorias.h>
#include <fase_grupos.h>

//librerias externa unicamente para que en terminal se vea mejor que es para ponerle un delay como en arduino

#include <thread>
#include <chrono>


using namespace std;

int main() {




    srand(time(0));

    equipo* torneo[48];
    int cantidad_equipos = 0;


    string arbitros[150];
    int cantidad_arbitros = 0;

    string sedes[20];
    int cantidad_sedes = 0;
    cargar_sedes(sedes, cantidad_sedes);

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


    tabladegrupo tablas[12];
    for(int i = 0; i < 12; i++) {
        tablas[i].inicializar(sorteo.getGrupo(i), (char)('A' + i));
    }

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

                int indice_real_grupo = g + offset;
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
                string s = sedes[rand() % cantidad_sedes];


                Partido p1(local1, vis1, "Dia " + to_string(dia_actual), a1, a2, a3,s);
                cout << "--- Grupo " << grupo_actual->getLetra() << " ---" << endl;
                p1.simular();

                tablas[indice_real_grupo].registrar_resultado(local1,p1.goles_L_hoy,vis1,p1.goles_V_hoy);

                // --- PARTIDO 2 DEL GRUPO ---
                string b1 = arbitros[rand() % cantidad_arbitros];
                string b2 = arbitros[rand() % cantidad_arbitros];
                string b3 = arbitros[rand() % cantidad_arbitros];
                 s = sedes[rand() % cantidad_sedes];

                Partido p2(local2, vis2, "Dia " + to_string(dia_actual), b1, b2, b3,s);
                p2.simular();

                tablas[indice_real_grupo].registrar_resultado(local2,p2.goles_L_hoy,vis2,p2.goles_V_hoy);

            }
            // avanzamos al siguiente dia una vez que juegan los 2 grupos
            dia_actual++;
        }
    }
    cout << "\n==========================================================";
    cout << "\n           RESULTADOS FINALES FASE DE GRUPOS              ";
    cout << "\n==========================================================\n";
    for(int i = 0; i < 12; i++) {
        tablas[i].ordenar();
        tablas[i].imprimirTablaCompleta();
    }

    equipo* clasificados_r16[32];
    clasificar_r16(sorteo, clasificados_r16);

    equipo* puesto1 = nullptr;
    equipo* puesto2 = nullptr;
    equipo* puesto3 = nullptr;
    equipo* puesto4 = nullptr;
    equipo* r8[16]; // Para confederación en cuartos
    equipo* r4[8];  // Para confederación en semis

    jugar_fases_finales(clasificados_r16, arbitros, cantidad_arbitros, sedes, cantidad_sedes,
                       puesto1, puesto2, puesto3, puesto4, r8, r4);

   // mostrarResumenMundial(puesto1, puesto2, puesto3, puesto4, torneo, cantidad_equipos, clasificados_r16, clasificados_r8, clasificados_r4);

    if(puesto1 != nullptr) {
        mostrarResumenMundial(puesto1, puesto2, puesto3, puesto4, torneo, cantidad_equipos, clasificados_r16, r8, r4);
    }

    cout << "\n=== GUARDANDO RESULTADOS EN ARCHIVOS CSV ===" << endl;

    guardar_datos(torneo, cantidad_equipos);

    actualizar_equipos_csv(torneo, cantidad_equipos);

    cout << "Simulacion terminada y datos persistidos." << endl;

    // 4. Liberamos la memoria
    for(int i = 0; i < cantidad_equipos; i++) {
        delete torneo[i];
    }






    return 0;}





