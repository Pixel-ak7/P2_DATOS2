#ifndef TANK_H
#define TANK_H

#include <SFML/Graphics.hpp>

// Clase Tank que representa un tanque en el juego
class Tank {
public:
    enum Color {
        BLUE,
        RED,
        CYAN,
        YELLOW
    };

    // Constructor y métodos del tanque
    // Qué sucede: Definen cómo se comportan los tanques (mover, dibujar, recibir daño).
    // Por qué sucede: Necesitamos representar la funcionalidad básica del tanque en el juego.
    // Qué deberíamos esperar: Un tanque que puede moverse, recibir daño y dibujarse en pantalla.
    Tank(Color color, int x, int y, int id);
    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int x, int y) { this->x = x; this->y = y; }
    Color getColor() const { return color; }
    int getHealth() const { return health; }
    void draw(sf::RenderWindow &window, int cellSize) const;
    void takeDamage(int damage);
    bool isDestroyed() const;
    int getId() const { return id; }

private:
    int x, y;  // Posición del tanque en el mapa
    Color color;  // Color del tanque
    int health;  // Vida del tanque (100 por defecto)
    int id;  // ID único del tanque
};

#endif
