#include <iostream>
#include <clases.h>
#include <funciones_auxiliares.h>
#include <bombos.h>
#include <fase_grupos.h>
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

    // Mate pille aca cargue las sedes y los arbitros directo del CSV.
    // Puse unos topes fijos pa no enredarnos y al final tira cuantos cargo pa confirmar.
    static const int MAX_SEDES    = 20;
    static const int MAX_ARBITROS = 64;
    string sedes[MAX_SEDES];
    string arbitros[MAX_ARBITROS];
    int total_sedes    = cargar_sedes(sedes, MAX_SEDES);
    int total_arbitros = cargar_arbitros(arbitros, MAX_ARBITROS);
    cout << "Sedes cargadas: "    << total_sedes    << endl;
    cout << "Arbitros cargados: " << total_arbitros << endl;

    // Aca aramamos los bombos, hacemos el sorteo y los grupos.
    cout << "Sorteo del mundial UdeAWorldCup" << endl;
    Sorteo sorteo;
    sorteo.conformarbombos(torneo, cantidad_equipos);
    sorteo.imprimirbombos();
    sorteo.realizarSorteo();
    sorteo.imprimirGrupos();

    // Pille, en esta parte meto el calendario de la fase de grupos
    // las sedes y los arbitros simulo todos los partidos
    cout << "\nFase de Grupos" << endl;
    fasegrupos fase;
    fase.generar_calendario(sorteo);
    fase.asignar_recursos(sedes, total_sedes, arbitros, total_arbitros);
    fase.simular_fase_grupos(sedes, total_sedes, arbitros, total_arbitros);
    fase.imprimir_partidos(sedes, total_sedes, arbitros, total_arbitros);

    // Acs cuadro la tabla general de clasificación, saco los que pasaron a dieciseisavos(R16)
    // y los recorro para mostrarlos.
    fase.construir_clasificacion();
    fase.imprimir_tablas();
    cout << "\nClasificados a R16 (" << fase.getTotalClasificados() << " equipos)\n";
    equipo** cls = fase.getClasificados();
    for (int i = 0; i < fase.getTotalClasificados(); i++)
        cout << (i + 1) << ". " << cls[i]->pais
             << " [" << cls[i]->confederacion << "]" << endl;

    //borramos los equipos de la memoria para no dejar basura.
    for (int i = 0; i < cantidad_equipos; i++)
        delete torneo[i];

    return 0;
}
