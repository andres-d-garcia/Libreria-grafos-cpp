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
    
    // Auxiliar para insertar/actualizar una arista saliente
    void agregarAristaDirigida(NodoVertice* vOrigen, const T& destino, double peso) {
        NodoArista* actual = vOrigen->aristas;
        while (actual != nullptr) {
            if (actual->destino == destino) {
                actual->peso = peso; // Si ya existe, actualizamos su peso
                return;
            }
            actual = actual->siguiente;
        }
        // Si no existe, la agregamos al inicio de la sub-lista
        vOrigen->aristas = new NodoArista(destino, peso, vOrigen->aristas);
        ++num_aristas_cache;
    }
    bool eliminarAristaDirigida(NodoVertice* vOrigen, const T& destino) {
        NodoArista* prev = nullptr;
        NodoArista* curr = vOrigen->aristas;

        while (curr != nullptr) {
            if (curr->destino == destino) {
                if (prev == nullptr) {
                    vOrigen->aristas = curr->siguiente;
                } else {
                    prev->siguiente = curr->siguiente;
                }
                delete curr;
                --num_aristas_cache;
                return true;
            }
            prev = curr;
            curr = curr->siguiente;
        }
        return false;
    }
    void copiarDesde(const GrafoLista& otra) {
        NodoVertice* currV = otra.cabeza_vertices_;
        while (currV != nullptr) {
            agregarVertice(currV->dato);
            currV = currV->siguiente;
        }

        currV = otra.cabeza_vertices_;
        while (currV != nullptr) {
            NodoVertice* vOrigenNuevo = buscarVertice(currV->dato);
            NodoArista* currA = currV->aristas;
            while (currA != nullptr) {
                agregarAristaDirigida(vOrigenNuevo, currA->destino, currA->peso);
                currA = currA->siguiente;
            }
            currV = currV->siguiente;
        }
    }
    public:
    // --- Consultas básicas ---

    bool existeVertice(const T& vertice) const override {
        return buscarVertice(vertice) != nullptr;
    }

    bool existeArista(const T& origen, const T& destino) const override {
        NodoVertice* vOrigen = buscarVertice(origen);
        if (vOrigen == nullptr) return false;

        NodoArista* actual = vOrigen->aristas;
        while (actual != nullptr) {
            if (actual->destino == destino) return true;
            actual = actual->siguiente;
        }
        return false;
    }

    double obtenerPeso(const T& origen, const T& destino) const override {
        NodoVertice* vOrigen = buscarVertice(origen);
        if (vOrigen == nullptr) throw VerticeInexistente();

        NodoArista* actual = vOrigen->aristas;
        while (actual != nullptr) {
            if (actual->destino == destino) return actual->peso;
            actual = actual->siguiente;
        }
        throw VerticeInexistente();
    }

    // --- Métricas ---

    int numVertices() const override {
        return num_vertices_cache;
    }

    int numAristas() const override {
        return num_aristas_cache;
    }

    int grado(const T& vertice) const override {
        NodoVertice* vTarget = buscarVertice(vertice);
        if (vTarget == nullptr) throw VerticeInexistente();

        int g = 0;
        NodoArista* actual = vTarget->aristas;
        while (actual != nullptr) {
            ++g;
            actual = actual->siguiente;
        }
        return g;
    }

    // --- Vecinos / Conexiones ---

    Lista<T> obtenerVecinos(const T& vertice) const override {
        NodoVertice* vTarget = buscarVertice(vertice);
        if (vTarget == nullptr) throw VerticeInexistente();

        Lista<T> vecinos;
        NodoArista* actual = vTarget->aristas;
        while (actual != nullptr) {
            vecinos.push_back(actual->destino);
            actual = actual->siguiente;
        }
        return vecinos;
    }

    // Constructor
    GrafoLista() : cabeza_vertices_(nullptr), num_vertices_cache(0), num_aristas_cache(0) {}
    GrafoLista(const GrafoLista& otra)
        : cabeza_vertices_(nullptr), num_vertices_cache(0), num_aristas_cache(0) {
        copiarDesde(otra);
    }

    GrafoLista& operator=(const GrafoLista& otra) {
        if (this != &otra) {
            limpiarMemoria();
            copiarDesde(otra);
        }
        return *this;
    }

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
    // --- Modificadores de Vértices ---

    void agregarVertice(const T& vertice) override {
        if (buscarVertice(vertice) != nullptr) {
            throw ErrorGrafo("grafo: el vertice ya existe");
        }
        // Insertamos al inicio (O(1))
        cabeza_vertices_ = new NodoVertice(vertice, cabeza_vertices_);
        ++num_vertices_cache;
    }

    void eliminarVertice(const T& vertice) override {
        NodoVertice* vTarget = buscarVertice(vertice);
        if (vTarget == nullptr) {
            throw VerticeInexistente();
        }

        // 1. Eliminar todas las aristas entrantes hacia 'vertice' desde los demás vértices
        NodoVertice* actualV = cabeza_vertices_;
        while (actualV != nullptr) {
            if (actualV != vTarget) {
                NodoArista* prevA = nullptr;
                NodoArista* currA = actualV->aristas;
                while (currA != nullptr) {
                    if (currA->destino == vertice) {
                        NodoArista* aBorrar = currA;
                        if (prevA == nullptr) {
                            actualV->aristas = currA->siguiente;
                            currA = actualV->aristas;
                        } else {
                            prevA->siguiente = currA->siguiente;
                            currA = prevA->siguiente;
                        }
                        delete aBorrar;
                        --num_aristas_cache;
                    } else {
                        prevA = currA;
                        currA = currA->siguiente;
                    }
                }
            }
            actualV = actualV->siguiente;
        }

        // 2. Eliminar todas las aristas salientes del propio 'vertice'
        NodoArista* actualA = vTarget->aristas;
        while (actualA != nullptr) {
            NodoArista* aBorrar = actualA;
            actualA = actualA->siguiente;
            delete aBorrar;
            --num_aristas_cache;
        }
        vTarget->aristas = nullptr;

        // 3. Eliminar 'vertice' de la lista principal de vértices
        NodoVertice* prevV = nullptr;
        actualV = cabeza_vertices_;
        while (actualV != nullptr) {
            if (actualV == vTarget) {
                if (prevV == nullptr) {
                    cabeza_vertices_ = actualV->siguiente;
                } else {
                    prevV->siguiente = actualV->siguiente;
                }
                delete actualV;
                break;
            }
            prevV = actualV;
            actualV = actualV->siguiente;
        }

        --num_vertices_cache;
    }

    // --- Modificadores de Aristas ---

    void agregarArista(const T& origen, const T& destino, double peso = 1.0, bool dirigida = false) override {
        validarPeso(peso);

        NodoVertice* vOrigen = buscarVertice(origen);
        NodoVertice* vDestino = buscarVertice(destino);

        if (vOrigen == nullptr || vDestino == nullptr) {
            throw VerticeInexistente();
        }

        agregarAristaDirigida(vOrigen, destino, peso);

        if (!dirigida && !(origen == destino)) {
            agregarAristaDirigida(vDestino, origen, peso);
        }
    }

    void eliminarArista(const T& origen, const T& destino, bool dirigida = false) override {
        NodoVertice* vOrigen = buscarVertice(origen);
        NodoVertice* vDestino = buscarVertice(destino);

        if (vOrigen == nullptr || vDestino == nullptr) {
            throw VerticeInexistente();
        }

        bool eliminada = eliminarAristaDirigida(vOrigen, destino);

        if (!dirigida && !(origen == destino)) {
            eliminarAristaDirigida(vDestino, origen);
        }

        if (!eliminada) {
            throw VerticeInexistente();
        }
    }
};
// ---- GrafoMatriz<T>: Implementación mediante Matriz de Adyacencia ------------

template <typename T>
class GrafoMatriz : public GrafoBase<T> {
private:
    Lista<T> vertices_;      // Lista lineal para almacenar los vértices y mapearlos por índice
    double** matriz_;        // Matriz bidimensional dinámica para los pesos
    int capacidad_;          // Capacidad actual reservada en la matriz
    int num_aristas_cache;   // Contador de aristas para O(1)

    // Redimensionar la matriz cuando se supera la capacidad
    void redimensionar( nueva_capacidad) {
        // Reservar nueva matriz
        double** nueva_matriz = new double*[nueva_capacidad];
        for (int i = 0; i < nueva_capacidad; ++i) {
            nueva_matriz[i] = new double[nueva_capacidad];
            for (int j = 0; j < nueva_capacidad; ++j) {
                nueva_matriz[i][j] = INF; // Inicializar todo con ausencia de arista
            }
        }

        // Copiar datos anteriores si existían
        if (matriz_ != nullptr) {
            int n = vertices_.tamano();
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    nueva_matriz[i][j] = matriz_[i][j];
                }
            }
            // Liberar matriz antigua
            for (int i = 0; i < capacidad_; ++i) {
                delete[] matriz_[i];
            }
            delete[] matriz_;
        }

        matriz_ = nueva_matriz;
        capacidad_ = nueva_capacidad;
    }

    void liberarMemoria() {
        if (matriz_ != nullptr) {
            for (int i = 0; i < capacidad_; ++i) {
                delete[] matriz_[i];
            }
            delete[] matriz_;
            matriz_ = nullptr;
        }
        vertices_.limpiar();
        capacidad_ = 0;
        num_aristas_cache = 0;
    }

    void copiarDesde(const GrafoMatriz& otra) {
        liberarMemoria();
        capacidad_ = otra.capacidad_;
        num_aristas_cache = otra.num_aristas_cache;
        vertices_ = otra.vertices_;

        matriz_ = new double*[capacidad_];
        for (int i = 0; i < capacidad_; ++i) {
            matriz_[i] = new double[capacidad_];
            for (int j = 0; j < capacidad_; ++j) {
                matriz_[i][j] = otra.matriz_[i][j];
            }
        }
    }

public:
    // Constructor por defecto (capacidad inicial por ejemplo 10)
    GrafoMatriz(int capacidad_inicial = 10) 
        : matriz_(nullptr), capacidad_(0), num_aristas_cache(0) {
        redimensionar(capacidad_inicial);
    }

    // Constructor de copia (Regla de los 3)
    GrafoMatriz(const GrafoMatriz& otra) 
        : matriz_(nullptr), capacidad_(0), num_aristas_cache(0) {
        copiarDesde(otra);
    }

    // Operador de asignación (Regla de los 3)
    GrafoMatriz& operator=(const GrafoMatriz& otra) {
        if (this != &otra) {
            copiarDesde(otra);
        }
        return *this;
    }

    // Destructor
    ~GrafoMatriz() override {
        liberarMemoria();
    }
    // --- Modificadores de Vértices ---

    void agregarVertice(const T& vertice) override {
        if (vertices_.contiene(vertice)) {
            throw ErrorGrafo("grafo: el vertice ya existe");
        }
        
        int n = vertices_.tamano();
        
        // Si alcanzamos la capacidad máxima, redimensionamos (duplicando el tamaño)
        if (n == capacidad_) {
            redimensionar(capacidad_ == 0 ? 10 : capacidad_ * 2);
        }
        
        // Agregamos el vértice al final de nuestra lista de mapeo
        vertices_.push_back(vertice);
        
        // Aseguramos que la nueva fila y columna estén inicializadas en INF
        for (int i = 0; i <= n; ++i) {
            matriz_[n][i] = INF;
            matriz_[i][n] = INF;
        }
    }

    void eliminarVertice(const T& vertice) override {
        int idx = vertices_.indiceDe(vertice);
        if (idx < 0) {
            throw VerticeInexistente();
        }

        int n = vertices_.tamano();

        // 1. Restar del caché las aristas conectadas al vértice que vamos a eliminar
        for (int i = 0; i < n; ++i) {
            if (matriz_[idx][i] != INF) --num_aristas_cache; // Aristas salientes
            if (i != idx && matriz_[i][idx] != INF) --num_aristas_cache; // Aristas entrantes (evita doble resta si hay bucle)
        }

        // 2. Desplazar filas hacia arriba para sobreescribir la fila 'idx'
        for (int i = idx; i < n - 1; ++i) {
            for (int j = 0; j < n; ++j) {
                matriz_[i][j] = matriz_[i + 1][j];
            }
        }

        // 3. Desplazar columnas hacia la izquierda para sobreescribir la columna 'idx'
        for (int i = 0; i < n - 1; ++i) {
            for (int j = idx; j < n - 1; ++j) {
                matriz_[i][j] = matriz_[i][j + 1];
            }
        }

        // 4. Limpiar la última fila y columna (dejadas atrás por el desplazamiento)
        for (int i = 0; i < n; ++i) {
            matriz_[n - 1][i] = INF;
            matriz_[i][n - 1] = INF;
        }

        // 5. Eliminar el vértice de la lista de mapeo
        vertices_.eliminarPos(static_cast<size_t>(idx));
    }
    
    // --- Modificadores de Aristas ---

    void agregarArista(const T& origen, const T& destino, double peso = 1.0, bool dirigida = false) override {
        validarPeso(peso);

        int idx_origen = vertices_.indiceDe(origen);
        int idx_destino = vertices_.indiceDe(destino);

        if (idx_origen < 0 || idx_destino < 0) {
            throw VerticeInexistente();
        }

        // Si no existía la arista en esa dirección, aumentamos el caché
        if (matriz_[idx_origen][idx_destino] == INF) {
            ++num_aristas_cache;
        }
        
        // Asignamos el peso en la intersección (O(1))
        matriz_[idx_origen][idx_destino] = peso;

        // Si no es dirigida y no es un bucle hacia sí mismo, replicamos en el sentido inverso
        if (!dirigida && idx_origen != idx_destino) {
            if (matriz_[idx_destino][idx_origen] == INF) {
                ++num_aristas_cache;
            }
            matriz_[idx_destino][idx_origen] = peso;
        }
    }

    void eliminarArista(const T& origen, const T& destino, bool dirigida = false) override {
        int idx_origen = vertices_.indiceDe(origen);
        int idx_destino = vertices_.indiceDe(destino);

        if (idx_origen < 0 || idx_destino < 0) {
            throw VerticeInexistente();
        }

        bool eliminada = false;

        // Verificamos si existe la arista principal
        if (matriz_[idx_origen][idx_destino] != INF) {
            matriz_[idx_origen][idx_destino] = INF; // Restablecemos a ausencia de arista
            --num_aristas_cache;
            eliminada = true;
        }

        // Si no es dirigida, eliminamos el sentido inverso
        if (!dirigida && idx_origen != idx_destino) {
            if (matriz_[idx_destino][idx_origen] != INF) {
                matriz_[idx_destino][idx_origen] = INF;
                --num_aristas_cache;
            }
        }

        // Si no se encontró la arista para eliminar, lanzamos la excepción para mantener consistencia
        if (!eliminada) {
            throw VerticeInexistente();
        }
    }
    // --- Consultas básicas ---

    bool existeVertice(const T& vertice) const override {
        return vertices_.contiene(vertice);
    }

    bool existeArista(const T& origen, const T& destino) const override {
        int idx_origen = vertices_.indiceDe(origen);
        int idx_destino = vertices_.indiceDe(destino);

        if (idx_origen < 0 || idx_destino < 0) return false;

        return matriz_[idx_origen][idx_destino] != INF;
    }

    double obtenerPeso(const T& origen, const T& destino) const override {
        int idx_origen = vertices_.indiceDe(origen);
        int idx_destino = vertices_.indiceDe(destino);

        if (idx_origen < 0 || idx_destino < 0) {
            throw VerticeInexistente();
        }

        double peso = matriz_[idx_origen][idx_destino];
        if (peso == INF) {
            throw VerticeInexistente();
        }

        return peso;
    }

    // --- Métricas ---

    int numVertices() const override {
        return static_cast<int>(vertices_.tamano());
    }

    int numAristas() const override {
        return num_aristas_cache;
    }

    int grado(const T& vertice) const override {
        int idx = vertices_.indiceDe(vertice);
        if (idx < 0) {
            throw VerticeInexistente();
        }

        int g = 0;
        int n = vertices_.tamano();
        // Contamos cuántas conexiones salientes tiene en su fila
        for (int i = 0; i < n; ++i) {
            if (matriz_[idx][i] != INF) {
                ++g;
            }
        }
        return g;
    }

    // --- Vecinos / Conexiones ---

    Lista<T> obtenerVecinos(const T& vertice) const override {
        int idx = vertices_.indiceDe(vertice);
        if (idx < 0) {
            throw VerticeInexistente();
        }

        Lista<T> vecinos;
        int n = vertices_.tamano();
        // Recorremos la fila buscando conexiones válidas
        for (int i = 0; i < n; ++i) {
            if (matriz_[idx][i] != INF) {
                // Si hay conexión, añadimos el vértice correspondiente usando su índice
                vecinos.push_back(vertices_[i]); 
            }
        }
        return vecinos;
    }
};
// ---- Algoritmo BFS (Búsqueda en Anchura) -----------------------------------

template <typename T>
Lista<T> bfs(const GrafoBase<T>& grafo, const T& inicio) {
    if (!grafo.existeVertice(inicio)) {
        throw VerticeInexistente();
    }

    Lista<T> ordenRecorrido;
    Lista<T> visitados;
    Cola<T> cola;

    cola.encolar(inicio);
    visitados.push_back(inicio);

    while (!cola.vacia()) {
        T actual = cola.frente();
        cola.desencolar();
        ordenRecorrido.push_back(actual);

        // Obtener vecinos usando nuestra estructura Lista<T>
        Lista<T> vecinos = grafo.obtenerVecinos(actual);
        for (auto* n = vecinos.primer(); n != nullptr; n = n->siguiente) {
            const T& vecino = n->dato;
            if (!visitados.contiene(vecino)) {
                visitados.push_back(vecino);
                cola.encolar(vecino);
            }
        }
    }

    return ordenRecorrido;
}
// ---- Algoritmo DFS (Búsqueda en Profundidad) -------------------------------

template <typename T>
Lista<T> dfs(const GrafoBase<T>& grafo, const T& inicio) {
    if (!grafo.existeVertice(inicio)) {
        throw VerticeInexistente();
    }

    Lista<T> ordenRecorrido;
    Lista<T> visitados;
    Pila<T> pila;

    pila.apilar(inicio);

    while (!pila.vacia()) {
        T actual = pila.tope();
        pila.desapilar();

        if (!visitados.contiene(actual)) {
            visitados.push_back(actual);
            ordenRecorrido.push_back(actual);

            // Obtenemos los vecinos del vértice actual
            Lista<T> vecinos = grafo.obtenerVecinos(actual);

            // Para que la Pila procese los vecinos en orden natural (de izquierda a derecha),
            // utilizamos una pila auxiliar temporal para invertir el orden de apilamiento.
            Pila<T> pilaInversa;
            for (auto* n = vecinos.primer(); n != nullptr; n = n->siguiente) {
                const T& vecino = n->dato;
                if (!visitados.contiene(vecino)) {
                    pilaInversa.apilar(vecino);
                }
            }

            while (!pilaInversa.vacia()) {
                pila.apilar(pilaInversa.tope());
                pilaInversa.desapilar();
            }
        }
    }

    return ordenRecorrido;
}
}  // namespace grafo

#endif  // GRAFO_HPP
