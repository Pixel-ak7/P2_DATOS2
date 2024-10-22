#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "Map.h"
#include "Tank.h"
#include <vector>

// Estructura que representa una celda del mapa
struct Cell {
    int x;
    int y;

    // Constructor predeterminado y sobrecarga del constructor
    // Qué sucede: Inicializa las coordenadas de la celda.
    // Por qué sucede: Se necesita un constructor para crear celdas con valores predeterminados o específicos.
    // Qué deberíamos esperar: Celdas con coordenadas (0,0) o con coordenadas especificadas.
    Cell() : x(0), y(0) {}
    Cell(int x_, int y_) : x(x_), y(y_) {}
};

// Funciones de búsqueda de rutas
// Qué sucede: Se definen tres funciones para mover tanques: BFS, movimiento aleatorio y Dijkstra.
// Por qué sucede: Cada uno de estos métodos tiene una utilidad específica para calcular la ruta de los tanques.
// Qué deberíamos esperar: Diferentes comportamientos de movimiento según el algoritmo seleccionado.
std::vector<Cell> bfs(const Map& map, int startX, int startY, int endX, int endY, const std::vector<Tank>& tanks);
std::vector<Cell> moveRandomly(int startX, int startY, const Map& map, const std::vector<Tank>& tanks);
std::vector<Cell> dijkstra(const Map& map, int startX, int startY, int endX, int endY, const std::vector<Tank>& tanks);

#endif
