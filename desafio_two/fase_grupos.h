#ifndef FASE_GRUPOS_H
#define FASE_GRUPOS_H

#include <iostream>
#include "clases.h"
#include "bombos.h"

using namespace std;

class filadeclasificacion {
public:
    equipo* eq;
    int pj, pg, pe, pp, gf, gc, puntos;

    filadeclasificacion() : eq(nullptr), pj(0), pg(0), pe(0), pp(0), gf(0), gc(0), puntos(0) {}
    int diferencia() const { return gf - gc; }
};

class tabladegrupo {
public:
    filadeclasificacion filas[4];
    int n;
    char letra;

    tabladegrupo() : n(0), letra('?') {}

    // Función manual para alinear texto sin cstdio ni iomanip
    void imprimirEspacios(string texto, int ancho) {
        cout << texto;
        for (int i = 0; i < ancho - (int)texto.length(); i++) cout << " ";
    }

    void imprimirEspaciosInt(int valor, int ancho) {
        string s = to_string(valor);
        cout << s;
        for (int i = 0; i < ancho - (int)s.length(); i++) cout << " ";
    }

    void inicializar(Grupo* g, char l) {
        letra = l;
        n = (g->tamanio() > 4) ? 4 : g->tamanio();
        for (int i = 0; i < n; i++) {
            filas[i].eq = g->getEquipo(i);
        }
    }

    void registrar_resultado(equipo* e1, int g1, equipo* e2, int g2) {
        for (int i = 0; i < n; i++) {
            if (filas[i].eq == e1) {
                filas[i].pj++; filas[i].gf += g1; filas[i].gc += g2;
                if (g1 > g2) { filas[i].pg++; filas[i].puntos += 3; }
                else if (g1 == g2) { filas[i].pe++; filas[i].puntos += 1; }
                else filas[i].pp++;
            }
            if (filas[i].eq == e2) {
                filas[i].pj++; filas[i].gf += g2; filas[i].gc += g1;
                if (g2 > g1) { filas[i].pg++; filas[i].puntos += 3; }
                else if (g2 == g1) { filas[i].pe++; filas[i].puntos += 1; }
                else filas[i].pp++;
            }
        }
    }

    void ordenar() {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                bool swap = false;
                if (filas[j].puntos < filas[j+1].puntos) swap = true;
                else if (filas[j].puntos == filas[j+1].puntos) {
                    if (filas[j].diferencia() < filas[j+1].diferencia()) swap = true;
                }
                if (swap) {
                    filadeclasificacion temp = filas[j];
                    filas[j] = filas[j+1];
                    filas[j+1] = temp;
                }
            }
        }
    }

    void imprimirTablaCompleta() {
        cout << "\nGRUPO " << letra << "\n";
        imprimirEspacios("EQUIPO", 15); cout << "| PJ | PG | PE | PP | GF | GC | DG | PTS\n";
        cout << "----------------------------------------------------------\n";
        for (int i = 0; i < n; i++) {
            imprimirEspacios(filas[i].eq->pais, 15);
            cout << "| "; imprimirEspaciosInt(filas[i].pj, 3);
            cout << "| "; imprimirEspaciosInt(filas[i].pg, 3);
            cout << "| "; imprimirEspaciosInt(filas[i].pe, 3);
            cout << "| "; imprimirEspaciosInt(filas[i].pp, 3);
            cout << "| "; imprimirEspaciosInt(filas[i].gf, 3);
            cout << "| "; imprimirEspaciosInt(filas[i].gc, 3);
            cout << "| "; imprimirEspaciosInt(filas[i].diferencia(), 3);
            cout << "| " << filas[i].puntos << "\n";
        }
        imprimirGoleador();
    }

    void imprimirGoleador() {
        jugador* top = nullptr;
        int maxG = -1;
        for(int i=0; i<n; i++) {
            for(int j=0; j < filas[i].eq->contador_jugadores; j++) {
                if(filas[i].eq->plantilla[j]->goles > maxG) {
                    maxG = filas[i].eq->plantilla[j]->goles;
                    top = filas[i].eq->plantilla[j];
                }
            }
        }
        if(top && maxG > 0)
            cout << "GOLEADOR GRUPO: " << top->nombre << " (" << maxG << " goles)\n";
    }
};
#endif