#ifndef MAP_H
#define MAP_H

#include <vector>
#include "Tank.h"

class Map {
public:
    // Constructor para inicializar el mapa con un tamaño específico.
    // Qué sucede: Crea un mapa de tamaño `size x size` y establece la matriz de obstáculos y adyacencia.
    // Por qué sucede: El mapa define el área de juego y permite establecer relaciones entre celdas.
    Map(int size);

    // Método para generar obstáculos en el mapa.
    // Qué sucede: Llena la matriz `grid` con obstáculos, con base en el porcentaje especificado.
    // Por qué sucede: Los obstáculos crean desafíos en el movimiento de los tanques.
    void generateObstacles(int percentage);

    // Verificar si una celda contiene un obstáculo.
    // Qué sucede: Devuelve `true` si la celda especificada es un obstáculo.
    // Por qué sucede: Para determinar si una posición es válida para que un tanque se mueva.
    bool isObstacle(int x, int y) const;

    // Verificar si una posición es válida y no contiene obstáculos.
    // Qué sucede: Devuelve `true` si la posición está dentro del límite y no es un obstáculo.
    // Por qué sucede: Permite a los tanques verificar si pueden moverse a una determinada celda.
    bool isValidPosition(int x, int y) const;

    // Verificar si una posición es válida y no contiene obstáculos ni tanques.
    // Qué sucede: Devuelve `true` si la posición es válida y no hay un tanque.
    // Por qué sucede: Evita colisiones entre tanques.
    bool isValidPosition(int x, int y, const std::vector<Tank>& tanks) const;

    // Obtener el tamaño del mapa.
    // Qué sucede: Devuelve el tamaño del mapa.
    // Por qué sucede: Es útil para límites y cálculos en otras partes del juego.
    int getSize() const;

    // Dibujar el mapa en la ventana.
    // Qué sucede: Dibuja cada celda, mostrando si es un obstáculo o no.
    // Por qué sucede: Representa visualmente el estado del mapa en la ventana de juego.
    void draw(sf::RenderWindow& window, int cellSize) const;

    // Inicializar la matriz de adyacencia.
    // Qué sucede: Calcula y establece las relaciones de adyacencia entre las celdas.
    // Por qué sucede: Para modelar el mapa como un grafo que puede ser utilizado en pathfinding.
    void initializeAdjacencyMatrix();

    // Verificar si dos celdas son adyacentes según la matriz de adyacencia.
    // Qué sucede: Devuelve `true` si dos celdas son adyacentes.
    // Por qué sucede: Ayuda en la lógica del grafo para navegación y búsqueda de rutas.
    bool areCellsAdjacent(int x1, int y1, int x2, int y2) const;

private:
    int size;  // Tamaño del mapa (cantidad de celdas en cada dimensión).
    std::vector<std::vector<bool>> grid;  // Matriz que indica la presencia de obstáculos en el mapa.
    std::vector<std::vector<int>> adjacencyMatrix;  // Matriz de adyacencia para representar el grafo.

    // Convertir coordenadas de celda en índice de la matriz de adyacencia.
    // Qué sucede: Calcula un índice lineal para una celda dada.
    // Por qué sucede: Para acceder eficientemente a las celdas en estructuras unidimensionales.
    int cellIndex(int x, int y) const;
};

#endif
