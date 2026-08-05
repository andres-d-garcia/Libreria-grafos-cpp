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
template <typename Grafo>
void probarAlgoritmos(const char* tipo) {
    cout << "=== Algoritmos con " << tipo << " ===" << endl;

    // Grafo dirigido ponderado (DAG) para recorridos y caminos
    Grafo g;
    for (char c : {'A', 'B', 'C', 'D', 'E'}) g.agregarVertice(c);
    g.agregarArista('A', 'B', 4.0, true);
    g.agregarArista('A', 'C', 2.0, true);
    g.agregarArista('C', 'B', 1.0, true);
    g.agregarArista('B', 'D', 5.0, true);
    g.agregarArista('C', 'D', 8.0, true);
    g.agregarArista('C', 'E', 10.0, true);
    g.agregarArista('D', 'E', 2.0, true);

    grafo::Lista<char> vertices;
    for (char c : {'A', 'B', 'C', 'D', 'E'}) vertices.push_back(c);

    cout << "BFS desde 'A': ";
    imprimirLista(grafo::bfs(g, 'A'));
    cout << endl;
    cout << "DFS desde 'A': ";
    imprimirLista(grafo::dfs(g, 'A'));
    cout << endl;
    cout << "tieneCiclo dirigido (esp. false): "
         << (grafo::tieneCiclo(g, 'A', true) ? "Si" : "No") << endl;
    cout << "caminoBfsNoPonderado A->E (esp. A,C,E): ";
    imprimirLista(grafo::caminoBfsNoPonderado(g, 'A', 'E'));
    cout << endl;
    cout << "dijkstra A->E (esp. A,C,B,D,E, costo 10): ";
    imprimirLista(grafo::dijkstra(g, 'A', 'E'));
    cout << endl;
    cout << "bellmanFord A->E (esp. A,C,B,D,E): ";
    imprimirLista(grafo::bellmanFord(g, 'A', 'E'));
    cout << endl;
    cout << "ordenTopologico (esp. A,C,B,D,E): ";
    imprimirLista(grafo::ordenTopologico(g, vertices));
    cout << endl;

    // Floyd-Warshall (todos los pares)
    auto rutas = grafo::floydWarshall(g, vertices);
    cout << "floydWarshall A->E (esp. A,C,B,D,E): ";
    imprimirLista(rutas.obtenerCamino('A', 'E'));
    cout << endl;

    // Ciclo en grafo dirigido -> no hay orden topologico
    Grafo gtopo;
    gtopo.agregarVertice('A');
    gtopo.agregarVertice('B');
    gtopo.agregarArista('A', 'B', 1.0, true);
    gtopo.agregarArista('B', 'A', 1.0, true);
    grafo::Lista<char> vtopo;
    vtopo.push_back('A');
    vtopo.push_back('B');
    try {
        grafo::ordenTopologico(gtopo, vtopo);
        cout << "ERROR: no deberia existir orden topologico" << endl;
    } catch (const grafo::ErrorGrafo& e) {
        cout << "ordenTopologico con ciclo -> capturado: " << e.mensaje() << endl;
    }

    // Bellman-Ford con arista negativa (sin ciclo negativo)
    Grafo gneg;
    for (char c : {'A', 'B', 'C', 'D'}) gneg.agregarVertice(c);
    gneg.agregarArista('A', 'C', 1.0, true);
    gneg.agregarArista('C', 'B', 2.0, true);
    gneg.agregarArista('B', 'D', -4.0, true);
    cout << "bellmanFord arista negativa A->D (esp. A,C,B,D): ";
    imprimirLista(grafo::bellmanFord(gneg, 'A', 'D'));
    cout << endl;

    // Ciclo de peso negativo -> Bellman-Ford debe lanzar
    Grafo gciclo;
    gciclo.agregarVertice('A');
    gciclo.agregarVertice('B');
    gciclo.agregarVertice('C');
    gciclo.agregarArista('A', 'B', 1.0, true);
    gciclo.agregarArista('B', 'C', 1.0, true);
    gciclo.agregarArista('C', 'A', -3.0, true);
    try {
        grafo::bellmanFord(gciclo, 'A', 'C');
        cout << "ERROR: no deberia detectar ciclo negativo" << endl;
    } catch (const grafo::ErrorGrafo& e) {
        cout << "ciclo negativo -> capturado: " << e.mensaje() << endl;
    }

    // Grafo no dirigido con una componente aislada y un bucle
    Grafo gu;
    for (char c : {'A', 'B', 'C', 'D', 'X'}) gu.agregarVertice(c);
    gu.agregarArista('A', 'B', 1.0, false);
    gu.agregarArista('B', 'C', 1.0, false);
    gu.agregarArista('A', 'C', 1.0, false);
    gu.agregarArista('D', 'D', 1.0, false);

    grafo::Lista<char> vu;
    for (char c : {'A', 'B', 'C', 'D', 'X'}) vu.push_back(c);

    auto comps = grafo::componentesConexas(gu, vu);
    cout << "componentesConexas (esp. 3): " << comps.tamano() << endl;
    cout << "esConexo no dirigido (esp. false): "
         << (grafo::esConexo(gu, vu) ? "Si" : "No") << endl;

    // Grafo dirigido fuertemente conexo
    Grafo gsc;
    for (char c : {'A', 'B', 'C'}) gsc.agregarVertice(c);
    gsc.agregarArista('A', 'B', 1.0, true);
    gsc.agregarArista('B', 'C', 1.0, true);
    gsc.agregarArista('C', 'A', 1.0, true);
    grafo::Lista<char> vsc;
    for (char c : {'A', 'B', 'C'}) vsc.push_back(c);
    cout << "esConexo fuertemente conexo (esp. true): "
         << (grafo::esConexo(gsc, vsc, true) ? "Si" : "No") << endl;

    // Prim (arbol de expansion minima)
    Grafo gp;
    for (char c : {'A', 'B', 'C', 'D'}) gp.agregarVertice(c);
    gp.agregarArista('A', 'B', 1.0, false);
    gp.agregarArista('A', 'C', 4.0, false);
    gp.agregarArista('B', 'C', 2.0, false);
    gp.agregarArista('B', 'D', 5.0, false);
    gp.agregarArista('C', 'D', 1.0, false);

    grafo::Lista<char> vp;
    for (char c : {'A', 'B', 'C', 'D'}) vp.push_back(c);

    grafo::Lista<grafo::Arista<char>> arbol;
    cout << "prim peso total MST (esp. 4): " << grafo::prim(gp, vp, arbol) << endl;

    // Prim sobre grafo no conexo -> error
    Grafo gn;
    gn.agregarVertice('A');
    gn.agregarVertice('B');
    gn.agregarVertice('X');
    gn.agregarArista('A', 'B', 1.0, false);
    grafo::Lista<char> vn;
    vn.push_back('A');
    vn.push_back('B');
    vn.push_back('X');
    grafo::Lista<grafo::Arista<char>> arb2;
    try {
        grafo::prim(gn, vn, arb2);
        cout << "ERROR: no deberia existir MST" << endl;
    } catch (const grafo::ErrorGrafo& e) {
        cout << "prim no conexo -> capturado: " << e.mensaje() << endl;
    }

    cout << endl;
}

void probarCasosLimite() {
    cout << "=== Casos limite: grafo vacio / 1 nodo / desconectados ===" << endl;

    // Grafo vacio
    grafo::GrafoLista<char> vacio;
    grafo::Lista<char> vv;
    cout << "Vacio: vertices=" << vacio.numVertices()
         << " aristas=" << vacio.numAristas()
         << " esConexo=" << grafo::esConexo(vacio, vv) << endl;

    try {
        grafo::bfs(vacio, 'A');
        cout << "ERROR: bfs no deberia lanzar" << endl;
    } catch (const grafo::VerticeInexistente&) {
        cout << "bfs en grafo vacio -> VerticeInexistente capturado" << endl;
    }

    grafo::Lista<grafo::Arista<char>> arbolVacio;
    cout << "prim en vacio (esp. 0): " << grafo::prim(vacio, vv, arbolVacio) << endl;

    // Grafo de un solo nodo
    grafo::GrafoLista<char> uno;
    uno.agregarVertice('A');
    grafo::Lista<char> v1;
    v1.push_back('A');
    cout << "1 nodo: grado=" << uno.grado('A')
         << " vecinos=";
    imprimirLista(uno.obtenerVecinos('A'));
    cout << " esConexo=" << grafo::esConexo(uno, v1) << endl;

    // Nodos desconectados
    grafo::GrafoLista<char> discon;
    discon.agregarVertice('A');
    discon.agregarVertice('B');
    discon.agregarVertice('C');
    grafo::Lista<char> vd;
    vd.push_back('A');
    vd.push_back('B');
    vd.push_back('C');

    auto comps = grafo::componentesConexas(discon, vd);
    cout << "3 nodos sin aristas: componentes (esp. 3)=" << comps.tamano() << endl;
    cout << "esConexo (esp. false): " << (grafo::esConexo(discon, vd) ? "Si" : "No") << endl;

    // Camino inexistente retorna lista vacia
    grafo::Lista<char> camino = grafo::caminoBfsNoPonderado(discon, 'A', 'B');
    cout << "camino A->B sin aristas (esp. vacio): ";
    imprimirLista(camino);
    cout << " tamano=" << camino.tamano() << endl;

    // Operaciones sobre vertices/aristas inexistentes
    try {
        discon.grado('Z');
    } catch (const grafo::VerticeInexistente&) {
        cout << "grado('Z') -> VerticeInexistente capturado" << endl;
    }

    try {
        discon.eliminarArista('A', 'B', false);
    } catch (const grafo::VerticeInexistente&) {
        cout << "eliminarArista inexistente -> VerticeInexistente capturado" << endl;
    }

    // Out of bounds en estructuras base
    grafo::Lista<int> listaVacia;
    try {
        listaVacia.at(0);
    } catch (const grafo::ErrorGrafo& e) {
        cout << "at(0) en lista vacia -> capturado: " << e.mensaje() << endl;
    }

    cout << endl;
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

    probarAlgoritmos<grafo::GrafoLista<char>>("GrafoLista");
    probarAlgoritmos<grafo::GrafoMatriz<char>>("GrafoMatriz");
    probarCasosLimite();

    return 0;
}
