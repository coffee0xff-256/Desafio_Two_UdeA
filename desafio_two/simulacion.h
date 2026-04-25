#ifndef SIMULACION_H
#define SIMULACION_H


//librerias externa unicamente para que en terminal se vea mejor que es para ponerle un delay como en arduino

#include <thread>
#include <chrono>
using namespace std;


class Partido {
public:
    equipo* local;
    equipo* visitante;
    string fecha;
    string hora = "00:00";
    string arb1, arb2, arb3;
    string sede = "";

    int goles_L_hoy = 0;
    int goles_V_hoy = 0; //aqui ema me toco poner estos atributos publicos

    double posesion_local = 0;
    double posesion_visitante = 0;

    Partido(equipo* l, equipo* v, string f, string a1, string a2, string a3, string s)
        : local(l), visitante(v), fecha(f), arb1(a1), arb2(a2), arb3(a3) , sede(s){}

    void calcularPosesion() {
        double rankL = local->ranking == 0 ? 1 : local->ranking;
        double rankV = visitante->ranking == 0 ? 1 : visitante->ranking;
        posesion_local = (rankV / (rankL + rankV)) * 100.0;
        posesion_visitante = (rankL / (rankL + rankV)) * 100.0;
    }

    int calcularGolesEsperados(equipo* a, equipo* b) {
        double niu = 1.35;
        double GFa = a->ga == 0 ? 1 : a->ga;
        double GCb = b->gc == 0 ? 1 : b->gc;

        double part1 = raiz_0_6(GFa / niu);
        double part2 = raiz_0_4(GCb / niu);

        double lambda = niu * part1 * part2;

        return (int)(lambda + 0.5); // ema un redondeo manual
    }

    int simularJugadores(equipo* eq, int goles_meta) {
        int goles_hoy = 0;

        for(int i = 0; i < 11; i++) {
            jugador* j = eq->plantilla[i];

            j->partidos_jugados += 1;
            j->minutos_jugados += 90;

            if (probabilidad(13.0)) { j->faltas += 1; eq->faltas += 1; }
            if (probabilidad(2.75)) { j->faltas += 1; eq->faltas += 1; }
            if (probabilidad(0.7))  { j->faltas += 1; eq->faltas += 1; }

            int amarillas_hoy = 0;
            if (probabilidad(6.0)) amarillas_hoy++;
            if (probabilidad(1.15)) amarillas_hoy++;

            if (amarillas_hoy == 2) {
                j->t_rojas += 1;
                eq->t_r += 1;
            } else if (amarillas_hoy == 1) {
                j->t_amarillas += 1;
                eq->t_a += 1;
            }

            while(goles_hoy < goles_meta && probabilidad(4.0)) {
                j->goles += 1;
                eq->ga += 1;
                goles_hoy++;
                cout << "  -> Gol de: " << j->nombre << " " << j->apellido << " (#" << j->dorsal << ")\n";



            }
        }
        return goles_hoy;
    }

    void simular() {
        cout << "\n=== " << local->pais << " vs " << visitante->pais << " ===" << endl;
        cout << "Fecha: " << fecha << " | Sede: " << sede << endl;
        cout << "Arbitros: " << arb1 << ", " << arb2 << ", " << arb3 << endl;

        calcularPosesion();
        cout << "Posesion: " << local->pais << " " << posesion_local << "% - "
             << visitante->pais << " " << posesion_visitante << "%" << endl;
        int meta_local = calcularGolesEsperados(local, visitante);
        int meta_visitante = calcularGolesEsperados(visitante, local);

        cout << "\nAnotaciones de " << local->pais << ":" << endl;
         goles_L_hoy = simularJugadores(local, meta_local);

        cout << "\nAnotaciones de " << visitante->pais << ":" << endl;
        goles_V_hoy = simularJugadores(visitante, meta_visitante);

        local->gc += goles_V_hoy;
        visitante->gc += goles_L_hoy;

        if (goles_L_hoy > goles_V_hoy) {
            local->pg += 1; visitante->pp += 1;
        } else if (goles_V_hoy > goles_L_hoy) {
            visitante->pg += 1; local->pp += 1;
        } else {
            local->pe += 1; visitante->pe += 1;
        }

        cout << "RESULTADO FINAL: " << local->pais << " " << goles_L_hoy << " - "
             << goles_V_hoy << " " << visitante->pais << "\n==============================\n";

    }

    equipo* ganador = nullptr;

    void simular_eliminatoria() {
        cout << "\n=== [ELIMINATORIA] " << local->pais << " vs " << visitante->pais << " ===" << endl;
        cout << "Fecha: " << fecha << " | Sede: " << sede << endl;
        cout << "Arbitros: " << arb1 << ", " << arb2 << ", " << arb3 << endl;

        calcularPosesion();
        cout << "Posesion: " << local->pais << " " << posesion_local << "% - "
             << visitante->pais << " " << posesion_visitante << "%" << endl;

        int meta_local = calcularGolesEsperados(local, visitante);
        int meta_visitante = calcularGolesEsperados(visitante, local);

        cout << "\nAnotaciones de " << local->pais << ":" << endl;
        goles_L_hoy = simularJugadores(local, meta_local);

        cout << "\nAnotaciones de " << visitante->pais << ":" << endl;
        goles_V_hoy = simularJugadores(visitante, meta_visitante);

        // emma sumo estadisticas de goles
        local->gc += goles_V_hoy;
        visitante->gc += goles_L_hoy;

        //esto es para  los minutos jugados
        int minutos_partido = 90;

        // determino ganador o desempate
        if (goles_L_hoy > goles_V_hoy) {
            local->pg += 1; visitante->pp += 1;
            ganador = local;
        } else if (goles_V_hoy > goles_L_hoy) {
            visitante->pg += 1; local->pp += 1;
            ganador = visitante;
        } else {


            // empate y desempatamos mediante ranking fifa
            local->pe += 1; visitante->pe += 1;
            minutos_partido = 120;

            cout << "\n--- EMPATE EN TIEMPO REGLAMENTARIO. PRORROGA Y DESEMPATE POR RANKING ---" << endl;


            int pesoL = visitante->ranking > 0 ? visitante->ranking : 100;
            int pesoV = local->ranking > 0 ? local->ranking : 100;
            int total_peso = pesoL + pesoV;

            //  aleatorio sesgad tipo como dijo arlinton
            if ( (rand() % total_peso) < pesoL ) {
                ganador = local;
                goles_L_hoy += 1;
                cout << "-> " << local->pais << " anota y avanza por su peso en el Ranking FIFA!" << endl;


            } else {
                ganador = visitante;
                goles_V_hoy += 1;
                cout << "-> " << visitante->pais << " anota y avanza por su peso en el Ranking FIFA!" << endl;

            }
        }



        for(int i = 0; i < local->contador_jugadores; i++) {
            if(local->plantilla[i]) local->plantilla[i]->minutos_jugados += minutos_partido;
        }

        for(int i = 0; i < visitante->contador_jugadores; i++) {
            if(visitante->plantilla[i]) visitante->plantilla[i]->minutos_jugados += minutos_partido;
        }



        cout << "\nRESULTADO FINAL: " << local->pais << " " << goles_L_hoy << " - "
             << goles_V_hoy << " " << visitante->pais << endl;

        cout << "Goleadores " << local->pais << " (Dorsales): ";
        for(int i = 0; i < goles_L_hoy; i++) {
            if(local->contador_jugadores > 0) {
                cout << "[" << local->plantilla[rand() % local->contador_jugadores]->dorsal << "] ";
            }
        }
        if(goles_L_hoy == 0) cout << "Ninguno";

        cout << "\nGoleadores " << visitante->pais << " (Dorsales): ";
        for(int i = 0; i < goles_V_hoy; i++) {
            if(visitante->contador_jugadores > 0) {
                cout << "[" << visitante->plantilla[rand() % visitante->contador_jugadores]->dorsal << "] ";
            }
        }
        if(goles_V_hoy == 0) cout << "Ninguno";

        cout << "\n======================================================\n";
    }


};
#endif // SIMULACION_H
