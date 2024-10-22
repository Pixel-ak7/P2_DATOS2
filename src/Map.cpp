#include "Map.h"
#include <cstdlib>
#include <ctime>

// Constructor del mapa
// Qué sucede: Inicializa el mapa, la matriz de obstáculos y la matriz de adyacencia.
// Por qué sucede: Se asegura de que el mapa comience vacío y que todas las celdas estén bien definidas.
Map::Map(int size) 
    : size(size), grid(size, std::vector<bool>(size, false)), adjacencyMatrix(size * size, std::vector<int>(size * size, 0)) {
    std::srand(std::time(nullptr));
    initializeAdjacencyMatrix();  // Inicializamos la matriz de adyacencia
}

// Generar obstáculos en el mapa
// Qué sucede: Genera obstáculos de manera aleatoria en la matriz `grid` según el porcentaje especificado.
// Por qué sucede: Los obstáculos crean desafíos adicionales en la navegación de los tanques.
void Map::generateObstacles(int percentage) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (std::rand() % 100 < percentage) {
                grid[i][j] = true; // Hay un obstáculo
            }
        }
    }
}

// Verificar si una celda tiene un obstáculo
// Qué sucede: Devuelve `true` si la celda en las coordenadas `x, y` es un obstáculo.
// Por qué sucede: Permite determinar si la celda es accesible o no.
bool Map::isObstacle(int x, int y) const {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        return false;
    }
    return grid[x][y];
}

// Verificar si una posición es válida
// Qué sucede: Verifica si la celda está dentro de los límites y no contiene un obstáculo.
// Por qué sucede: Evita movimientos fuera de los límites o a celdas ocupadas por obstáculos.
bool Map::isValidPosition(int x, int y) const {
    return x >= 0 && x < size && y >= 0 && y < size && !grid[x][y];
}

// Verificar si una posición es válida y libre de obstáculos y tanques
// Qué sucede: Verifica si la celda está dentro de los límites y no contiene un obstáculo o un tanque.
// Por qué sucede: Permite garantizar que una celda esté completamente libre antes de mover un tanque.
bool Map::isValidPosition(int x, int y, const std::vector<Tank>& tanks) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    for (const Tank& tank : tanks) {
        if (tank.getX() == x && tank.getY() == y) {
            return false; // Hay un tanque en la posición
        }
    }
    return true;
}

// Obtener el tamaño del mapa
// Qué sucede: Devuelve el tamaño de la matriz.
// Por qué sucede: Facilita el cálculo de los límites del mapa en otras partes del código.
int Map::getSize() const {
    return size;
}

// Dibujar el mapa en la ventana
// Qué sucede: Dibuja cada celda del mapa, diferenciando entre celdas libres y celdas con obstáculos.
// Por qué sucede: Para proporcionar una representación visual del estado del mapa en el juego.
void Map::draw(sf::RenderWindow& window, int cellSize) const {
    sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));
    cellShape.setFillColor(sf::Color::White);
    cellShape.setOutlineThickness(1);
    cellShape.setOutlineColor(sf::Color::Black);

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            cellShape.setPosition(x * cellSize, y * cellSize);
            if (grid[x][y]) {
                cellShape.setFillColor(sf::Color::Black);  // Representar obstáculos
            } else {
                cellShape.setFillColor(sf::Color::White);
            }
            window.draw(cellShape);
        }
    }
}

// Inicializar la matriz de adyacencia
// Qué sucede: Establece las relaciones de adyacencia entre las celdas que no tienen obstáculos.
// Por qué sucede: Permite modelar el mapa como un grafo que se utiliza para la búsqueda de rutas.
void Map::initializeAdjacencyMatrix() {
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            int currentIndex = cellIndex(x, y);
            if (!isObstacle(x, y)) {
                // Verificar celdas adyacentes (arriba, abajo, izquierda, derecha)
                if (isValidPosition(x + 1, y)) adjacencyMatrix[currentIndex][cellIndex(x + 1, y)] = 1;
                if (isValidPosition(x - 1, y)) adjacencyMatrix[currentIndex][cellIndex(x - 1, y)] = 1;
                if (isValidPosition(x, y + 1)) adjacencyMatrix[currentIndex][cellIndex(x, y + 1)] = 1;
                if (isValidPosition(x, y - 1)) adjacencyMatrix[currentIndex][cellIndex(x, y - 1)] = 1;
            }
        }
    }
}

// Verificar si dos celdas son adyacentes
// Qué sucede: Devuelve `true` si dos celdas son adyacentes en el grafo según la matriz de adyacencia.
// Por qué sucede: Facilita la verificación de conectividad entre celdas para el pathfinding.
bool Map::areCellsAdjacent(int x1, int y1, int x2, int y2) const {
    int index1 = cellIndex(x1, y1);
    int index2 = cellIndex(x2, y2);
    return adjacencyMatrix[index1][index2] == 1;
}

// Convertir coordenadas de celda a índice de la matriz de adyacencia
// Qué sucede: Convierte las coordenadas `x, y` a un índice lineal.
// Por qué sucede: Permite acceder a la matriz de adyacencia usando índices en lugar de coordenadas 2D.
int Map::cellIndex(int x, int y) const {
    return y * size + x;  // Índice lineal para una celda en una matriz 2D
}
