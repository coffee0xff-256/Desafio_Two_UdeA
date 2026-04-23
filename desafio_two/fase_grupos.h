#ifndef FASE_GRUPOS_H
#define FASE_GRUPOS_H
#include <iostream>
#include <cstdlib>
#include "clases.h"
#include "bombos.h"

using namespace std;

//Definicion de la fila de tabla de clasificacion de los equipos dentro de su grupo
struct filadeclasificacion{
    equipo*eq;
    int pj, pg, pe, pp;
    int gf, gc;
    int puntos;

    filadeclasificacion():eq(nullptr),
        pj(0), pg(0), pe(0), pp(0), gf(0), gc(0), puntos(0){}
    int diferencia() const{return gf - gc;}
};

// Creación de la tabla de grupos separada po 4 filas.
class tabladegrupo{
public:
    static const int tamano_g = 4;
    filadeclasificacion filas[tamano_g];
    int n;
    char letra;
    tabladegrupo():n(0),letra('?'){}

    void inicio(Grupo* g, char l){
        letra =  l;
        n = g -> tamanio();
        for (int i = 0; i < n; i++){
            filas[i].eq = g->getEquipo(i);
        }
    }

    filadeclasificacion* buscar(equipo* e){
        for (int i = 0; i < n; i++)
            if(filas[i].eq == e) return &filas[i];
        return nullptr;
    }
};




#endif // FASE_GRUPOS_H
