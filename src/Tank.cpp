#include "Tank.h"

// Constructor del tanque
// Qué sucede: Inicializa el tanque con un color, posición, ID y vida.
// Por qué sucede: Permite crear un tanque con características específicas.
// Qué deberíamos esperar: Un tanque ubicado en una posición dada con 100 de vida.
Tank::Tank(Color color, int x, int y, int id) 
    : x(x), y(y), color(color), health(100), id(id) {}

// Dibujar el tanque en la ventana de juego
// Qué sucede: Dibuja el tanque y su barra de vida en la ventana.
// Por qué sucede: Para representar visualmente el tanque y su estado actual.
// Qué deberíamos esperar: El tanque dibujado en la posición correspondiente con su barra de vida.
void Tank::draw(sf::RenderWindow& window, int cellSize) const {
    sf::RectangleShape tankShape(sf::Vector2f(cellSize, cellSize));
    tankShape.setPosition(x * cellSize, y * cellSize);

    switch (color) {
        case BLUE:    tankShape.setFillColor(sf::Color::Blue); break;
        case RED:     tankShape.setFillColor(sf::Color::Red); break;
        case CYAN:    tankShape.setFillColor(sf::Color::Cyan); break;
        case YELLOW:  tankShape.setFillColor(sf::Color::Yellow); break;
    }

    window.draw(tankShape);

    // Dibujar la barra de vida
    sf::RectangleShape healthBarBackground(sf::Vector2f(cellSize, 5));
    healthBarBackground.setPosition(x * cellSize, y * cellSize - 8);
    healthBarBackground.setFillColor(sf::Color::Red);

    float healthPercentage = static_cast<float>(health) / 100.0f;
    sf::RectangleShape healthBar(sf::Vector2f(cellSize * healthPercentage, 5));
    healthBar.setPosition(x * cellSize, y * cellSize - 8);
    healthBar.setFillColor(sf::Color::Green);

    window.draw(healthBarBackground);
    window.draw(healthBar);
}

// Aplicar daño al tanque
// Qué sucede: Reduce la vida del tanque según la cantidad de daño recibido.
// Por qué sucede: Los tanques deben perder vida cuando son impactados.
// Qué deberíamos esperar: La vida del tanque se reduce hasta un mínimo de 0.
void Tank::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

// Verificar si el tanque ha sido destruido
// Qué sucede: Comprueba si la vida del tanque ha llegado a 0.
// Por qué sucede: Determina si el tanque debe ser removido del juego.
// Qué deberíamos esperar: True si el tanque está destruido, false si aún tiene vida.
bool Tank::isDestroyed() const {
    return health <= 0;
}
