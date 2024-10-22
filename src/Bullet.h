#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Tank.h"
#include <vector>

// Clase Bullet para representar la bala disparada por un tanque
class Bullet {
public:
    // Constructor de la clase Bullet
    // Qué sucede: Inicializa una bala con una posición inicial, un objetivo y la dirección calculada.
    // Por qué sucede: Permite crear una bala con una trayectoria definida entre el origen (tanque) y el objetivo.
    // Qué deberíamos esperar: La bala se crea con las coordenadas iniciales y una dirección hacia el objetivo.
    Bullet(int startX, int startY, int targetX, int targetY, int shooterId);

    // Método para actualizar la posición de la bala
    // Qué sucede: Actualiza la posición de la bala según su dirección y velocidad, y verifica colisiones con tanques y obstáculos.
    // Por qué sucede: La bala debe moverse en cada frame y destruirse si colisiona con un obstáculo o tanque.
    // Qué deberíamos esperar: La bala se mueve hacia adelante, y `destroyBullet` se establece en true si debe ser eliminada.
    void update(const Map& map, std::vector<Tank>& tanks, bool& destroyBullet);

    // Método para dibujar la bala
    // Qué sucede: Dibuja la bala en su posición actual sobre la ventana.
    // Por qué sucede: La bala debe ser visible para los jugadores mientras se mueve.
    // Qué deberíamos esperar: La bala se dibuja en la ventana en su posición actual.
    void draw(sf::RenderWindow& window, int cellSize);

private:
    float posX, posY;  // Posición de la bala
    // Qué sucede: Define las coordenadas de la posición actual de la bala.
    // Por qué sucede: Necesitamos saber en qué lugar del mapa se encuentra la bala.
    // Qué deberíamos esperar: `posX` y `posY` indican la ubicación de la bala en el mapa.

    float dirX, dirY;  // Dirección de la bala
    // Qué sucede: Define la dirección normalizada en la que se mueve la bala.
    // Por qué sucede: Para mover la bala correctamente hacia su objetivo, necesitamos la dirección.
    // Qué deberíamos esperar: `dirX` y `dirY` contienen los valores que determinan el movimiento en el eje x e y.

    float speed = 0.2f;  // Velocidad de la bala
    // Qué sucede: Establece la velocidad constante a la que se mueve la bala.
    // Por qué sucede: Necesitamos definir la rapidez con la que la bala se mueve en cada frame.
    // Qué deberíamos esperar: La bala se mueve a una velocidad constante de `0.2` unidades por frame.

    int shooterId;  // ID del tanque que disparó la bala
    // Qué sucede: Almacena el ID del tanque que disparó la bala.
    // Por qué sucede: Necesitamos saber quién disparó la bala para evitar que la colisión afecte al tanque que la disparó.
    // Qué deberíamos esperar: `shooterId` identifica el tanque que disparó, evitando daños por auto-colisión.
};

#endif
