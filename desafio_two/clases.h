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
};

    class equipo{
    public:
        string pais;
        string dt;
        //Mate agrego este string para poder hacer el sorteo

        // el string confederacion unicamente es para el header bombos.h
        //pensamos en modificar el formato csv, pero es inncesario ya que unicamente se utiliza para los bombos

        string confederacion;
        int ranking;
        int goles_favor;
        //ema aqui pongo las otras variables que me faltan
        int ga;
        int gc;
        int pg;
        int pe;
        int pp;
        int t_a;
        int t_r;
        int faltas;
        //ema aqui viene la cosa, por cada equipo tenemos 26 jugadores así que lo que haremos será un arreglo dinamico para
        // esos jugadores que son 26

        jugador* plantilla[26];
        int contador_jugadores;

        //constructor otra vez

        //aqui ema aunque inialice todo en 0 , cuando leemos los csv los arreglamos;

        equipo(){
            ranking = 0;
            goles_favor=0;
            contador_jugadores=0;
            pais="";
            dt = "";
            ga = 0;
            gc = 0;
            pg = 0;
            pe = 0;
            pp = 0;
            t_a = 0;
            t_r = 0;
            faltas = 0;

            for(int i = 0; i < 26; i++){
                plantilla[i] = nullptr;

            }
        }

        //destructor emma importante

        ~equipo(){
            for(int i = 0; i < contador_jugadores ; i++){
                if(plantilla[i] != nullptr){
                    delete plantilla[i];
                }
            }
        }


        void repartir_goles(){
            if(contador_jugadores == 0){return;}
            int goles_base = goles_favor / contador_jugadores;
            int goles_sobrantes = goles_favor % contador_jugadores;

            for(int i = 0; i < contador_jugadores;i++){
                plantilla[i]->goles = goles_base;

                if(i < goles_sobrantes){
                    plantilla[i]->goles += 1;
                }
            }
        }

    };




#endif // CLASES_H
