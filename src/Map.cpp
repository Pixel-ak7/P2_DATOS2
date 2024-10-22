#include "Map.h"
#include <cstdlib>
#include <ctime>

// Constructor del mapa
// Qué sucede: Inicializa el tamaño del mapa y llena la cuadrícula sin obstáculos.
// Por qué sucede: Se necesita un mapa vacío inicialmente antes de agregar obstáculos.
// Qué deberíamos esperar: Un mapa de tamaño especificado sin obstáculos.
Map::Map(int size) : size(size), grid(size, std::vector<bool>(size, false)) {
    std::srand(std::time(nullptr));
}

// Método para generar obstáculos en el mapa
// Qué sucede: Llena el mapa con obstáculos según el porcentaje especificado.
// Por qué sucede: Agregar obstáculos para crear un desafío durante la navegación de los tanques.
// Qué deberíamos esperar: Obstáculos aleatorios en el mapa según el porcentaje especificado.
void Map::generateObstacles(int percentage) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (std::rand() % 100 < percentage) {
                grid[i][j] = true;
            }
        }
    }
}

// Verificar si una celda es un obstáculo
// Qué sucede: Comprueba si una celda en el mapa es un obstáculo.
// Por qué sucede: Para impedir que los tanques o balas atraviesen los obstáculos.
// Qué deberíamos esperar: True si la celda es un obstáculo, false si no lo es.
bool Map::isObstacle(int x, int y) const {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        return false;
    }
    return grid[x][y];
}

// Verificar si la posición es válida
// Qué sucede: Comprueba si una posición está dentro del límite y no es un obstáculo.
// Por qué sucede: Para asegurar que solo se pueda mover a celdas válidas.
// Qué deberíamos esperar: True si la posición es válida, false si no.
bool Map::isValidPosition(int x, int y) const {
    return x >= 0 && x < size && y >= 0 && y < size && !grid[x][y];
}

// Sobrecarga para verificar la validez de una posición considerando los tanques
bool Map::isValidPosition(int x, int y, const std::vector<Tank>& tanks) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    
    for (const Tank& tank : tanks) {
        if (tank.getX() == x && tank.getY() == y) {
            return false;
        }
    }
    return true;
}

int Map::getSize() const {
    return size;
}

// Dibujar el mapa en la ventana de juego
// Qué sucede: Dibuja cada celda del mapa y representa los obstáculos en negro.
// Por qué sucede: Para mostrar el mapa con todos los obstáculos y celdas vacías.
// Qué deberíamos esperar: Un mapa dibujado en la ventana con las celdas adecuadas.
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
