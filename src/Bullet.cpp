#include "Bullet.h"
#include <cmath>
#include <cstdlib>  // Para rand()

// Función que verifica si la línea de vista está despejada usando un algoritmo básico de raycasting
// Qué sucede: Calcula si hay una trayectoria directa y sin obstáculos entre dos puntos (inicio y objetivo).
// Por qué sucede: Para determinar si la bala tiene un camino despejado sin colisiones con obstáculos.
// Qué deberíamos esperar: Devuelve `true` si la línea de vista está despejada, `false` si hay obstáculos.
bool isLineOfSightClear(int x1, int y1, int x2, int y2, const Map& map) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = std::max(std::abs(dx), std::abs(dy));

    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    float currentX = x1;
    float currentY = y1;

    for (int i = 0; i < steps; i++) {
        if (map.isObstacle(static_cast<int>(currentX), static_cast<int>(currentY))) {
            return false;  // Hay un obstáculo en la línea de vista
        }
        currentX += xIncrement;
        currentY += yIncrement;
    }

    return true;  // No se encontraron obstáculos, la línea de vista está despejada
}

// Constructor de la clase Bullet
// Qué sucede: Inicializa la bala con la posición inicial, objetivo, y calcula la dirección.
// Por qué sucede: Para que la bala se dirija del punto inicial al objetivo seleccionado.
// Qué deberíamos esperar: La bala se mueve desde la posición de disparo hacia el objetivo con la dirección calculada.
Bullet::Bullet(int startX, int startY, int targetX, int targetY, int shooterId)
    : posX(startX), posY(startY), shooterId(shooterId) {
    // Calcular la dirección
    float dx = targetX - startX;
    float dy = targetY - startY;
    float length = std::sqrt(dx * dx + dy * dy);
    dirX = dx / length;
    dirY = dy / length;
}

// Método para actualizar la bala
// Qué sucede: Mueve la bala, verifica colisiones y rebotes en obstáculos y bordes del mapa.
// Por qué sucede: La bala debe moverse hacia adelante y rebotar en obstáculos o tanques según las reglas del juego.
// Qué deberíamos esperar: La bala cambia su posición y rebota o se destruye si impacta contra un obstáculo o un tanque.
void Bullet::update(const Map& map, std::vector<Tank>& tanks, bool& destroyBullet) {
    // Verificar si la línea de vista está despejada
    // Qué sucede: Verifica si hay obstáculos en la trayectoria de la bala.
    // Por qué sucede: Para decidir si la bala puede continuar o si debe rebotar.
    // Qué deberíamos esperar: Si hay un obstáculo, la bala cambia su dirección (rebota).
    if (!isLineOfSightClear(static_cast<int>(posX), static_cast<int>(posY), static_cast<int>(posX + dirX), static_cast<int>(posY + dirY), map)) {
        // Si hay un obstáculo, rebota con un ángulo aleatorio
        // Qué sucede: Calcula una nueva dirección con un ángulo aleatorio de rebote.
        // Por qué sucede: Los rebotes hacen el comportamiento de la bala más dinámico e impredecible.
        // Qué deberíamos esperar: La dirección de la bala cambia ligeramente para simular un rebote.
        float randomAngle = (std::rand() % 90 - 45) * (M_PI / 180.0f);  // Convertir a radianes
        float newDirX = dirX * std::cos(randomAngle) - dirY * std::sin(randomAngle);
        float newDirY = dirX * std::sin(randomAngle) + dirY * std::cos(randomAngle);
        dirX = newDirX;
        dirY = newDirY;
    }

    // Actualizar la posición de la bala
    // Qué sucede: La bala se mueve en la dirección calculada con la velocidad establecida.
    // Por qué sucede: Para que la bala avance en cada actualización de frame.
    // Qué deberíamos esperar: La posición de la bala se actualiza según su velocidad y dirección.
    posX += dirX * speed;
    posY += dirY * speed;

    // Verificar si colisiona con algún tanque
    // Qué sucede: Se verifica si la bala impacta contra alguno de los tanques en la lista.
    // Por qué sucede: Si impacta contra un tanque, se aplica el daño y la bala se destruye.
    // Qué deberíamos esperar: Si colisiona con un tanque que no es el que disparó, la bala se destruye y el tanque recibe daño.
    for (Tank& tank : tanks) {
        if (tank.getX() == static_cast<int>(posX) && tank.getY() == static_cast<int>(posY)) {
            if (tank.getId() != shooterId) {
                // Aplicar el daño correcto según el tipo de tanque
                // Qué sucede: Dependiendo del color del tanque, recibe diferente cantidad de daño.
                // Por qué sucede: Los tanques de diferentes colores tienen resistencias distintas.
                // Qué deberíamos esperar: Los tanques azul/celeste reciben 25% de daño, y los rojo/amarillo 50%.
                switch (tank.getColor()) {
                    case Tank::BLUE:
                    case Tank::CYAN:
                        tank.takeDamage(25);  // 25% de daño para azul/celeste
                        break;
                    case Tank::RED:
                    case Tank::YELLOW:
                        tank.takeDamage(50);  // 50% de daño para rojo/amarillo
                        break;
                }
                destroyBullet = true;  // Destruir la bala tras impactar
                return;
            }
        }
    }

    // Verificar si la bala sale de los bordes de la pantalla
    // Qué sucede: Verifica si la bala ha salido de los límites del mapa.
    // Por qué sucede: Si la bala llega a los bordes, debe rebotar en la dirección opuesta.
    // Qué deberíamos esperar: La bala cambia su dirección si alcanza los bordes del mapa.
    if (posX < 0 || posY < 0 || posX >= map.getSize() || posY >= map.getSize()) {
        // Rebote en los bordes de la pantalla
        // Qué sucede: La dirección de la bala se invierte si alcanza los bordes del mapa.
        if (posX < 0 || posX >= map.getSize()) {
            dirX = -dirX;  // Invertir dirección en el eje X
        }
        if (posY < 0 || posY >= map.getSize()) {
            dirY = -dirY;  // Invertir dirección en el eje Y
        }
    }
}

// Método para dibujar la bala
// Qué sucede: Dibuja la bala en su posición actual sobre la ventana de juego.
// Por qué sucede: Para mostrar visualmente el movimiento y la ubicación de la bala en el juego.
// Qué deberíamos esperar: La bala se dibuja en la ventana de juego en su posición actual.
void Bullet::draw(sf::RenderWindow& window, int cellSize) {
    sf::CircleShape bulletShape(cellSize / 6);  // Tamaño pequeño de la bala
    bulletShape.setFillColor(sf::Color::Black); // Color de la bala
    bulletShape.setPosition(posX * cellSize, posY * cellSize);
    window.draw(bulletShape);
}
