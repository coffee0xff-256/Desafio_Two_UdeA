#include <iostream>
#include <clases.h>
#include <funciones_auxiliares.h>
#include <bombos.h>
using namespace std;

int main() {

    srand(time(0));
    equipo* torneo[48];
    int cantidad_equipos = 0;

    cout << "carga de datos"<<endl;
    cargar_datos(torneo, cantidad_equipos);
    cargar_confederaciones(torneo, cantidad_equipos);

    cout<< "guardando en los csv"<<endl;
    guardar_datos(torneo,cantidad_equipos);

    //Mate acá hago lo que viene siendo lo que se imprime en pantalla del requisito II del documento

    cout << "\nSorteo del mundial UdeAWorlcup\n" << endl;
    Sorteo sorteo;
    sorteo.conformarbombos(torneo, cantidad_equipos);
    sorteo.imprimirbombos();
    sorteo.realizarSorteo();
    sorteo.imprimirGrupos();

    for(int i = 0; i < cantidad_equipos; i++) {
        delete torneo[i];
    }


    return 0;
}
