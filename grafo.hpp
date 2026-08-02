#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <iostream>

// Librería de grafos C++ — header-only, "incluir y usar".
// Un único archivo; solo <iostream> como librería externa, el resto son
// estructuras implementadas a mano con nodos y punteros (new/delete).

namespace grafo {

// ---- Constantes y errores comunes ------------------------------------------

// Valor que representa la ausencia de arista.
inline constexpr double INF = 1e300;

// Cota para detectar overflow en sumas.
inline constexpr double MAX_PESO = 1.7e308;

class ErrorGrafo {
public:
    explicit ErrorGrafo(const char* msg) : mensaje_(msg) {}
    const char* mensaje() const { return mensaje_; }

private:
    const char* mensaje_;
};

class VerticeInexistente : public ErrorGrafo {
public:
    VerticeInexistente()
        : ErrorGrafo("grafo: el vertice o la arista no existe") {}
};

class OverflowPeso : public ErrorGrafo {
public:
    OverflowPeso() : ErrorGrafo("grafo: desbordamiento en la suma de pesos") {}
};

// Lanza OverflowPeso si la suma se desborda o un operando es NaN.
inline double sumaSegura(double a, double b) {
    if (a != a || b != b) throw OverflowPeso();
    if (a > 0 && b > MAX_PESO - a) throw OverflowPeso();
    if (a < 0 && b < -MAX_PESO - a) throw OverflowPeso();
    return a + b;
}

inline void validarPeso(double peso) {
    if (peso != peso || peso >= INF || peso <= -INF) {
        throw ErrorGrafo("grafo: el peso de una arista debe ser finito");
    }
}

// ---- Lista<T>: lista enlazada simple ----------------------------------------

template <typename T>
class Lista {
public:
    struct Nodo {
        T dato;
        Nodo* siguiente;
    };

    Lista() : cabeza_(nullptr), tam_(0) {}
    ~Lista() { limpiar(); }

    Lista(const Lista& otra) : cabeza_(nullptr), tam_(0) {
        copiarDesde(otra);
    }

    Lista& operator=(const Lista& otra) {
        if (this != &otra) copiarDesde(otra);
        return *this;
    }

    size_t tamano() const { return tam_; }
    bool vacia() const { return tam_ == 0; }

    // Puntero al primer nodo, para recorrerla manualmente.
    Nodo* primer() const { return cabeza_; }

    int indiceDe(const T& valor) const {
        Nodo* n = cabeza_;
        int i = 0;
        while (n != nullptr) {
            if (n->dato == valor) return i;
            n = n->siguiente;
            ++i;
        }
        return -1;
    }

    bool contiene(const T& valor) const { return indiceDe(valor) >= 0; }

    // Lanza ErrorGrafo si pos >= tamano().
    T& at(size_t pos) {
        verificarIndice(pos);
        return nodoEn(pos)->dato;
    }

    const T& at(size_t pos) const {
        verificarIndice(pos);
        return nodoEn(pos)->dato;
    }

    T& operator[](size_t pos) { return at(pos); }
    const T& operator[](size_t pos) const { return at(pos); }

    void push_back(const T& valor) {
        Nodo* nuevo = new Nodo{valor, nullptr};
        if (cabeza_ == nullptr) {
            cabeza_ = nuevo;
        } else {
            Nodo* n = cabeza_;
            while (n->siguiente != nullptr) n = n->siguiente;
            n->siguiente = nuevo;
        }
        ++tam_;
    }

    void push_frente(const T& valor) {
        cabeza_ = new Nodo{valor, cabeza_};
        ++tam_;
    }

    void insertar(size_t pos, const T& valor) {
        verificarPosicion(pos);
        if (pos == 0) {
            push_frente(valor);
            return;
        }
        Nodo* anterior = nodoEn(pos - 1);
        anterior->siguiente = new Nodo{valor, anterior->siguiente};
        ++tam_;
    }

    void eliminarPos(size_t pos) {
        verificarIndice(pos);
        Nodo* aEliminar;
        if (pos == 0) {
            aEliminar = cabeza_;
            cabeza_ = cabeza_->siguiente;
        } else {
            Nodo* anterior = nodoEn(pos - 1);
            aEliminar = anterior->siguiente;
            anterior->siguiente = aEliminar->siguiente;
        }
        delete aEliminar;
        --tam_;
    }

    bool eliminarValor(const T& valor) {
        const int idx = indiceDe(valor);
        if (idx < 0) return false;
        eliminarPos(static_cast<size_t>(idx));
        return true;
    }

    void limpiar() {
        Nodo* n = cabeza_;
        while (n != nullptr) {
            Nodo* sig = n->siguiente;
            delete n;
            n = sig;
        }
        cabeza_ = nullptr;
        tam_ = 0;
    }

private:
    Nodo* nodoEn(size_t pos) const {
        Nodo* n = cabeza_;
        for (size_t i = 0; i < pos; ++i) n = n->siguiente;
        return n;
    }

    void verificarIndice(size_t pos) const {
        if (pos >= tam_) throw ErrorGrafo("Lista: indice fuera de rango");
    }

    void verificarPosicion(size_t pos) const {
        if (pos > tam_) throw ErrorGrafo("Lista: posicion fuera de rango");
    }

    void copiarDesde(const Lista& otra) {
        limpiar();
        for (Nodo* n = otra.cabeza_; n != nullptr; n = n->siguiente) {
            push_back(n->dato);
        }
    }

    Nodo* cabeza_;
    size_t tam_;
};

// ---- Cola<T> (FIFO) y Pila<T> (LIFO) -----------------------------------------

template <typename T>
class Cola {
public:
    Cola() : frente_(nullptr), fondo_(nullptr), tam_(0) {}
    ~Cola() { limpiar(); }

    Cola(const Cola& otra) : frente_(nullptr), fondo_(nullptr), tam_(0) {
        copiarDesde(otra);
    }

    Cola& operator=(const Cola& otra) {
        if (this != &otra) copiarDesde(otra);
        return *this;
    }

    void encolar(const T& valor) {
        Nodo* nuevo = new Nodo{valor, nullptr};
        if (fondo_ == nullptr) {
            frente_ = fondo_ = nuevo;
        } else {
            fondo_->siguiente = nuevo;
            fondo_ = nuevo;
        }
        ++tam_;
    }

    void desencolar() {
        if (frente_ == nullptr) throw ErrorGrafo("Cola: cola vacia");
        Nodo* aux = frente_;
        frente_ = frente_->siguiente;
        if (frente_ == nullptr) fondo_ = nullptr;
        delete aux;
        --tam_;
    }

    T& frente() {
        if (frente_ == nullptr) throw ErrorGrafo("Cola: cola vacia");
        return frente_->dato;
    }

    const T& frente() const {
        if (frente_ == nullptr) throw ErrorGrafo("Cola: cola vacia");
        return frente_->dato;
    }

    bool vacia() const { return frente_ == nullptr; }
    size_t tamano() const { return tam_; }

    void limpiar() {
        while (frente_ != nullptr) {
            Nodo* aux = frente_;
            frente_ = frente_->siguiente;
            delete aux;
        }
        frente_ = fondo_ = nullptr;
        tam_ = 0;
    }

private:
    struct Nodo {
        T dato;
        Nodo* siguiente;
    };

    void copiarDesde(const Cola& otra) {
        limpiar();
        for (Nodo* n = otra.frente_; n != nullptr; n = n->siguiente) {
            encolar(n->dato);
        }
    }

    Nodo* frente_;
    Nodo* fondo_;
    size_t tam_;
};

template <typename T>
class Pila {
public:
    Pila() : tope_(nullptr), tam_(0) {}
    ~Pila() { limpiar(); }

    Pila(const Pila& otra) : tope_(nullptr), tam_(0) {
        copiarDesde(otra);
    }

    Pila& operator=(const Pila& otra) {
        if (this != &otra) copiarDesde(otra);
        return *this;
    }

    void apilar(const T& valor) {
        tope_ = new Nodo{valor, tope_};
        ++tam_;
    }

    void desapilar() {
        if (tope_ == nullptr) throw ErrorGrafo("Pila: pila vacia");
        Nodo* aux = tope_;
        tope_ = tope_->siguiente;
        delete aux;
        --tam_;
    }

    T& tope() {
        if (tope_ == nullptr) throw ErrorGrafo("Pila: pila vacia");
        return tope_->dato;
    }

    const T& tope() const {
        if (tope_ == nullptr) throw ErrorGrafo("Pila: pila vacia");
        return tope_->dato;
    }

    bool vacia() const { return tope_ == nullptr; }
    size_t tamano() const { return tam_; }

    void limpiar() {
        while (tope_ != nullptr) {
            Nodo* aux = tope_;
            tope_ = tope_->siguiente;
            delete aux;
        }
        tam_ = 0;
    }

private:
    struct Nodo {
        T dato;
        Nodo* siguiente;
    };

    void copiarDesde(const Pila& otra) {
        limpiar();
        Pila auxiliar;
        for (Nodo* n = otra.tope_; n != nullptr; n = n->siguiente) {
            auxiliar.apilar(n->dato);
        }
        while (!auxiliar.vacia()) {
            apilar(auxiliar.tope());
            auxiliar.desapilar();
        }
    }

    Nodo* tope_;
    size_t tam_;
};

// ---- GrafoBase<T>: Interfaz común para todas las implementaciones -----------

template <typename T>
class GrafoBase {
public:
    virtual ~GrafoBase() {}

    // --- Modificadores ---
    virtual void agregarVertice(const T& vertice) = 0;
    virtual void eliminarVertice(const T& vertice) = 0;
    
    // Por defecto asume aristas no ponderadas (peso 1.0) y no dirigidas
    virtual void agregarArista(const T& origen, const T& destino, double peso = 1.0, bool dirigida = false) = 0;
    virtual void eliminarArista(const T& origen, const T& destino, bool dirigida = false) = 0;

    // --- Consultas básicas ---
    virtual bool existeVertice(const T& vertice) const = 0;
    virtual bool existeArista(const T& origen, const T& destino) const = 0;
    virtual double obtenerPeso(const T& origen, const T& destino) const = 0;

    // --- Métricas ---
    virtual int numVertices() const = 0;
    virtual int numAristas() const = 0;
    virtual int grado(const T& vertice) const = 0;

    // --- Vecinos / Conexiones ---
    // Retorna una Lista<T> (nuestra estructura manual) con los vértices adyacentes
    virtual Lista<T> obtenerVecinos(const T& vertice) const = 0;
};

// ---- GrafoLista<T>: Implementación mediante Lista de Adyacencia ------------

template <typename T>
class GrafoLista : public GrafoBase<T> {
private:
    // Nodo para representar cada conexión (arista)
    struct NodoArista {
        T destino;
        double peso;
        NodoArista* siguiente;

        NodoArista(const T& dest, double p, NodoArista* sig = nullptr)
            : destino(dest), peso(p), siguiente(sig) {}
    };

    // Nodo para representar cada vértice del grafo
    struct NodoVertice {
        T dato;
        NodoArista* aristas;     // Puntero a la sub-lista de adyacencia
        NodoVertice* siguiente;  // Puntero al siguiente vértice en la lista principal

        NodoVertice(const T& d, NodoVertice* sig = nullptr)
            : dato(d), aristas(nullptr), siguiente(sig) {}
    };

    NodoVertice* cabeza_vertices_; // Inicio de la lista enlazada de vértices
    int num_vertices_cache;        // Contadores para hacer numVertices() O(1)
    int num_aristas_cache;         // Contadores para hacer numAristas() O(1)

    // Método auxiliar privado para buscar un vértice
    NodoVertice* buscarVertice(const T& vertice) const {
        NodoVertice* actual = cabeza_vertices_;
        while (actual != nullptr) {
            if (actual->dato == vertice) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

public:
    // Constructor
    GrafoLista() : cabeza_vertices_(nullptr), num_vertices_cache(0), num_aristas_cache(0) {}

    // Destructor (Falta implementar la limpieza de memoria)
    ~GrafoLista() override {
        limpiarMemoria();
    }

    // Método para vaciar el grafo y liberar memoria
    void limpiarMemoria() {
        NodoVertice* actualV = cabeza_vertices_;
        while (actualV != nullptr) {
            // 1. Borrar todas las aristas del vértice actual
            NodoArista* actualA = actualV->aristas;
            while (actualA != nullptr) {
                NodoArista* aBorrar = actualA;
                actualA = actualA->siguiente;
                delete aBorrar;
            }
            // 2. Borrar el vértice en sí
            NodoVertice* vBorrar = actualV;
            actualV = actualV->siguiente;
            delete vBorrar;
        }
        cabeza_vertices_ = nullptr;
        num_vertices_cache = 0;
        num_aristas_cache = 0;
    }

    
};
}  // namespace grafo

#endif  // GRAFO_HPP
