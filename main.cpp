#include <iostream>

#include "grafo.hpp"

using namespace std;

struct Persona {
    int id;
    int edad;

    bool operator==(const Persona& otra) const {
        return id == otra.id && edad == otra.edad;
    }
};

ostream& operator<<(ostream& os, const Persona& p) {
    return os << "P" << p.id;
}

template <typename T>
void imprimirLista(const grafo::Lista<T>& lista) {
    cout << "[";
    for (auto* n = lista.primer(); n != nullptr; n = n->siguiente) {
        cout << n->dato;
        if (n->siguiente != nullptr) cout << ", ";
    }
    cout << "]";
}

void probarLista() {
    cout << "=== Lista<int>: operaciones basicas ===" << endl;

    grafo::Lista<int> lista;
    for (int i = 0; i < 6; ++i) lista.push_back(i * 10);

    cout << "tamano=" << lista.tamano() << " contenido=";
    imprimirLista(lista);
    cout << endl;

    lista.push_frente(-1);
    cout << "push_frente(-1): ";
    imprimirLista(lista);
    cout << endl;

    lista.insertar(3, 999);
    cout << "insertar(3, 999): ";
    imprimirLista(lista);
    cout << endl;

    cout << "at(3)=" << lista.at(3) << " lista[3]=" << lista[3] << endl;

    lista.eliminarPos(3);
    cout << "eliminarPos(3): ";
    imprimirLista(lista);
    cout << endl;

    cout << "eliminarValor(30)=" << lista.eliminarValor(30)
         << " eliminarValor(12345)=" << lista.eliminarValor(12345) << endl;
    cout << "contiene(20)=" << lista.contiene(20)
         << " indiceDe(40)=" << lista.indiceDe(40) << endl;

    lista.limpiar();
    cout << "despues de limpiar: vacia=" << lista.vacia()
         << " tamano=" << lista.tamano() << endl << endl;
}

void probarListaCopia() {
    cout << "=== Lista<int>: copia y asignacion ===" << endl;

    grafo::Lista<int> a;
    for (int i = 1; i <= 4; ++i) a.push_back(i);

    grafo::Lista<int> b(a);
    grafo::Lista<int> c;
    c = a;

    b.at(0) = 777;
    c.at(0) = 888;

    cout << "a=";
    imprimirLista(a);
    cout << " b=";
    imprimirLista(b);
    cout << " c=";
    imprimirLista(c);
    cout << endl << "(a no cambia al modificar b o c)" << endl << endl;
}

void probarListaRobustez() {
    cout << "=== Lista<int>: robustez ===" << endl;

    grafo::Lista<int> lista;
    try {
        lista.at(0);
    } catch (const grafo::ErrorGrafo& e) {
        cout << "at(0) en vacia -> capturado: " << e.mensaje() << endl;
    }

    try {
        lista.insertar(1, 42);
    } catch (const grafo::ErrorGrafo& e) {
        cout << "insertar pos 1 en vacia -> capturado: " << e.mensaje() << endl;
    }

    lista.push_back(1);
    try {
        lista.eliminarPos(5);
    } catch (const grafo::ErrorGrafo& e) {
        cout << "eliminarPos(5) fuera de rango -> capturado: " << e.mensaje()
             << endl;
    }

    cout << endl;
}

void probarCola() {
    cout << "=== Cola<int>: FIFO ===" << endl;

    grafo::Cola<int> cola;
    for (int i = 1; i <= 5; ++i) cola.encolar(i);

    cout << "tamano=" << cola.tamano() << " frente=" << cola.frente() << endl;
    while (!cola.vacia()) {
        cout << cola.frente();
        cola.desencolar();
        if (!cola.vacia()) cout << ", ";
    }
    cout << endl;

    try {
        cola.desencolar();
    } catch (const grafo::ErrorGrafo& e) {
        cout << "desencolar de vacia -> capturado: " << e.mensaje() << endl;
    }

    grafo::Cola<int> copia;
    for (int i = 7; i <= 9; ++i) copia.encolar(i);
    grafo::Cola<int> otra(copia);
    copia.desencolar();
    cout << "copia original intacta: frente(otra)=" << otra.frente()
         << " frente(copia)=" << copia.frente() << endl << endl;
}

void probarPila() {
    cout << "=== Pila<int>: LIFO ===" << endl;

    grafo::Pila<int> pila;
    for (int i = 1; i <= 5; ++i) pila.apilar(i);

    cout << "tamano=" << pila.tamano() << " tope=" << pila.tope() << endl;
    while (!pila.vacia()) {
        cout << pila.tope();
        pila.desapilar();
        if (!pila.vacia()) cout << ", ";
    }
    cout << endl;

    try {
        pila.desapilar();
    } catch (const grafo::ErrorGrafo& e) {
        cout << "desapilar de vacia -> capturado: " << e.mensaje() << endl;
    }
    cout << endl;
}

void probarTipoCustom() {
    cout << "=== Lista<Persona>: tipo custom ===" << endl;

    grafo::Lista<Persona> personas;
    personas.push_back({1, 30});
    personas.push_back({2, 25});
    personas.push_back({3, 28});

    Persona buscada{2, 25};
    cout << "indiceDe({2,25})=" << personas.indiceDe(buscada)
         << " contiene={2,25}=" << personas.contiene(buscada) << endl;
    cout << "contenido=";
    imprimirLista(personas);
    cout << endl;

    cout << "eliminarValor({2,25})=" << personas.eliminarValor(buscada)
         << " -> ";
    imprimirLista(personas);
    cout << endl << endl;
}

void probarErrores() {
    cout << "=== Errores comunes ===" << endl;

    cout << "sumaSegura(1e200, 1e200)=" << grafo::sumaSegura(1e200, 1e200)
         << endl;

    try {
        grafo::sumaSegura(grafo::MAX_PESO, grafo::MAX_PESO);
    } catch (const grafo::OverflowPeso& e) {
        cout << "suma que desborda -> capturado: " << e.mensaje() << endl;
    }

    try {
        grafo::validarPeso(grafo::INF);
    } catch (const grafo::ErrorGrafo& e) {
        cout << "peso centinela/INF -> capturado: " << e.mensaje() << endl;
    }

    try {
        grafo::validarPeso(0.5);
    } catch (...) {
        cout << "ERROR: validarPeso(0.5) no deberia lanzar" << endl;
    }
    cout << "(validarPeso(0.5) pasa sin error)" << endl;
}
void probarGrafoPolimorfico(grafo::GrafoBase<char>* g, const char* tipo) {
    cout << "=== Probando Polimorfismo: " << tipo << " ===" << endl;
    
    // 1. Agregar Vértices usando chars simples
    g->agregarVertice('C'); // C = Caracas
    g->agregarVertice('M'); // M = Maracaibo
    g->agregarVertice('V'); // V = Valencia

    // 2. Agregar Aristas
    g->agregarArista('C', 'M', 500.0, false); // No dirigida
    g->agregarArista('C', 'V', 150.0, true);  // Dirigida

    // 3. Consultas Básicas y Métricas
    cout << "Vertices (esp. 3): " << g->numVertices() << endl;
    cout << "Aristas (esp. 2): " << g->numAristas() << endl;
    cout << "Existe 'C'?: " << (g->existeVertice('C') ? "Si" : "No") << endl;
    cout << "Existe 'C' -> 'M'?: " << (g->existeArista('C', 'M') ? "Si" : "No") << endl;
    cout << "Peso 'C' -> 'V' (esp. 150): " << g->obtenerPeso('C', 'V') << endl;
    cout << "Grado de 'C' (esp. 2): " << g->grado('C') << endl;

    // 4. Obtener Vecinos
    cout << "Vecinos de 'C': ";
    imprimirLista(g->obtenerVecinos('C')); // Tu función imprimirLista funciona perfecto con char
    cout << endl;

    // 5. Eliminaciones
    g->eliminarArista('C', 'M', false);
    cout << "Aristas tras eliminar C-M (esp. 1): " << g->numAristas() << endl;

    g->eliminarVertice('V');
    cout << "Vertices tras eliminar V (esp. 2): " << g->numVertices() << endl;
    cout << endl;
}

void probarGrafos() {
    try {
        // Instanciamos usando tipo 'char' y punteros a la clase base
        grafo::GrafoBase<char>* grafoL = new grafo::GrafoLista<char>();
        grafo::GrafoBase<char>* grafoM = new grafo::GrafoMatriz<char>();

        probarGrafoPolimorfico(grafoL, "GrafoLista");
        probarGrafoPolimorfico(grafoM, "GrafoMatriz");

        // Liberamos memoria manual
        delete grafoL;
        delete grafoM;
        
        cout << "Pruebas de polimorfismo de grafos pasadas con exito." << endl << endl;
    } catch (const grafo::ErrorGrafo& e) {
        cout << "Error en prueba de grafos: " << e.mensaje() << endl;
    }
}
void probarBFS() {
    cout << "=== Pruebas de BFS (Amplitud) ===" << endl;
    grafo::GrafoLista<char> g;
    g.agregarVertice('A');
    g.agregarVertice('B');
    g.agregarVertice('C');
    g.agregarVertice('D');

    g.agregarArista('A', 'B', 1.0, false);
    g.agregarArista('A', 'C', 1.0, false);
    g.agregarArista('B', 'D', 1.0, false);

    grafo::Lista<char> recorrido = grafo::bfs(g, 'A');
    cout << "Orden BFS desde 'A': ";
    imprimirLista(recorrido);
    cout << endl << endl;
}

int main() {
    probarLista();
    probarListaCopia();
    probarListaRobustez();
    probarCola();
    probarPila();
    probarTipoCustom();
    probarErrores();

    probarGrafos();

    return 0;
}
