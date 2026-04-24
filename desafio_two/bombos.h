#ifndef BOMBOS_H
#define BOMBOS_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <clases.h>
#include <fstream>

//Mate acá hago el nodo de la lista enlazada como en el code anterior
template<typename T>
class Nodo {
public:
    T dato;
    Nodo<T>* sig;

    Nodo(const T& d):dato(d), sig(nullptr) {}
};


//clase de la lista enlazada simple en este apartado
template<typename T>
class Lista {
    Nodo<T>* cabeza;
    int      tam;

public:
    Lista():cabeza(nullptr), tam(0) {}
    ~Lista(){ vaciar(); }

    Lista(const Lista&)            = delete;
    Lista& operator=(const Lista&) = delete;

    void agregar(const T& dato) {
        Nodo<T>* nuevo = new Nodo<T>(dato);
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            Nodo<T>* actual = cabeza;
            while (actual->sig != nullptr)
                actual = actual->sig;
            actual->sig = nuevo;
        }
        tam++;
    }

    T& operator[](int i) {
        Nodo<T>* actual = cabeza;
        for (int k = 0; k < i; k++)
            actual = actual->sig;
        return actual->dato;
    }

    const T& operator[](int i) const {
        Nodo<T>* actual = cabeza;
        for (int k = 0; k < i; k++)
            actual = actual->sig;
        return actual->dato;
    }

    void eliminar(int i) {
        if (i < 0 || i >= tam) return;
        Nodo<T>* aEliminar;
        if (i == 0) {
            aEliminar = cabeza;
            cabeza    = cabeza->sig;
        } else {
            Nodo<T>* anterior = cabeza;
            for (int k = 0; k < i - 1; k++)
                anterior = anterior->sig;
            aEliminar     = anterior->sig;
            anterior->sig = aEliminar->sig;
        }
        delete aEliminar;
        tam--;
    }

// Bro funcion para barajar los elementos de manera random con iteraciones desde i hasta j, code fisher-yates
    void mezclar() {
        if (tam <= 1) return;
        int tamano = tam;
        T* aux = new T[tamano];
        for (int i = 0; i < tamano; i++)
            aux[i] = (*this)[i];

        for (int i = tamano - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            T temp = aux[i];
            aux[i]= aux[j];
            aux[j]= temp;
        }
        vaciar();
        for (int i = 0; i < tamano; i++)
            agregar(aux[i]);
        delete[] aux;
    }
//Eliminacion de nodos que libera memoria.
    void vaciar() {
        while (cabeza != nullptr) {
            Nodo<T>* temp = cabeza;
            cabeza = cabeza->sig;
            delete temp;
        }
        tam = 0;
    }

    int  tamano() const {return tam;}
    bool vacia() const {return tam == 0;}
};


// clase Bombo lo puse con el puntero equipo* hecho en clases.h
// La lista guarda punteros NO hace copia de los equipos.


class Bombo {
    int numero;
    Lista<equipo*> equipos;

public:
    Bombo(int num) : numero(num) {}
    Bombo(const Bombo&)= delete;
    Bombo& operator=(const Bombo&) = delete;

    void agregar(equipo* e) { equipos.agregar(e);}
    void mezclar()          { equipos.mezclar();}

    int getNumero() const{ return numero;}
    int tamano() const{ return equipos.tamano(); }
    equipo* operator[](int i)   { return equipos[i];}

    void imprimir() const {
        cout << "\nBombo " << numero << " (" << equipos.tamano() << " equipos)\n";
        for (int i = 0; i < equipos.tamano(); i++) {
            cout << (i + 1) << ". ["
                 << equipos[i]->ranking << "] "
                 << equipos[i]->pais    << " ("
                 << equipos[i]->confederacion << ")\n";
        }
    }
};

// Creacion de bombos con las restricciones del Doc y hace que todo quede como se requiere.
class Grupo {
    char letra;
    Lista<equipo*> equipos;

    int contarDeConf(const string& conf) const {
        int cuenta = 0;
        for (int i = 0; i < equipos.tamano(); i++)
            if (equipos[i]->confederacion == conf)
                cuenta++;
        return cuenta;
    }

public:
    Grupo(char l) : letra(l) {}
    Grupo(const Grupo&)            = delete;
    Grupo& operator=(const Grupo&) = delete;

    char getLetra() const { return letra;            }
    int  tamanio()  const { return equipos.tamano(); }
    void vaciar() { equipos.vaciar();}
    //Creacion del getEquipo
    equipo* getEquipo(int i) const{return equipos[i];}

    bool puedeAgregar(equipo* e) const {
        int yaHay = contarDeConf(e->confederacion);
        if (yaHay == 0) return true;
        if (e->confederacion == "UEFA" && yaHay < 2) return true;
        return false;
    }

    bool agregar(equipo* e) {
        if (!puedeAgregar(e)) return false;
        equipos.agregar(e);
        return true;
    }

    void imprimir() const {
        cout << "\nGRUPO\n " << letra << ":\n";
        for (int i = 0; i < equipos.tamano(); i++) {
            cout << "  " << (i + 1) << ". ["
                 << equipos[i]->ranking << "] "
                 << equipos[i]->pais    << " ("
                 << equipos[i]->confederacion << ")\n";
        }
    }
};

// Creacion de la clase sorteo igual que las anteriores y las une de manera que haga la mezcla de los bombos
//crea los 12 grupos repartidos con sus respectivas funciones.
class Sorteo {
    static const int total_bombos = 4;
    static const int total_grupos = 12;

    Bombo* bombos[total_bombos];
    Grupo* grupos[total_grupos];

    bool intentarSorteo() {
        for (int i = 0; i < total_grupos; i++)
            grupos[i]->vaciar();
        Lista<equipo*> copia;
        for (int j = 0; j < bombos[0]->tamano(); j++)
            copia.agregar((*bombos[0])[j]);
        copia.mezclar();

        for (int j = 0; j < copia.tamano(); j++) {
            if (copia[j]->pais == "United States") {
                grupos[0]->agregar(copia[j]);
                copia.eliminar(j);
                break;
            }
        }
        for (int g = 1; g < total_grupos && !copia.vacia(); g++) {
            grupos[g]->agregar(copia[0]);
            copia.eliminar(0);
        }
        for (int b = 1; b < total_bombos; b++) {
            Lista<equipo*> copiaBombo;
            for (int j = 0; j < bombos[b]->tamano(); j++)
                copiaBombo.agregar((*bombos[b])[j]);
            copiaBombo.mezclar();

            int cuantos = copiaBombo.tamano();
            for (int j = 0; j < cuantos; j++) {
                equipo* e = copiaBombo[j];

                Lista<int> validos;
                for (int g = 0; g < total_grupos; g++) {
                    if (grupos[g]->tamanio() == b && grupos[g]->puedeAgregar(e))
                        validos.agregar(g);
                }

                if (validos.vacia()) return false;

                int elegido = rand() % validos.tamano();
                grupos[validos[elegido]]->agregar(e);
            }
        }
        return true;
    }

public:
    Sorteo() {
        for (int i = 0; i < total_bombos; i++) bombos[i] = new Bombo(i + 1);
        for (int i = 0; i < total_grupos;  i++) grupos[i] = new Grupo('A' + i);
    }

    ~Sorteo() {
        for (int i = 0; i < total_bombos; i++) delete bombos[i];
        for (int i = 0; i < total_grupos;  i++) delete grupos[i];
    }

    Sorteo(const Sorteo&)= delete;
    Sorteo& operator=(const Sorteo&) = delete;

    void conformarbombos(equipo* torneo[], int total) {
        equipo*  usa    = nullptr;
        equipo** resto  = new equipo*[total - 1];
        int      nResto = 0;

        for (int i = 0; i < total; i++) {
            if (torneo[i]->pais == "United States")
                usa = torneo[i];
            else
                resto[nResto++] = torneo[i];
        }

        for (int i = 1; i < nResto; i++) {
            equipo* clave = resto[i];
            int     j     = i - 1;
            while (j >= 0 && resto[j]->ranking > clave->ranking) {
                resto[j + 1] = resto[j];
                j--;
            }
            resto[j + 1] = clave;
        }

        if (usa) bombos[0]->agregar(usa);
        for (int i =  0; i < 11; i++) bombos[0]->agregar(resto[i]);
        for (int i = 11; i < 23; i++) bombos[1]->agregar(resto[i]);
        for (int i = 23; i < 35; i++) bombos[2]->agregar(resto[i]);
        for (int i = 35; i < 47; i++) bombos[3]->agregar(resto[i]);

        delete[] resto;
    }

    void realizarSorteo() {
        int intentos = 0;
        //emma cambio aquí el srand, debido a que es mejor utilizarlo y llamarlo una sola vez en el main

        while (!intentarSorteo()) {
            if (++intentos > 10000) {
                cout << "ERROR: no se encontro solucion";
                return;
            }
        }
        if (intentos == 0)
            cout << "\nSorteo completado en la primera iteracion\n";
        else
            cout << "\nSorteo completado (" << intentos << " reintentos) \n";

        guardarSorteo();
    }

    void imprimirbombos() const {
        cout <<"\nSorteo de los bombos\n";
        for (int i = 0; i < total_bombos; i++)
            bombos[i]->imprimir();
    }
    //Funcón para guardar los bombos en cada iteracion en un archivo .csv
    void guardarSorteo() const {
        ofstream archivo("sorteo_grupos.csv");
        archivo << "Grupo,Posicion,Pais,Confederacion,Ranking\n";
        for (int i = 0; i < total_grupos; i++) {
            Grupo* g = grupos[i];
            for (int j = 0; j < g->tamanio(); j++) {
                archivo << g->getLetra() << ","
                        << (j + 1) << ","
                        << g->getEquipo(j)->pais << ","
                        << g->getEquipo(j)->confederacion << ","
                        << g->getEquipo(j)->ranking << "\n";
            }
        }
        archivo.close();
        cout << "\nSorteo guardado en sorteo_grupos.csv\n";
    }

    void imprimirGrupos() const {
        cout << "Grupos";
        for (int i = 0; i < total_grupos; i++)
            grupos[i]->imprimir();
        cout << "\n";
    }

    Bombo* getBombo(int i) { return bombos[i]; }
    Grupo* getGrupo(int i) { return grupos[i]; }
};

#endif // BOMBOS_H