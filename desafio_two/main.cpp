#include <iostream>
#include <clases.h>
#include <funciones_auxiliares.h>
using namespace std;

int main() {
    equipo* torneo[48];
    int cantidad_equipos = 0;

    cout << "carga de datos"<<endl;
    cargar_datos(torneo, cantidad_equipos);

    cout<< "guardando en los csv"<<endl;
    guardar_datos(torneo,cantidad_equipos);

    for(int i = 0; i < cantidad_equipos; i++) {
        delete torneo[i];
    }

    return 0;
}
