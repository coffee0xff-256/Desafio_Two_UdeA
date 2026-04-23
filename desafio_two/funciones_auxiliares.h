#ifndef FUNCIONES_AUXILIARES_H
#define FUNCIONES_AUXILIARES_H
#include <string>
#include <clases.h>
#include <iostream>
#include <fstream>
using namespace std;

string extraer_palabra(string& linea) {
    string palabra = "";
    int i = 0;
    int longitud = linea.length();

    while (i < longitud && linea[i] != ',') {
        palabra += linea[i];
        i++;
    }


    if (i < longitud) {
        linea.erase(0, i + 1);
    } else {
        linea = "";
    }

    return palabra;
}
void cargar_datos(equipo* torneo[], int& cantidad_equipos) {
    string linea;
    string campo_temp;


    ifstream archivo_equipos("equipos.csv");

    getline(archivo_equipos, linea);

    while (getline(archivo_equipos, linea)) {
        if (linea.empty()) continue;

        equipo* nuevo_equipo = new equipo();


        nuevo_equipo->pais = extraer_palabra(linea);

        if (nuevo_equipo->pais.empty() || nuevo_equipo->pais == "\r") {
            delete nuevo_equipo;
            continue;
        }

        nuevo_equipo->dt = extraer_palabra(linea);

        campo_temp = extraer_palabra(linea);
        if(!campo_temp.empty()) nuevo_equipo->ranking = stoi(campo_temp);

        campo_temp = extraer_palabra(linea);
        if(!campo_temp.empty()) nuevo_equipo->goles_favor = stoi(campo_temp);
        //Nuevo campo para la confederación.
        nuevo_equipo->confederacion = extraer_palabra(linea);

        torneo[cantidad_equipos] = nuevo_equipo;
        cantidad_equipos++;
    }
    archivo_equipos.close();

    ifstream archivo_jugadores("jugadores.csv");

    getline(archivo_jugadores, linea);

    while (getline(archivo_jugadores, linea)) {
        if (linea.empty()) continue;


        string nombre_equipo_temp = extraer_palabra(linea);

        equipo* equipo_actual = nullptr;
        for (int i = 0; i < cantidad_equipos; i++) {
            if (torneo[i]->pais == nombre_equipo_temp) {
                equipo_actual = torneo[i];
                break;
            }
        }

        if (equipo_actual != nullptr && equipo_actual->contador_jugadores < 26) {
            jugador* nuevo_jugador = new jugador();


            nuevo_jugador->nombre = extraer_palabra(linea);
            nuevo_jugador->apellido = extraer_palabra(linea);


            campo_temp = extraer_palabra(linea);
            if(!campo_temp.empty()) nuevo_jugador->dorsal = stoi(campo_temp);


            equipo_actual->plantilla[equipo_actual->contador_jugadores] = nuevo_jugador;
            equipo_actual->contador_jugadores++;
        }
    }
    archivo_jugadores.close();

    for (int i = 0; i < cantidad_equipos; i++) {
        torneo[i]->repartir_goles();
    }
}

// Funcion para cargar datos de los bombos dados en el .csv

void cargar_confederaciones(equipo* torneo[], int cantidad_equipos) {
    string linea;
    string campo_temp;

    ifstream archivo_bombos("bombos.csv");

    while (getline(archivo_bombos, linea)) {
        if (linea.empty()) continue;
        if (!linea.empty() && (unsigned char)linea[0] == 0xEF)
            linea.erase(0, 3);
        campo_temp= extraer_palabra(linea);
        string nombre = extraer_palabra(linea);
        string confederacion= extraer_palabra(linea);

        if (!confederacion.empty() && confederacion.back() == '\r')
            confederacion.pop_back();

        for (int i = 0; i < cantidad_equipos; i++) {
            if (torneo[i]->pais == nombre) {
                torneo[i]->confederacion = confederacion;
                break;
            }
        }
    }
    archivo_bombos.close();
}
void guardar_datos(equipo* torneo[], int cantidad_equipos) {

    ofstream archivo_salida("jugadores.csv");


    //ema aqui lo que hice fue reescribir casi el mismo csv casi hahaha
    archivo_salida << "Equipo,nombre,apellido,dorsal,PJ,goles,minutos j,asistencias,T.Amarillas,T.rojas,Faltas\n";


    for (int i = 0; i < cantidad_equipos; i++) {
        for (int j = 0; j < torneo[i]->contador_jugadores; j++) {
            jugador* p = torneo[i]->plantilla[j];

            archivo_salida << torneo[i]->pais << ","
                           << p->nombre << ","
                           << p->apellido << ","
                           << p->dorsal << ","
                           << p->partidos_jugados << ","
                           << p->goles << ","
                           << p->minutos_jugados << ","
                           << p->asistencias << ","
                           << p->t_amarillas << ","
                           << p->t_rojas << ","
                           << p->faltas << "\n";
        }
    }

    archivo_salida.close();
}

#endif // FUNCIONES_AUXILIARES_H
