#include <iostream>
#include <fstream>
#include "bombos.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QDebug>


//Lectura de los archivos ingresados en .csv dado por .qrc del arbol dado.

void leerCSV(const QString& ruta, Equipo* equipos, int& total) {
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: no se pudo abrir el recurso de Qt: " << ruta.toStdString() ;
        total = 0;
        return;
    }

    total = 0;
    QTextStream in(&archivo);

    while (!in.atEnd() && total < 48) {
        QString linea = in.readLine();
        if (linea.isEmpty()) continue;

        QStringList partes = linea.split(',');

        if (partes.size() >= 3) {
            int ranking = partes[0].toInt();
            QString nombre = partes[1].trimmed();
            QString conf = partes[2].trimmed();

            QByteArray utf8Nombre = nombre.toUtf8();
            QByteArray utf8Conf = conf.toUtf8();

            equipos[total++] = Equipo(ranking,utf8Nombre.constData(),utf8Conf.constData());
        }
    }
    archivo.close();
}

//Inicio de la lectura de archivos .qrc y extraccion de los datos dados para la creación de los bombos.
//Restricción de datos para la creacion de los grupos dados desde la lectura de estas y consolidaciónd de las mismas.
int main() {
    std::cout << "Mundial UdeAWorld";
    const int maxequipos = 48;
    Equipo equipos[maxequipos];
    int total = 0;

    // Ruta de la .qrc ingresada para su lectura.
    leerCSV(":/new/prefix1/bombos.csv", equipos, total);

    if (total == 0) {
        std::cout << "No se cargaron equipos. Verifique la ruta del recurso (.qrc).\n";
        return 1;
    }

    Sorteo sorteo;
    sorteo.conformarbombos(equipos, total);
    sorteo.imprimirbombos();

    std::cout << "Realización del sorteo";
    sorteo.realizarSorteo();

    sorteo.imprimirGrupos();

    return 0;
}