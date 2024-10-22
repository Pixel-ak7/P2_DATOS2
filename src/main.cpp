#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Tank.h"
#include "Pathfinding.h"
#include "Bullet.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <algorithm>
#include <climits>

// Función para generar un número aleatorio entre dos valores
// Qué sucede: Genera un número aleatorio entre `min` y `max`.
// Por qué sucede: Se utiliza para colocar tanques o generar otros elementos aleatoriamente en el mapa.
// Qué deberíamos esperar: Un número aleatorio dentro del rango especificado.
int getRandomPosition(int min, int max) {
    return std::rand() % (max - min + 1) + min;
}

// Función para verificar si una celda está ocupada por un tanque
// Qué sucede: Revisa si un tanque está en la posición dada.
// Por qué sucede: Es importante para evitar colisiones y asegurarse de que los tanques no ocupen la misma posición.
// Qué deberíamos esperar: `true` si la posición está ocupada por un tanque, `false` si está libre.
bool isPositionOccupied(int x, int y, const std::vector<Tank>& tanks) {
    for (const Tank& tank : tanks) {
        if (tank.getX() == x && tank.getY() == y) {
            return true;
        }
    }
    return false;
}

// Función para contar los tanques vivos de un jugador según su color
// Qué sucede: Cuenta cuántos tanques de un color específico están aún activos.
// Por qué sucede: Para determinar si un jugador ha perdido todos sus tanques.
// Qué deberíamos esperar: El número de tanques vivos de un jugador.
int countAliveTanks(const std::vector<Tank>& tanks, const std::vector<Tank::Color>& playerColors) {
    int count = 0;
    for (const Tank& tank : tanks) {
        if (!tank.isDestroyed() && std::find(playerColors.begin(), playerColors.end(), tank.getColor()) != playerColors.end()) {
            count++;
        }
    }
    return count;
}

// Enumeración de los power-ups
// Qué sucede: Se definen los diferentes tipos de power-ups disponibles en el juego.
// Por qué sucede: Para permitir efectos especiales que los jugadores puedan utilizar durante el juego.
enum PowerUp {
    NONE,
    DOUBLE_TURN,
    MOVE_PRECISION,
    ATTACK_PRECISION,
    ATTACK_POWER
};

// Control del número de turnos adicionales por power-up de doble turno
int turnControl[2] = {0, 0};  // `turnControl[0]` para jugador 1, `turnControl[1]` para jugador 2

int main() {
    // Inicializar la semilla de números aleatorios
    std::srand(std::time(nullptr));

    // Dimensiones del mapa
    const int mapSize = 20;  // Tamaño del mapa (20x20)
    const int cellSize = 30; // Tamaño de cada celda (en píxeles)

    // Crear ventana del juego
    // Qué sucede: Se crea una ventana para mostrar el juego.
    // Por qué sucede: La ventana es la interfaz principal donde se desarrolla el juego.
    // Qué deberíamos esperar: Una ventana gráfica que representa el campo de juego.
    sf::RenderWindow window(sf::VideoMode(mapSize * cellSize, mapSize * cellSize + 50), "Tank Attack!");

    // Cargar la fuente para los textos del juego
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf")) {
        std::cerr << "Error cargando la fuente\n";
        return -1; // Termina el programa si no se encuentra la fuente
    }

    // Crear el texto para el contador de turnos y el temporizador global
    // Qué sucede: Se inicializan los elementos de texto para mostrar el estado del juego.
    // Por qué sucede: Se proporciona retroalimentación visual a los jugadores (turnos y tiempo).
    sf::Text turnText;
    turnText.setFont(font);
    turnText.setCharacterSize(24);
    turnText.setFillColor(sf::Color::Black);
    turnText.setPosition(10, mapSize * cellSize);

    sf::Text globalTimerText;
    globalTimerText.setFont(font);
    globalTimerText.setCharacterSize(24);
    globalTimerText.setFillColor(sf::Color::Black);
    globalTimerText.setPosition(window.getSize().x - 180, mapSize * cellSize);

    sf::Text powerUpText;
    powerUpText.setFont(font);
    powerUpText.setCharacterSize(24);
    powerUpText.setFillColor(sf::Color::Black);
    powerUpText.setPosition(10, mapSize * cellSize + 25);

    // Crear el mapa y generar obstáculos
    // Qué sucede: Se inicializa el mapa y se colocan obstáculos en celdas aleatorias.
    // Por qué sucede: Los obstáculos añaden dificultad y estrategia al movimiento de los tanques.
    Map gameMap(mapSize);
    gameMap.generateObstacles(10);  // Generar con un 10% de obstáculos

    // Crear los tanques del jugador 1 y del jugador 2
    // Qué sucede: Se añaden los tanques de cada jugador a la lista de tanques.
    // Por qué sucede: Cada jugador debe tener sus tanques representados en el mapa.
    std::vector<Tank> tanks;

    // Añadir tanques para el jugador 1 (colores azul y rojo)
    for (int i = 0; i < 2; ++i) {
        int x, y;
        do {
            x = getRandomPosition(0, mapSize / 2 - 1);
            y = getRandomPosition(0, mapSize - 1);
        } while (isPositionOccupied(x, y, tanks));
        tanks.emplace_back(Tank::BLUE, x, y, i);  // Añadir tanque azul con ID único
    }
    for (int i = 0; i < 2; ++i) {
        int x, y;
        do {
            x = getRandomPosition(0, mapSize / 2 - 1);
            y = getRandomPosition(0, mapSize - 1);
        } while (isPositionOccupied(x, y, tanks));
        tanks.emplace_back(Tank::RED, x, y, i + 2);  // Añadir tanque rojo con ID único
    }

    // Añadir tanques para el jugador 2 (colores celeste y amarillo)
    for (int i = 0; i < 2; ++i) {
        int x, y;
        do {
            x = getRandomPosition(mapSize / 2, mapSize - 1);
            y = getRandomPosition(0, mapSize - 1);
        } while (isPositionOccupied(x, y, tanks));
        tanks.emplace_back(Tank::CYAN, x, y, i + 4);  // Añadir tanque celeste con ID único
    }
    for (int i = 0; i < 2; ++i) {
        int x, y;
        do {
            x = getRandomPosition(mapSize / 2, mapSize - 1);
            y = getRandomPosition(0, mapSize - 1);
        } while (isPositionOccupied(x, y, tanks));
        tanks.emplace_back(Tank::YELLOW, x, y, i + 6);  // Añadir tanque amarillo con ID único
    }

    // Inicializar variables de control para el juego
    int currentPlayer = 1;  // Jugador actual (1 o 2)
    Tank* selectedTank = nullptr;  // Puntero al tanque seleccionado por el jugador
    bool waitingForBFSClick = false;  // Indica si estamos esperando un clic para el movimiento con BFS
    bool waitingForDijkstraClick = false;  // Indica si estamos esperando un clic para el movimiento con Dijkstra
    bool powerUsed = false;  // Indica si el jugador ya usó un poder en este turno
    char selectedPower = '\0';  // Poder seleccionado ('M', 'D', 'P'), `\0` si no se ha seleccionado ninguno
    std::vector<Cell> currentPath;  // Almacena la ruta calculada del tanque seleccionado
    sf::Clock globalClock;  // Temporizador global para el tiempo total del juego
    sf::Clock turnClock;  // Temporizador para controlar la duración de cada turno

    // Variables para el modo disparo
    bool isShootingMode = false;  // Indica si el modo disparo está activado
    Bullet* activeBullet = nullptr;  // Puntero a la bala activa en el juego
    bool hasShot = false;  // Indica si el tanque ya disparó en el turno actual

    // Variables para el sistema de power-ups
    PowerUp playerPowerUp[2] = { NONE, NONE };  // Power-ups asignados a cada jugador
    bool isPowerUpActive = false;  // Indica si un power-up está activo
    bool powerUpActivated = false;  // Indica si un power-up fue activado en el turno actual
    bool powerUpConsumed = false;  // Indica si el power-up fue consumido

    // Definir colores de tanques para cada jugador
    std::vector<Tank::Color> player1Colors = {Tank::BLUE, Tank::RED};
    std::vector<Tank::Color> player2Colors = {Tank::CYAN, Tank::YELLOW};

    // Bucle principal del juego
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Cerrar la ventana si se presiona el botón de cierre
            if (event.type == sf::Event::Closed)
                window.close();

            // Detectar clic en un tanque para seleccionarlo
            // Qué sucede: El jugador puede seleccionar un tanque para moverlo o atacar.
            // Por qué sucede: Cada turno, un jugador debe poder seleccionar y mover sus tanques.
            // Qué deberíamos esperar: El tanque seleccionado se indica visualmente y está listo para moverse.
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mouseX = event.mouseButton.x / cellSize;
                int mouseY = event.mouseButton.y / cellSize;

                if (waitingForBFSClick && selectedTank != nullptr) {
                    // Mover el tanque usando BFS si se hace clic en un destino válido
                    if (gameMap.isValidPosition(mouseX, mouseY) && !isPositionOccupied(mouseX, mouseY, tanks)) {
                        currentPath = bfs(gameMap, selectedTank->getX(), selectedTank->getY(), mouseX, mouseY, tanks);
                        waitingForBFSClick = false;  // Terminar la espera para el clic
                    }
                } else if (waitingForDijkstraClick && selectedTank != nullptr) {
                    // Mover el tanque usando Dijkstra si se hace clic en un destino válido
                    if (gameMap.isValidPosition(mouseX, mouseY) && !isPositionOccupied(mouseX, mouseY, tanks)) {
                        currentPath = dijkstra(gameMap, selectedTank->getX(), selectedTank->getY(), mouseX, mouseY, tanks);
                        waitingForDijkstraClick = false;  // Terminar la espera para el clic
                    }
                } else if (selectedTank == nullptr) {
                    // Seleccionar un tanque si no estamos esperando para BFS o Dijkstra
                    for (Tank& tank : tanks) {
                        if ((currentPlayer == 1 && (tank.getColor() == Tank::BLUE || tank.getColor() == Tank::RED)) ||
                            (currentPlayer == 2 && (tank.getColor() == Tank::CYAN || tank.getColor() == Tank::YELLOW))) {
                            if (tank.getX() == mouseX && tank.getY() == mouseY) {
                                selectedTank = &tank;  // Selecciona el tanque
                                std::cout << "Tanque seleccionado en (" << tank.getX() << ", " << tank.getY() << ")\n";
                                break;
                            }
                        }
                    }
                }
            }

            // Detectar la tecla M para activar el movimiento del tanque
            // Qué sucede: Permite al jugador mover el tanque seleccionado.
            // Por qué sucede: Los tanques deben ser capaces de moverse en el campo de batalla.
            if (event.type == sf::Event::KeyPressed && selectedTank != nullptr && !powerUsed) {
                if (event.key.code == sf::Keyboard::M && selectedPower == '\0') {
                    selectedPower = 'M';
                    powerUsed = true;
                    if (selectedTank->getColor() == Tank::BLUE || selectedTank->getColor() == Tank::CYAN) {
                        int randomDecision = std::rand() % 2;
                        if (randomDecision == 0) {
                            std::cout << "Usando BFS para mover tanque azul/celeste\n";
                            waitingForBFSClick = true;  // Esperar clic para definir destino
                        } else {
                            std::cout << "Usando movimiento aleatorio para tanque azul/celeste\n";
                            currentPath = moveRandomly(selectedTank->getX(), selectedTank->getY(), gameMap, tanks);
                        }
                    } else if (selectedTank->getColor() == Tank::RED || selectedTank->getColor() == Tank::YELLOW) {
                        int randomDecision = std::rand() % 10;
                        if (randomDecision < 8) {
                            std::cout << "Usando Dijkstra para mover tanque rojo/amarillo\n";
                            waitingForDijkstraClick = true;  // Esperar clic para definir destino
                        } else {
                            std::cout << "Usando movimiento aleatorio para tanque rojo/amarillo\n";
                            currentPath = moveRandomly(selectedTank->getX(), selectedTank->getY(), gameMap, tanks);
                        }
                    }
                } 
            }

            // Detectar la tecla D para activar el modo disparo del tanque
            // Qué sucede: Permite al tanque disparar hacia un objetivo.
            // Por qué sucede: Los tanques necesitan atacar para eliminar a los enemigos.
            // Qué deberíamos esperar: Activación del modo disparo y selección de un objetivo para atacar.
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D && selectedTank != nullptr && !powerUsed && !powerUpActivated) {
                if (selectedPower == '\0') {
                    selectedPower = 'D';
                    powerUsed = true;
                    isShootingMode = true;  // Activar el modo disparo
                    std::cout << "Modo disparo activado\n";
                }
            }

            // Detectar clic en el objetivo durante el modo disparo
            // Qué sucede: El tanque dispara hacia la posición seleccionada.
            // Por qué sucede: El disparo permite eliminar tanques enemigos.
            // Qué deberíamos esperar: Creación de una bala que viaja hacia el objetivo.
            if (isShootingMode && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !hasShot) {
                int targetX = event.mouseButton.x / cellSize;
                int targetY = event.mouseButton.y / cellSize;

                activeBullet = new Bullet(selectedTank->getX(), selectedTank->getY(), targetX, targetY, selectedTank->getId());

                // Salir del modo disparo y marcar que se ha disparado en este turno
                isShootingMode = false;
                hasShot = true;
            }

            // Detectar la tecla P para activar un power-up
            // Qué sucede: Se activa el power-up del jugador actual si está disponible.
            // Por qué sucede: Los power-ups permiten obtener ventajas estratégicas durante el juego.
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P && !powerUsed) {
                if (playerPowerUp[currentPlayer - 1] != NONE && !powerUpConsumed) {
                    isPowerUpActive = true;
                    powerUpActivated = true;
                    powerUpConsumed = true;
                    std::cout << "Power-up activado: " << playerPowerUp[currentPlayer - 1] << "\n";
                }
            }
        }

        // Lógica para asignar power-ups aleatoriamente
        // Qué sucede: Cada turno, hay una probabilidad del 30% de recibir un power-up.
        // Por qué sucede: Añade un elemento de sorpresa y estrategia al juego.
        if (!isPowerUpActive && std::rand() % 100 < 30) {
            playerPowerUp[currentPlayer - 1] = static_cast<PowerUp>(std::rand() % 4 + 1);
        }

        // Mostrar el power-up actual en la pantalla
        std::string powerUpName;
        switch (playerPowerUp[currentPlayer - 1]) {
            case DOUBLE_TURN: powerUpName = "Doble Turno"; break;
            case MOVE_PRECISION: powerUpName = "Precisión de Movimiento"; break;
            case ATTACK_PRECISION: powerUpName = "Precisión de Ataque"; break;
            case ATTACK_POWER: powerUpName = "Poder de Ataque"; break;
            default: powerUpName = "Ninguno"; break;
        }
        powerUpText.setString("Power-up: " + powerUpName);

        // Actualizar la bala en cada frame
        // Qué sucede: Mueve la bala y verifica si impacta un tanque o el borde del mapa.
        // Por qué sucede: Para simular el movimiento de la bala después de un disparo.
        if (activeBullet != nullptr) {
            bool destroyBullet = false;
            activeBullet->update(gameMap, tanks, destroyBullet);
            if (destroyBullet) {
                delete activeBullet;
                activeBullet = nullptr;
            }
        }

        // Remover tanques destruidos del vector de tanques
        tanks.erase(std::remove_if(tanks.begin(), tanks.end(),
            [](const Tank& tank) { return tank.isDestroyed(); }), tanks.end());

        // Actualizar el texto del turno y el temporizador global
        int remainingTime = 300 - globalClock.getElapsedTime().asSeconds();  // Tiempo restante en segundos
        globalTimerText.setString("Tiempo: " + std::to_string(remainingTime / 60) + ":" + std::to_string(remainingTime % 60));
        turnText.setString("Turno del Jugador: " + std::to_string(currentPlayer));

        // Verificar si el tiempo se ha terminado o si un jugador ha eliminado todos los tanques del oponente
        int player1TanksAlive = countAliveTanks(tanks, player1Colors);
        int player2TanksAlive = countAliveTanks(tanks, player2Colors);

        if (remainingTime <= 0 || player1TanksAlive == 0 || player2TanksAlive == 0) {
            // Declarar al ganador
            if (player1TanksAlive > player2TanksAlive) {
                std::cout << "Jugador 1 gana con " << player1TanksAlive << " tanques vivos.\n";
            } else if (player2TanksAlive > player1TanksAlive) {
                std::cout << "Jugador 2 gana con " << player2TanksAlive << " tanques vivos.\n";
            } else {
                std::cout << "Empate, ambos jugadores tienen la misma cantidad de tanques vivos.\n";
            }
            window.close();  // Cerrar el juego
        }

        // Cambiar de turno cada 15 segundos o cuando se cumplan turnos adicionales por power-up
        if (turnClock.getElapsedTime().asSeconds() >= 15.0f || turnControl[currentPlayer - 1] > 0) {
            if (turnControl[currentPlayer - 1] > 0) {
                turnControl[currentPlayer - 1]--;  // Reducir turnos adicionales si existen
            } else {
                currentPlayer = (currentPlayer == 1) ? 2 : 1;  // Cambiar al otro jugador
            }

            turnClock.restart();
            powerUsed = false;
            selectedPower = '\0';
            selectedTank = nullptr;
            currentPath.clear();
            hasShot = false;
            powerUpActivated = false;  // Reiniciar el estado de power-up
            powerUpConsumed = false;

            // Limpiar bala activa al final del turno
            delete activeBullet;
            activeBullet = nullptr;
        }

        // Limpiar la ventana antes de dibujar el siguiente frame
        window.clear(sf::Color::White);

        // Dibujar el mapa y los tanques
        gameMap.draw(window, cellSize);
        for (Tank& tank : tanks) {
            tank.draw(window, cellSize);
        }

        // Dibujar la ruta planificada en verde si se calculó una ruta
        if (!currentPath.empty()) {
            for (const Cell& cell : currentPath) {
                sf::RectangleShape pathRect(sf::Vector2f(cellSize, cellSize));
                pathRect.setPosition(cell.x * cellSize, cell.y * cellSize);
                pathRect.setFillColor(sf::Color::Green);
                window.draw(pathRect);
            }
        }

        // Mover el tanque seleccionado según la ruta calculada
        if (!currentPath.empty() && selectedTank != nullptr) {
            Cell nextMove = currentPath.front();
            currentPath.erase(currentPath.begin());
            selectedTank->setPosition(nextMove.x, nextMove.y);
        }

        // Dibujar la bala si hay una activa
        if (activeBullet != nullptr) {
            activeBullet->draw(window, cellSize);
        }

        // Dibujar el texto del turno, el temporizador global y el power-up actual
        window.draw(turnText);
        window.draw(globalTimerText);
        window.draw(powerUpText);

        // Mostrar el contenido dibujado en la ventana
        window.display();
    }

    return 0;
}
