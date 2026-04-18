#ifndef BOMBOS_H
#define BOMBOS_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>

//Creación de Nodo generico basico para implementar las listas Enlazadas.

template<typename T>
class Nodo {
public:
    T        dato;
    Nodo<T>* sig;

    Nodo(const T& d) : dato(d), sig(nullptr) {}
};

// Creación de la lista T, generica para la implemntación siguiente.

template<typename T>
class Lista {
    Nodo<T>* cabeza;
    int      tam;

public:
    Lista()  : cabeza(nullptr), tam(0) {}
    ~Lista() { vaciar(); }

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
            aEliminar      = anterior->sig;
            anterior->sig  = aEliminar->sig;
        }
        delete aEliminar;
        tam--;
    }

    void mezclar() {
        if (tam <= 1) return;
        int tamano = tam;
        T* aux = new T[tamano];
        for (int i = 0; i < tamano; i++)
            aux[i] = (*this)[i];

        for (int i = tamano - 1; i > 0; i--) {
            int j    = rand() % (i + 1);
            T   temp = aux[i];
            aux[i]   = aux[j];
            aux[j]   = temp;
        }
        vaciar();
        for (int i = 0; i < tamano; i++)
            agregar(aux[i]);
        delete[] aux;
    }

    void vaciar() {
        while (cabeza != nullptr) {
            Nodo<T>* temp = cabeza;
            cabeza = cabeza->sig;
            delete temp;
        }
        tam = 0;
    }

    int tamano() const { return tam;}
    bool vacia()   const { return tam == 0; }
};

// Def de clase equipo para la definicion de su Ranking, Nombre y confederación segun los datos dados.

class Equipo {
    int  ranking;
    char nombre[60];
    char confederacion[20];

public:
    Equipo() : ranking(0) {
        nombre[0]        = '\0';
        confederacion[0] = '\0';
    }

    Equipo(int r, const char* nom, const char* conf) : ranking(r) {
        strncpy(this->nombre, nom, 59);
        this->nombre[59] = '\0';
        strncpy(this->confederacion, conf, 19);
        this->confederacion[19] = '\0';
    }

    Equipo(const Equipo& o) : ranking(o.ranking) {
        strncpy(nombre,        o.nombre,        59); nombre[59]        = '\0';
        strncpy(confederacion, o.confederacion, 19); confederacion[19] = '\0';
    }

    Equipo& operator=(const Equipo& o) {
        if (this != &o) {
            ranking = o.ranking;
            strncpy(nombre,        o.nombre,59); nombre[59]        = '\0';
            strncpy(confederacion, o.confederacion,19); confederacion[19] = '\0';
        }
        return *this;
    }

    bool operator==(const Equipo& o) const {
        return strcmp(nombre, o.nombre) == 0;
    }

    int getRanking() const { return ranking;       }
    const char* getNombre()const { return nombre;        }
    const char* getConfederacion() const { return confederacion; }

    void imprimir() const {
        std::cout << "[" << ranking << "] "<< nombre << " (" << confederacion << ")\n";
    }
};

//Clase de la definición parcial de los bombos para la separacion de selecciones dadas su posicion en esta.

class Bombo {
    int numero;
    Lista<Equipo> equipos;

public:
    Bombo(int num) : numero(num) {}
    Bombo(const Bombo&)= delete;
    Bombo& operator=(const Bombo&) = delete;

    void agregar(const Equipo& e) { equipos.agregar(e);  }
    void mezclar()                { equipos.mezclar();   }

    int getNumero() const { return numero;            }
    int tamano() const { return equipos.tamano(); }
    Equipo& operator[](int i){ return equipos[i];   }
    const Equipo& operator[](int i) const { return equipos[i];   }

    void imprimir() const {
        std::cout << "\n Bombo " << numero << " (" << equipos.tamano() << " equipos)\n";
        for (int i = 0; i < equipos.tamano(); i++) {
            std::cout << (i + 1) << ". ";
            equipos[i].imprimir();
        }
    }
};
// Clase de Grupo para la definicion de cada grupo para la definicion de los 12 que son pertinentes en esta instancia.
// Restriccion la UEFA puede tener dos selecciones en este apartado las demas Confe. No pueden hacerlo.

class Grupo {
    char          letra;
    Lista<Equipo> equipos;

    int contarDeConf(const char* conf) const {
        int cuenta = 0;
        for (int i = 0; i < equipos.tamano(); i++)
            if (strcmp(equipos[i].getConfederacion(), conf) == 0)
                cuenta++;
        return cuenta;
    }

public:
    Grupo(char l) : letra(l) {}
    Grupo(const Grupo&)            = delete;
    Grupo& operator=(const Grupo&) = delete;

    char getLetra() const { return letra;             }
    int  tamanio()  const { return equipos.tamano(); }
    void vaciar()         { equipos.vaciar();         }

    bool puedeAgregar(const Equipo& e) const {
        const char* conf    = e.getConfederacion();
        int         yaHay   = contarDeConf(conf);

        if (yaHay == 0) return true;
        if (strcmp(conf, "UEFA") == 0 && yaHay < 2)return true;
        return false;
    }

    bool agregar(const Equipo& e) {
        if (!puedeAgregar(e)) return false;
        equipos.agregar(e);
        return true;
    }

    void imprimir() const {
        std::cout << "\nGRUPO " << letra << ":\n";
        for (int i = 0; i < equipos.tamano(); i++) {
            std::cout << "  " << (i + 1) << ". ";
            equipos[i].imprimir();
        }
    }
};

//Clase para la realización del sorteo dados los bombos y los grupos definidos anteriormente.

class Sorteo {
    static const int total_bombos = 4;
    static const int total_grupos = 12;

    Bombo* bombos[total_bombos];
    Grupo* grupos[total_grupos];

    bool intentarSorteo() {
        for (int i = 0; i < total_grupos; i++)
            grupos[i]->vaciar();

        Lista<Equipo> copia;
        for (int j = 0; j < bombos[0]->tamano(); j++)
            copia.agregar((*bombos[0])[j]);
        copia.mezclar();

        for (int j = 0; j < copia.tamano(); j++) {
            if (strcmp(copia[j].getNombre(), "United States") == 0){
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
            Lista<Equipo> copiaBombo;
            for (int j = 0; j < bombos[b]->tamano(); j++)
                copiaBombo.agregar((*bombos[b])[j]);
            copiaBombo.mezclar();

            int cuantos = copiaBombo.tamano();
            for (int j = 0; j < cuantos; j++) {
                const Equipo& equipo = copiaBombo[j];

                Lista<int> validos;
                for (int g = 0; g < total_grupos; g++) {
                    if (grupos[g]->tamanio() == b && grupos[g]->puedeAgregar(equipo))
                        validos.agregar(g);
                }

                if (validos.vacia()) return false;

                int elegido = rand() % validos.tamano();
                grupos[validos[elegido]]->agregar(equipo);
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

    Sorteo(const Sorteo&) = delete;
    Sorteo& operator=(const Sorteo&) = delete;

    void conformarbombos(const Equipo* lista, int total) {
        Equipo  usa;
        Equipo* resto  = new Equipo[total - 1];
        int     nResto = 0;

        for (int i = 0; i < total; i++) {
            if (strcmp(lista[i].getNombre(), "United States") == 0)
                usa = lista[i];
            else
                resto[nResto++] = lista[i];
        }

        for (int i = 1; i < nResto; i++) {
            Equipo clave = resto[i];
            int    j     = i - 1;
            while (j >= 0 && resto[j].getRanking() > clave.getRanking()) {
                resto[j + 1] = resto[j];
                j--;
            }
            resto[j + 1] = clave;
        }

        bombos[0]->agregar(usa);
        for (int i =  0; i < 11; i++) bombos[0]->agregar(resto[i]);
        for (int i = 11; i < 23; i++) bombos[1]->agregar(resto[i]);
        for (int i = 23; i < 35; i++) bombos[2]->agregar(resto[i]);
        for (int i = 35; i < 47; i++) bombos[3]->agregar(resto[i]);

        delete[] resto;
    }

    void realizarSorteo() {
        srand(static_cast<unsigned>(time(nullptr)));
        int intentos = 0;
        while (!intentarSorteo()) {
            if (++intentos > 10000) {
                std::cout << "ERROR: no se encontro solucion en 10000 intentos.\n";
                return;
            }
        }
        if (intentos == 0)
            std::cout << "Sorteo completado al primer intento.\n";
        else
            std::cout << "Sorteo completado (" << intentos << " reintento(s)).\n";
    }

    void imprimirbombos() const {
        std::cout << "Bombos Sorteo";
        for (int i = 0; i < total_bombos; i++)
            bombos[i]->imprimir();
    }

    void imprimirGrupos() const {
        std::cout << "Grupos - Definidos";
        for (int i = 0; i < total_grupos; i++)
            grupos[i]->imprimir();
        std::cout << "\n";
    }

    Bombo* getBombo(int i) { return bombos[i]; }
    Grupo* getGrupo(int i) { return grupos[i]; }
};

#endif // BOMBOS_H