#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Tank.h"

// Clase Map que representa el mapa del juego
class Map {
public:
    Map(int size);  // Constructor del mapa
    void generateObstacles(int percentage);  // Generar obstáculos en el mapa
    bool isObstacle(int x, int y) const;  // Verificar si una celda es un obstáculo
    bool isValidPosition(int x, int y) const;  // Verificar si la posición es válida sin obstáculos
    bool isValidPosition(int x, int y, const std::vector<Tank>& tanks) const;  // Verificar si la posición es válida sin obstáculos ni tanques

    int getSize() const;  // Obtener el tamaño del mapa
    void draw(sf::RenderWindow& window, int cellSize) const;  // Dibujar el mapa en la ventana

private:
    int size;  // Tamaño del mapa
    std::vector<std::vector<bool>> grid;  // Representación del mapa con obstáculos
};

#endif
