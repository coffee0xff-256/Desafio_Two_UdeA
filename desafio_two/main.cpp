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
#include <memoria.h>
using namespace std;

int main() {


    srand(time(0));

    equipo* torneo[48];
    int cantidad_equipos = 0;


    string arbitros[150];
    int cantidad_arbitros = 0;

    string sedes[20];
    int cantidad_sedes = 0;

    cout << "=== CARGA DE DATOS ===" << endl;
    //cargar_datos(torneo, cantidad_equipos);
    //cargar_confederaciones(torneo, cantidad_equipos);
    //cargar_arbitros(arbitros, cantidad_arbitros);

    memoria::iniciar();
    cargar_sedes(sedes, cantidad_sedes);
    cargar_datos(torneo, cantidad_equipos);
    cargar_confederaciones(torneo, cantidad_equipos);
    cargar_arbitros(arbitros, cantidad_arbitros);
    memoria::sumar(cantidad_equipos * 27 + cantidad_arbitros + cantidad_sedes); //
    cout << "Datos cargados correctamente.\n";
    memoria::reportar("Carga de datos correctamente", torneo, cantidad_equipos);

    cout << "=== SORTEO DEL MUNDIAL UdeA WorldCup ===" << endl;

    memoria::iniciar();
    Sorteo sorteo;
    sorteo.conformarbombos(torneo, cantidad_equipos);
    sorteo.realizarSorteo();
    sorteo.imprimirGrupos();
    memoria::sumar(48 * 4 + cantidad_equipos);
    memoria::reportar("Sorteo de grupos", torneo, cantidad_equipos);

    tabladegrupo tablas[12];
    for(int i = 0; i < 12; i++) {
        tablas[i].inicializar(sorteo.getGrupo(i), (char)('A' + i));
    }

    cout << "\n==================================================" << endl;
    cout << "       INICIANDO FASE DE GRUPOS (18 DIAS)         " << endl;
    cout << "==================================================" << endl;

    memoria::iniciar();

    int dia_actual = 1;
    for(int jornada = 0; jornada < 3; jornada++) {
        // ema iteramos sobre los 12 grupos, avanzando de 2 en 2
        for(int g = 0; g < 12; g += 2) {
            cout << "\n////////////////////////////////////////" << endl;
            cout << "               DIA " << dia_actual << endl;
            cout << "////////////////////////////////////////\n" << endl;


            for(int offset = 0; offset < 2; offset++) {

                int indice_real_grupo = g + offset;
                Grupo* grupo_actual = sorteo.getGrupo(g + offset);


                equipo* eq1 = grupo_actual->getEquipo(0);
                equipo* eq2 = grupo_actual->getEquipo(1);
                equipo* eq3 = grupo_actual->getEquipo(2);
                equipo* eq4 = grupo_actual->getEquipo(3);

                equipo* local1; equipo* vis1;
                equipo* local2; equipo* vis2;


                if (jornada == 0) {
                    local1 = eq1; vis1 = eq2;
                    local2 = eq3; vis2 = eq4;
                } else if (jornada == 1) {
                    local1 = eq1; vis1 = eq3;
                    local2 = eq2; vis2 = eq4;
                } else {
                    local1 = eq1; vis1 = eq4;
                    local2 = eq2; vis2 = eq3;
                }


                // ema  3 arbitros al azar de arbitros.csv
                string a1 = arbitros[rand() % cantidad_arbitros];
                string a2 = arbitros[rand() % cantidad_arbitros];
                string a3 = arbitros[rand() % cantidad_arbitros];
                string s = sedes[rand() % cantidad_sedes];


                Partido p1(local1, vis1, "Dia " + to_string(dia_actual), a1, a2, a3,s);
                cout << "--- Grupo " << grupo_actual->getLetra() << " ---" << endl;
                p1.simular();

                tablas[indice_real_grupo].registrar_resultado(local1,p1.goles_L_hoy,vis1,p1.goles_V_hoy);


                string b1 = arbitros[rand() % cantidad_arbitros];
                string b2 = arbitros[rand() % cantidad_arbitros];
                string b3 = arbitros[rand() % cantidad_arbitros];
                s = sedes[rand() % cantidad_sedes];

                Partido p2(local2, vis2, "Dia " + to_string(dia_actual), b1, b2, b3,s);
                p2.simular();

                tablas[indice_real_grupo].registrar_resultado(local2,p2.goles_L_hoy,vis2,p2.goles_V_hoy);

            }

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

    memoria::sumar(36 * 22 + 12 * 12);
    memoria::reportar("Fase de grupos", torneo, cantidad_equipos);

    equipo* clasificados_r16[32];

    memoria::iniciar();

    clasificar_r16(sorteo, clasificados_r16);

    equipo* puesto1 = nullptr;
    equipo* puesto2 = nullptr;
    equipo* puesto3 = nullptr;
    equipo* puesto4 = nullptr;
    equipo* r8[16];
    equipo* r4[8];

    jugar_fases_finales(clasificados_r16, arbitros, cantidad_arbitros, sedes, cantidad_sedes,
                        puesto1, puesto2, puesto3, puesto4, r8, r4);

    memoria::sumar(32*22);
    memoria::reportar("Fases finales", torneo, cantidad_equipos);


    if(puesto1 != nullptr) {
        mostrarResumenMundial(puesto1, puesto2, puesto3, puesto4, torneo, cantidad_equipos);
        memoria::iniciar();
        memoria::sumar(cantidad_equipos * 26);
        memoria::reportar("Estadisticas", torneo, cantidad_equipos);

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





