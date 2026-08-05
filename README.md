# Librería de Grafos C++

Librería **header-only** de "incluir y usar" para trabajar con grafos en C++.
Un único archivo (`grafo.hpp`), sin dependencias externas más allá de
`<iostream>` y sin contenedores STL: todas las estructuras están implementadas
a mano con nodos y punteros (`new`/`delete`).

Funciona con **cualquier tipo de dato** (tiene que soportar `operator==`) y es
resistente a fallos: valida índices fuera de rango, vértices/aristas
inexistentes, overflow en pesos y maneja la memoria de forma segura.

## Contenido

- `grafo.hpp` — la librería completa (estructuras base + dos implementaciones + algoritmos).
- `main.cpp` — programa de demo/prueba con test cases de cada algoritmo y de cada implementación.

## Compilar y ejecutar

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o main
./main          # en Windows: main.exe
```

## Incluir y usar

Solo copia `grafo.hpp` junto a tu código y agrégalo:

```cpp
#include <iostream>
#include "grafo.hpp"

int main() {
    // Eliges la implementación que prefieras:
    grafo::GrafoLista<int> g;   // lista de adyacencia
    // grafo::GrafoMatriz<int> g; // matriz de adyacencia

    g.agregarVertice(1);
    g.agregarVertice(2);
    g.agregarVertice(3);

    g.agregarArista(1, 2, 5.0);   // no dirigida, ponderada (peso 5)
    g.agregarArista(2, 3, 2.0, true); // dirigida, ponderada

    std::cout << "vertices=" << g.numVertices() << " aristas=" << g.numAristas() << "\n";

    // Camino más corto con Dijkstra
    grafo::Lista<int> ruta = grafo::dijkstra(g, 1, 3);
    for (auto* n = ruta.primer(); n != nullptr; n = n->siguiente) {
        std::cout << n->dato << " ";
    }
    std::cout << "\n";
    return 0;
}
```

## Estructura de la librería

### Estructuras base manuales

| Tipo | Descripción |
|------|-------------|
| `grafo::Lista<T>` | Lista enlazada simple (agregar, insertar, eliminar, buscar, recorrer). |
| `grafo::Cola<T>` | Cola FIFO (para BFS). |
| `grafo::Pila<T>` | Pila LIFO (para DFS). |

### Implementaciones de grafo

| Clase | Descripción |
|-------|-------------|
| `grafo::GrafoLista<T>` | Lista de adyacencia: cada vértice tiene una sub-lista de aristas. |
| `grafo::GrafoMatriz<T>` | Matriz de adyacencia con redimensionamiento automático. |

Ambas derivan de `grafo::GrafoBase<T>`, por lo que comparten la misma interfaz
y se pueden usar polimórficamente a través de un puntero a la clase base.

### API común (`GrafoBase<T>`)

| Método | Descripción |
|--------|-------------|
| `agregarVertice(v)` / `eliminarVertice(v)` | Agrega o elimina un vértice. |
| `agregarArista(o, d, peso=1.0, dirigida=false)` | Agrega una arista (ponderada o no, dirigida o no). |
| `eliminarArista(o, d, dirigida=false)` | Elimina una arista. |
| `existeVertice(v)` / `existeArista(o, d)` | Consultas de existencia. |
| `obtenerPeso(o, d)` | Peso de la arista `o -> d`. |
| `numVertices()` / `numAristas()` / `grado(v)` | Métricas. |
| `obtenerVecinos(v)` | Lista de adyacentes, como `grafo::Lista<T>`. |

### Algoritmos

| Función | Descripción |
|---------|-------------|
| `bfs(grafo, inicio)` | Recorrido en amplitud. |
| `dfs(grafo, inicio)` | Recorrido en profundidad. |
| `tieneCiclo(grafo, inicio, dirigida)` | Detección de ciclos. |
| `caminoBfsNoPonderado(grafo, inicio, destino)` | Camino más corto en grafos no ponderados. |
| `dijkstra(grafo, inicio, destino)` | Camino más corto con pesos positivos. |
| `bellmanFord(grafo, inicio, destino)` | Camino más corto con pesos negativos; detecta ciclos negativos. |
| `floydWarshall(grafo, vertices)` | Camino más corto entre todos los pares. |
| `componentesConexas(grafo, vertices)` | Componentes conexas. |
| `esConexo(grafo, vertices, dirigido=false)` | Conectividad / conexión fuerte. |
| `ordenTopologico(grafo, vertices)` | Orden topológico de un DAG. |
| `prim(grafo, vertices, arbolMST)` | Árbol de expansión mínima (retorna el peso total). |

Nota: las funciones que requieren conocer todos los vértices
(`floydWarshall`, `componentesConexas`, `esConexo`, `ordenTopologico`, `prim`)
reciben la lista de vértices como parámetro.

### Errores

Todas las operaciones lanzan excepciones derivadas de `grafo::ErrorGrafo`
cuando se usan de forma inválida:

- `grafo::VerticeInexistente` — vértice o arista inexistente.
- `grafo::OverflowPeso` — desbordamiento en la suma de pesos.
- `grafo::ErrorGrafo` — cualquier otro error (duplicados, ciclos, grafo no conexo, etc.).

```cpp
try {
    g.obtenerPeso(1, 99);   // 99 no existe
} catch (const grafo::ErrorGrafo& e) {
    std::cout << e.mensaje() << "\n";
}
```

## Tipos personalizados

El tipo de los vértices solo necesita `operator==`. Los tipos que imprimas
también pueden definir `operator<<`:

```cpp
struct Persona { int id; int edad; };

bool operator==(const Persona& a, const Persona& b) {
    return a.id == b.id && a.edad == b.edad;
}

grafo::GrafoLista<Persona> red;
red.agregarVertice({1, 30});
```
