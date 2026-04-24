#ifndef FASE_GRUPOS_H
#define FASE_GRUPOS_H

#include <iostream>
#include <cstdlib>
#include <clases.h>
#include <Partido.h>
#include <bombos.h>

using namespace std;

//Definicion de la fila de tabla de clasificacion de los equipos dentro de su grupo

//Emma Aquí cambio de struct a class por requisitos del desafio
class filadeclasificacion {
public:
    equipo* eq;
    int pj, pg, pe, pp;
    int gf, gc;
    int puntos;
    char grupos;

    filadeclasificacion() : eq(nullptr),
        pj(0), pg(0), pe(0), pp(0), gf(0), gc(0), puntos(0) {}

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
                if (mejor(filas[j + 1], filas[j])) {
                    filadeclasificacion tmp = filas[j];
                    filas[j] = filas[j + 1];
                    filas[j + 1] = tmp;
                }
            }
        }
    }

    void imprimir() const {
        cout << "\n  Grupo " << letra
             << " | PJ | PG | PE | PP | GF | GC | DG | PTS\n";
        for (int i = 0; i < n; i++) {
            const filadeclasificacion& f = filas[i];
            cout << "  " << f.eq->pais
                 << " | " << f.pj
                 << " | " << f.pg
                 << " | " << f.pe
                 << " | " << f.pp
                 << " | " << f.gf
                 << " | " << f.gc
                 << " | " << f.diferencia()
                 << " | " << f.puntos << "\n";
        }
    }

private:
    static bool mejor(const filadeclasificacion& a, const filadeclasificacion& b) {
        if (a.puntos != b.puntos) return a.puntos > b.puntos;
        if (a.diferencia() != b.diferencia()) return a.diferencia() > b.diferencia();
        if (a.gf !=b.gf) return a.gf > b.gf;
        return (rand() % 2) == 0;
    }
};

// Esta es la clase que manda en todo, genera los partidos y decide quien pasa de ronda
class fasegrupos {
public:
    static const int max_grupos = 12;
    static const int max_partidos = 72;

    partido partidos[max_partidos];
    int total_partidos;
    int total_clasificados;

    tabladegrupo tablas[max_grupos];
    equipo* clasificados[32];

    fasegrupos() : total_partidos(0), total_clasificados(0) {
        for (int i = 0; i < 32; ++i) clasificados[i] = nullptr;
    }

    // Mate, aquí cruzo a todos los equipos del mismo grupo para que jueguen entre ellos
    void generar_calendario(Sorteo& sorteo) {
        for (int g = 0; g < max_grupos; ++g) {
            tablas[g].inicializar(sorteo.getGrupo(g), 'A' + g);
            Grupo* gr = sorteo.getGrupo(g);
            for (int i = 0; i < gr->tamanio() - 1; i++) {
                for (int j = i + 1; j < gr->tamanio(); j++) {
                    partido& p = partidos[total_partidos++];
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
        equipo* eq_reg[MAX_EQ] = {nullptr};
        int     ultimo[MAX_EQ];
        int     n_reg = 0;
        for (int i = 0; i < MAX_EQ; i++) ultimo[i] = -99;

        int por_dia[17] = {0};

        for (int i = 0; i < total_partidos; i++) {
            partido& p = partidos[i];
            for (int dia = 0; dia <= 16; dia++) {
                if (por_dia[dia] >= 4) continue; // Máximo 4 partidos al día

                // Buscar último día jugado de cada equipo
                int ud1 = -99, ud2 = -99;
                for (int k = 0; k < n_reg; k++) {
                    if (eq_reg[k] == p.e1) ud1 = ultimo[k];
                    if (eq_reg[k] == p.e2) ud2 = ultimo[k];
                }
                // Exigir al menos 3 días de descanso
                if (dia - ud1 < 3 || dia - ud2 < 3) continue;

                p.dia = dia;
                por_dia[dia]++;
                // ... (código para actualizar el último día de descanso) ...
                break;
            }
        }
    }

    // Como pasan los 8 mejores terceros copio a los que quedaron de 3ro y los ordeno aparte
    void elegir_mejores_terceros() {
        filadeclasificacion terceros[12];
        for (int g = 0; g < max_grupos; g++)
            terceros[g] = tablas[g].filas[2];

        // Ordenar por puntos > DG > GF
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 11 - i; j++) {
                bool swap = false;
                if (terceros[j+1].puntos > terceros[j].puntos) swap = true;
                else if (terceros[j+1].puntos == terceros[j].puntos &&
                         terceros[j+1].diferencia() > terceros[j].diferencia()) swap = true;
                else if (terceros[j+1].puntos == terceros[j].puntos &&
                         terceros[j+1].diferencia() == terceros[j].diferencia() &&
                         terceros[j+1].gf > terceros[j].gf) swap = true;

                if (swap) {
                    filadeclasificacion tmp = terceros[j];
                    terceros[j]   = terceros[j+1];
                    terceros[j+1] = tmp;
                }
            }
        }
        for (int i = 0; i < 8; i++)
            clasificados[total_clasificados++] = terceros[i].eq;
    }

    static bool mejor_tercero(const filadeclasificacion& a, const filadeclasificacion& b) {
        if (a.puntos != b.puntos) return a.puntos > b.puntos;
        return a.diferencia() > b.diferencia();
    }

public:
    // Asigna sede y árbitros a todos los partidos ya generados
    void asignar_recursos(const string sedes[], int total_sedes,
                          const string arbitros[], int total_arbitros) {
        for (int i = 0; i < total_partidos; i++)
            partidos[i].asignar_recursos(total_sedes, total_arbitros);
    }

    // Simula todos los partidos de grupos y actualiza la tabla de clasificación
    void simular_fase_grupos(const string sedes[], int total_sedes,
                             const string arbitros[], int total_arbitros) {
        for (int i = 0; i < total_partidos; i++) {
            partido& p = partidos[i];
            p.simular(false, total_sedes, total_arbitros);
            for (int g = 0; g < max_grupos; g++) {
                if (tablas[g].letra == p.grupo) {
                    tablas[g].registrar_resultado(p.e1, p.goles_e1, p.e2, p.goles_e2);
                    break;
                }
            }
        }
    }

    // Imprime todos los partidos de grupos con sus resultados
    void imprimir_partidos(const string sedes[], int total_sedes,
                           const string arbitros[], int total_arbitros) const {
        cout << "\n RESULTADOS FASE DE GRUPOS\n";
        for (int i = 0; i < total_partidos; i++)
            partidos[i].imprimir(sedes, total_sedes, arbitros, total_arbitros);
    }

    // Imprime todas las tablas de clasificación
    void imprimir_tablas() const {
        cout << "\nTABLAS DE CLASIFICACION\n";
        for (int g = 0; g < max_grupos; g++)
            tablas[g].imprimir();
    }

    equipo** getClasificados() { return clasificados; }
    int getTotalClasificados() const { return total_clasificados; }
};

#endif