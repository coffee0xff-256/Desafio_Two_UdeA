#ifndef CLASES_H
#define CLASES_H
#include <string> // lo utilizare para facil manejo de los archivoc csv
using namespace std;

// emma aqui pondre las clases para mayor estructuramiento

class jugador {
public:
    //siguiendo la logica de nuestro csv que tiene la forma de:
    // nombre,apellido,dorsal,pj,goles,minutos,asistencias,t.amarillas,t.rojas,faltas
    string nombre;
    string apellido;
    int dorsal;
    int  partidos_jugados;
    int goles;
    int minutos_jugados;
    int asistencias;
    int t_amarillas;
    int t_rojas;
    int faltas;

    //emma inciamos el constructos como dice el documento todo a 0 excepto algunas cosas

    jugador(){
        dorsal = 0;
        partidos_jugados = 0;
        goles = 0;
        minutos_jugados = 0;
        asistencias = 0;
        t_amarillas = 0;
        t_rojas  = 0;
        faltas = 0;
        nombre = "";
        apellido = "";
    }








}
#endif // CLASES_H
