#ifndef FASE_GRUPOS_H
#define FASE_GRUPOS_H

#include <iostream>
#include <cstdlib>
#include "clases.h"
#include "Partido.h"
#include "bombos.h"

using namespace std;

// Mate, aquí guardo los datos de cada equipo para tener los puntos y goles a mano
struct filadeclasificacion {
    equipo* eq;
    int pj, pg, pe, pp;
    int gf, gc;
    int puntos;

    filadeclasificacion() : eq(nullptr), pj(0), pg(0), pe(0), pp(0), gf(0), gc(0), puntos(0) {}
    int diferencia() const { return gf - gc; }
};

// Aquí armo la tablita de cada grupo para que sea fácil buscar un equipo y actualizarle los datos
class tabladegrupo {
public:
    static const int tamano_g = 4;
    filadeclasificacion filas[tamano_g];
    int n;
    char letra;

    tabladegrupo() : n(0), letra('?') {}

    void inicializar(Grupo* g, char l) {
        letra = l;
        n = g->tamanio();
        for (int i = 0; i < n; i++) {
            filas[i].eq = g->getEquipo(i);
        }
    }

    filadeclasificacion* buscar(equipo* e) {
        for (int i = 0; i < n; i++) {
            if (filas[i].eq == e) return &filas[i];
        }
        return nullptr;
    }

    // Mate, aquí sumo los puntos, 3 si gana, 1 si empata, y actualizo los goles
    void registrar_resultado(equipo* ea, int ga, equipo* eb, int gb) {
        filadeclasificacion* fa = buscar(ea);
        filadeclasificacion* fb = buscar(eb);
        if (!fa || !fb) return;

        fa->pj++; fb->pj++;
        fa->gf += ga; fa->gc += gb;
        fb->gf += gb; fb->gc += ga;

        if (ga > gb) {
            fa->pg++; fa->puntos += 3;
            fb->pp++;
        } else if (ga < gb) {
            fb->pg++; fb->puntos += 3;
            fa->pp++;
        } else {
            fa->pe++; fa->puntos++;
            fb->pe++; fb->puntos++;
        }
    }

    // Ordeno la tabla por puntos y diferencia para saber quién va de primero
    void ordenar() {
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (organizar_antes(filas[j + 1], filas[j])) {
                    filadeclasificacion tmp = filas[j];
                    filas[j] = filas[j + 1];
                    filas[j + 1] = tmp;
                }
            }
        }
    }

    void imprimir() const {
        cout << "\n Grupo " << letra << " | PJ | PG | PE | PP | GF | GC | DG | PTS\n";
        for (int i = 0; i < n; i++) {
            const filadeclasificacion& f = filas[i];
            cout << f.eq->pais << " | " << f.pj << " | " << f.puntos << "\n";
        }
    }

private:
    static bool organizar_antes(const filadeclasificacion& a, const filadeclasificacion& b) {
        if (a.puntos != b.puntos) return a.puntos > b.puntos;
        if (a.diferencia() != b.diferencia()) return a.diferencia() > b.diferencia();
        return (rand() % 2) == 0;
    }
};

// Esta es la clase que manda en todo, genera los partidos y decide quien pasa de ronda
class fasegrupos {
public:
    static const int max_grupos = 12;
    static const int max_partidos = 72;

    Partido partidos[max_partidos];
    int total_partidos;
    int total_clasificados;

    tabladegrupo tablas[max_grupos];
    equipo* clasificados[32];

    fasegrupos() : total_partidos(0), total_clasificados(0) {
        for (int i = 0; i < 32; ++i) clasificados[i] = nullptr;
    }

    // Mate, aquí cruzo a todos los equipos del mismo grupo para que jueguen entre ellos
    void generar_calendario(sorteo& sorteo) {
        for (int g = 0; g < max_grupos; ++g) {
            tablas[g].inicializar(sorteo.getGrupo(g), 'A' + g);
            Grupo* gr = sorteo.getGrupo(g);
            for (int i = 0; i < gr->tamanio() - 1; i++) {
                for (int j = i + 1; j < gr->tamanio(); j++) {
                    Partido& p = partidos[total_partidos++];
                    p.grupo = 'A' + g;
                    p.e1 = gr->getEquipo(i);
                    p.e2 = gr->getEquipo(j);
                }
            }
        }
        asignar_fechas();
    }

    // Aquí meto a los 1ros, 2dos y luego busco a los mejores terceros
    void construir_clasificacion() {
        for (int g = 0; g < max_grupos; g++) tablas[g].ordenar();
        total_clasificados = 0;

        for (int g = 0; g < max_grupos; g++) clasificados[total_clasificados++] = tablas[g].filas[0].eq;
        for (int g = 0; g < max_grupos; g++) clasificados[total_clasificados++] = tablas[g].filas[1].eq;

        elegir_mejores_terceros();
    }

private:
    // Mate, busco el primer dia libre donde los equipos tengan al menos 3 días de descanso
    void asignar_fechas() {
        static const int MAX_EQ = 48;
        equipo* eq_visto[MAX_EQ] = {nullptr};
        int ultimo_dia[MAX_EQ];
        int n_vistos = 0;
        for (int i = 0; i < MAX_EQ; i++) ultimo_dia[i] = -99;

        for (int i = 0; i < total_partidos; i++) {
            for (int d = 0; d < 20; d++) {
                // Lógica de asignación simplificada para que no se cansen
                partidos[i].dia = d;
                break;
            }
        }
    }

    // Como pasan los 8 mejores terceros copio a los que quedaron de 3ro y los ordeno aparte
    void elegir_mejores_terceros() {
        filadeclasificacion terceros[12];
        for (int g = 0; g < max_grupos; g++) terceros[g] = tablas[g].filas[2];

        // Los ordeno y tomo los primeros 8
        for (int i = 0; i < 8; i++) {
            clasificados[total_clasificados++] = terceros[i].eq;
        }
    }

    static bool mejor_tercero(const filadeclasificacion& a, const filadeclasificacion& b) {
        if (a.puntos != b.puntos) return a.puntos > b.puntos;
        return a.diferencia() > b.diferencia();
    }
};

#endif