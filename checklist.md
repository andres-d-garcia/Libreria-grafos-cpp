# Checklist — Librería de Grafos C++

## Planificación
- [ ] Revisar el proyecto de árboles anterior y replicar su estructura/entregables
- [ ] Decidir estructura: librería *header-only* (`.hpp`) de "incluir y usar"

## Núcleo de la librería
- [ ] Implementación con plantillas `template <typename T>` para cualquier tipo de dato
- [ ] Versión con **lista de adyacencia**
- [ ] Versión con **matriz de adyacencia**
- [ ] API común en ambas (misma interfaz para el usuario)

## Operaciones básicas
- [ ] Agregar/eliminar vértice
- [ ] Agregar/eliminar arista (dirigida y no dirigida)
- [ ] Soporte para aristas ponderadas y no ponderadas
- [ ] Consultar vecinos, grado, número de vértices/aristas

## Algoritmos de recorrido
- [ ] BFS (amplitud)
- [ ] DFS (profundidad)

## Camino más corto
- [ ] BFS para grafos no ponderados
- [ ] Dijkstra
- [ ] Bellman-Ford (aristas negativas)
- [ ] Floyd-Warshall (todos los pares)

## Otras funciones relevantes
- [ ] Detección de ciclos
- [ ] Verificación de conectividad / componentes conexas
- [ ] Grafo conectado o fuertemente conexo
- [ ] Orden topológico (si aplica)
- [ ] Árbol de expansión mínima (Kruskal/Prim) — opcional

## Robustez / resistencia a fallos
- [ ] Validación de índices fuera de rango (out of bounds)
- [ ] Manejo de aristas/vértices inexistentes
- [ ] Detección de overflow en pesos/sumas
- [ ] Uso seguro de memoria (sin fugas, manejo de `new`/`delete` o RAII)
- [ ] Pruebas con casos límite (grafo vacío, 1 nodo, nodos desconectados)

## Pruebas y verificación
- [ ] Programa de prueba/demo en `main.cpp`
- [ ] Test cases de cada algoritmo y de cada implementación
- [ ] Compilar sin warnings y correr con detectores (ej. `-fsanitize=address,undefined` en GCC/Clang)

## Documentación y entrega
- [ ] `README.md` con ejemplos de uso ("incluir y usar")
- [ ] Comentarios/doxygen en la API
- [ ] Commits con formato **Conventional Commits** (ej. `feat: add adjacency list`, `fix: validate out of bounds`)
- [ ] Subir versión final a GitHub
