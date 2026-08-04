# Checklist — Librería de Grafos C++ (Pareja)

Leyenda:
- **[P1]** — Programador 1
- **[P2]** — Programador 2
- **[Ambos]** — tarea compartida

## 1. Planificación
- [x] 1.1 Revisar el proyecto de árboles anterior y replicar su estructura/entregables
- [x] 1.2 Decidir estructura: librería *header-only* (`.hpp`) de "incluir y usar"
- [x] 1.3 Todo en un **único `grafo.hpp`**; solo `<iostream>` como librería externa; sin contenedores STL (ni siquiera `std::vector`)

## 2. Estructuras base manuales (en `grafo.hpp`, sin contenedores STL)
- [x] **[Ambos]** Errores y validación comunes (out of bounds, vértices/aristas inexistentes)
- [x] **[P2]** `Lista<T>`: lista enlazada simple (agregar, eliminar, buscar, recorrer)
- [x] **[P2]** `Cola<T>` y `Pila<T>` propias (para BFS/DFS)

## 3. API común
- [x] **[Ambos]** Definir `GrafoBase<T>`: misma interfaz para ambas implementaciones

## 4. Grafo con lista de adyacencia — [P1]
- [x] Estructura: `NodoVertice` con lista enlazada de `NodoArista` (punteros y `new`/`delete`)
- [x] Agregar/eliminar vértice
- [x] Agregar/eliminar arista (dirigida y no dirigida, ponderada y no ponderada)
- [x] Consultas: vecinos, grado, número de vértices/aristas
- [x] Robustez: validaciones y casos límite

## 5. Grafo con matriz de adyacencia — [P2]
- [x] Estructura: **matriz dispersa enlazada** (lista de filas, cada fila con lista de celdas `{columna, peso}`; sin arreglos)
- [x] Agregar/eliminar vértice (manteniendo la matriz consistente)
- [x] Agregar/eliminar arista (dirigida y no dirigida, ponderada y no ponderada)
- [x] Consultas: vecinos, grado, número de vértices/aristas
- [x] Robustez: validaciones y casos límite

## 6. Algoritmos de recorrido
- [x] **[P1]** BFS (amplitud)
- [x] **[P1]** DFS (profundidad)
- [x] **[P1]** Detección de ciclos

## 7. Camino más corto
- [ ] **[P2]** BFS para grafos no ponderados
- [ ] **[P2]** Dijkstra
- [ ] **[P2]** Bellman-Ford (aristas negativas)
- [ ] **[Ambos]** Floyd-Warshall (todos los pares)

## 8. Otras funciones relevantes
- [ ] **[P1]** Verificación de conectividad / componentes conexas
- [ ] **[P2]** Grafo conectado o fuertemente conexo
- [ ] **[P2]** Orden topológico (si aplica)
- [ ] **[P1]** Árbol de expansión mínima (Kruskal/Prim) — opcional

## 9. Robustez / resistencia a fallos
- [ ] Validación de índices fuera de rango (out of bounds)
- [ ] Manejo de aristas/vértices inexistentes
- [ ] Detección de overflow en pesos/sumas
- [ ] Uso seguro de memoria (`new`/`delete` balanceados, sin fugas)
- [ ] Pruebas con casos límite (grafo vacío, 1 nodo, nodos desconectados)

## 10. Pruebas y verificación
- [ ] Programa de prueba/demo en `main.cpp`
- [ ] Test cases de cada algoritmo y de cada implementación
- [ ] Compilar sin warnings (con `-fsanitize=address,undefined` si el toolchain lo permite)

## 11. Documentación y entrega
- [ ] `README.md` con ejemplos de uso ("incluir y usar")
- [ ] Comentarios/doxygen en la API
- [ ] Commits con formato **Conventional Commits** (ej. `feat: add adjacency list`, `fix: validate out of bounds`)
- [ ] Subir versión final a GitHub
