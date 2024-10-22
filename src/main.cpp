// Incluye las librerías necesarias para gráficos, manejo de mapa, tanques y pathfinding
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
// Qué sucede: Genera un número aleatorio dentro de un rango dado.
// Por qué sucede: Se utiliza para ubicar aleatoriamente los tanques o elementos en el mapa.
// Qué deberíamos esperar: Un número aleatorio dentro del rango especificado.
int getRandomPosition(int min, int max) {
    return std::rand() % (max - min + 1) + min;
}

// Función para verificar si una celda está ocupada por un tanque
// Qué sucede: Recorre la lista de tanques para verificar si alguno ocupa la posición especificada.
// Por qué sucede: Evita colocar un tanque en una celda que ya está ocupada.
// Qué deberíamos esperar: Devuelve true si la celda está ocupada, false si está libre.
bool isPositionOccupied(int x, int y, const std::vector<Tank>& tanks) {
    for (const Tank& tank : tanks) {
        if (tank.getX() == x && tank.getY() == y) {
            return true;
        }
    }
    return false;
}

// Enumeración de los power-ups
// Qué sucede: Define los diferentes tipos de power-ups disponibles en el juego.
// Por qué sucede: Permite una fácil gestión de los power-ups asignándoles un nombre significativo.
// Qué deberíamos esperar: Una enumeración que indique distintos tipos de power-ups.
enum PowerUp {
    NONE,
    DOUBLE_TURN,
    MOVE_PRECISION,
    ATTACK_PRECISION,
    ATTACK_POWER
};

// Control del número de turnos adicionales por power-up de doble turno
// Qué sucede: Controla los turnos extra otorgados a cada jugador por el power-up de "doble turno".
// Por qué sucede: Gestiona cuántos turnos adicionales recibe cada jugador.
// Qué deberíamos esperar: Un contador para cada jugador (turnControl[0] y turnControl[1]).
int turnControl[2] = {0, 0};  // turnControl[0] para jugador 1, turnControl[1] para jugador 2

int main() {
    // Inicializar la semilla de números aleatorios
    // Qué sucede: Inicializa la semilla de generación aleatoria con la hora actual.
    // Por qué sucede: Asegura que los números generados sean diferentes cada vez que se ejecute el juego.
    // Qué deberíamos esperar: Una semilla basada en el tiempo para garantizar aleatoriedad.
    std::srand(std::time(nullptr));

    // Dimensiones del mapa
    const int mapSize = 20; // Tamaño del mapa (20x20)
    const int cellSize = 30; // Tamaño de cada celda en píxeles

    // Crear ventana del juego
    // Qué sucede: Se crea una ventana con un tamaño específico para mostrar el mapa y los elementos del juego.
    // Por qué sucede: Necesario para mostrar los gráficos del juego y permitir la interacción del usuario.
    // Qué deberíamos esperar: Una ventana de SFML que representa el campo de juego.
    sf::RenderWindow window(sf::VideoMode(mapSize * cellSize, mapSize * cellSize + 50), "Tank Attack!");

    // Cargar la fuente para los textos del juego
    // Qué sucede: Intenta cargar una fuente de texto desde un archivo específico.
    // Por qué sucede: La fuente se utiliza para mostrar los textos del turno, temporizador y power-ups en pantalla.
    // Qué deberíamos esperar: Fuente cargada correctamente; en caso de fallo, un mensaje de error.
    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf")) {
        std::cerr << "Error cargando la fuente\n";
        return -1;
    }

    // Crear textos para el contador de turnos, temporizador global y power-ups
    // Qué sucede: Se configuran varios textos para mostrar información en pantalla.
    // Por qué sucede: Es importante proporcionar información visual al jugador (turnos, tiempo, power-ups).
    // Qué deberíamos esperar: Textos inicializados con una fuente, tamaño y posición adecuados.
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
    // Qué sucede: Se crea un objeto Map que representa el terreno de juego y se generan obstáculos aleatorios.
    // Por qué sucede: Para hacer el juego más desafiante, se generan obstáculos que los tanques deben evitar.
    // Qué deberíamos esperar: Un mapa de juego con un 10% de obstáculos generados aleatoriamente.
    Map gameMap(mapSize);
    gameMap.generateObstacles(10);

    // Crear los tanques del jugador 1 (azul y rojo) y del jugador 2 (celeste y amarillo)
    // Qué sucede: Se añaden 4 tanques para cada jugador, con posiciones iniciales aleatorias en su respectiva mitad del mapa.
    // Por qué sucede: Cada jugador tiene 4 tanques, ubicados en diferentes secciones del mapa.
    // Qué deberíamos esperar: Los tanques se crean y se ubican sin superponerse entre sí.
    std::vector<Tank> tanks;

    // Jugador 1 (tanques azul y rojo con IDs únicos)
    // Qué sucede: Se posicionan los tanques del jugador 1 (2 azules y 2 rojos).
    for (int i = 0; i < 2; ++i) {
        int x, y;
        do {
            x = getRandomPosition(0, mapSize / 2 - 1);
            y = getRandomPosition(0, mapSize - 1);
        } while (isPositionOccupied(x, y, tanks));
        tanks.emplace_back(Tank::BLUE, x, y, i);  // Añadir ID único
    }
    for (int i = 0; i < 2; ++i) {
        int x, y;
        do {
            x = getRandomPosition(0, mapSize / 2 - 1);
            y = getRandomPosition(0, mapSize - 1);
        } while (isPositionOccupied(x, y, tanks));
        tanks.emplace_back(Tank::RED, x, y, i + 2);  // Añadir ID único
    }

    // Jugador 2 (tanques celeste y amarillo con IDs únicos)
    // Qué sucede: Se posicionan los tanques del jugador 2 (2 celestes y 2 amarillos).
    for (int i = 0; i < 2; ++i) {
        int x, y;
        do {
            x = getRandomPosition(mapSize / 2, mapSize - 1);
            y = getRandomPosition(0, mapSize - 1);
        } while (isPositionOccupied(x, y, tanks));
        tanks.emplace_back(Tank::CYAN, x, y, i + 4);  // Añadir ID único
    }
    for (int i = 0; i < 2; ++i) {
        int x, y;
        do {
            x = getRandomPosition(mapSize / 2, mapSize - 1);
            y = getRandomPosition(0, mapSize - 1);
        } while (isPositionOccupied(x, y, tanks));
        tanks.emplace_back(Tank::YELLOW, x, y, i + 6);  // Añadir ID único
    }

    // Variables de control del juego
    // Qué sucede: Inicializamos las variables que controlan el turno actual, el tanque seleccionado y el modo de juego (disparo, movimiento, etc.).
    // Por qué sucede: Necesitamos gestionar quién está jugando, qué acciones pueden realizarse y el estado de la partida.
    // Qué deberíamos esperar: Las variables establecen los estados y reglas necesarias para controlar la partida.
    int currentPlayer = 1; // Jugador actual
    Tank* selectedTank = nullptr;  // Puntero al tanque seleccionado
    bool waitingForBFSClick = false;  // Indica si estamos esperando un clic para BFS
    bool waitingForDijkstraClick = false;  // Indica si estamos esperando un clic para Dijkstra
    bool powerUsed = false;  // Indica si un poder ya fue usado en este turno
    char selectedPower = '\0';  // Poder seleccionado ('M', 'D', 'P'), '\0' si no se ha seleccionado ninguno
    std::vector<Cell> currentPath;  // Para almacenar la ruta calculada
    sf::Clock globalClock; // Reloj para medir el tiempo total de juego
    sf::Clock turnClock; // Reloj para medir el tiempo del turno actual

    // Variables para el modo disparo
    // Qué sucede: Se definen variables para gestionar el estado de disparo.
    // Por qué sucede: Necesitamos manejar si un tanque está en modo disparo y controlar la bala correspondiente.
    // Qué deberíamos esperar: Variables para gestionar el disparo, asegurando que sólo se haga una vez por turno.
    bool isShootingMode = false; // Modo disparo activado
    Bullet* activeBullet = nullptr; // Bala activa
    bool hasShot = false;  // Nueva bandera para limitar el disparo por turno

    // Variables para el sistema de power-ups
    // Qué sucede: Gestiona los power-ups disponibles para cada jugador y si están activos o consumidos.
    // Por qué sucede: Los power-ups añaden habilidades especiales a los jugadores, haciendo el juego más interesante.
    // Qué deberíamos esperar: Variables que controlen el estado de los power-ups y cuándo están disponibles.
    PowerUp playerPowerUp[2] = { NONE, NONE }; // Power-ups de los jugadores
    bool isPowerUpActive = false;  // Indica si un power-up está activo
    bool powerUpActivated = false;  // Se activó un power-up en este turno
    bool powerUpConsumed = false;  // Power-up fue utilizado en este turno

    // Bucle principal del juego
    // Qué sucede: Se gestiona toda la lógica del juego dentro de un bucle que se repite mientras la ventana esté abierta.
    // Por qué sucede: Es necesario mantener un bucle para gestionar las entradas del jugador y actualizar el estado del juego constantemente.
    // Qué deberíamos esperar: La ventana se actualizará en cada iteración, mostrando los cambios en el estado del juego.
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Detectar click en el tanque
            // Qué sucede: Se detecta un clic del ratón y verifica si se ha seleccionado un tanque.
            // Por qué sucede: Permite al jugador seleccionar un tanque que quiere mover o atacar.
            // Qué deberíamos esperar: Un tanque seleccionado y un mensaje de confirmación en la consola.
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int mouseX = event.mouseButton.x / cellSize;
                int mouseY = event.mouseButton.y / cellSize;

                if (waitingForBFSClick && selectedTank != nullptr) {
                    // Usa BFS para ir a donde el jugador ha hecho clic
                    // Qué sucede: Calcula la ruta usando BFS al punto clicado si es válido.
                    // Por qué sucede: Mover el tanque de forma óptima hacia la ubicación seleccionada utilizando BFS.
                    // Qué deberíamos esperar: El tanque sigue la ruta más corta hasta el destino seleccionado.
                    if (gameMap.isValidPosition(mouseX, mouseY) && !isPositionOccupied(mouseX, mouseY, tanks)) {
                        currentPath = bfs(gameMap, selectedTank->getX(), selectedTank->getY(), mouseX, mouseY, tanks);
                        waitingForBFSClick = false;  // Desactivar la espera
                    }
                } else if (waitingForDijkstraClick && selectedTank != nullptr) {
                    // Usa Dijkstra para ir a donde el jugador ha hecho clic
                    // Qué sucede: Calcula la ruta óptima utilizando Dijkstra hasta la posición seleccionada.
                    // Por qué sucede: Proporcionar un movimiento preciso para tanques con una probabilidad alta de usar Dijkstra.
                    // Qué deberíamos esperar: La ruta calculada se guarda en 'currentPath' y se desactiva la espera.
                    if (gameMap.isValidPosition(mouseX, mouseY) && !isPositionOccupied(mouseX, mouseY, tanks)) {
                        currentPath = dijkstra(gameMap, selectedTank->getX(), selectedTank->getY(), mouseX, mouseY, tanks);
                        waitingForDijkstraClick = false;  // Desactivar la espera
                    }
                } else if (selectedTank == nullptr) {
                    // Seleccionar un tanque si no estamos esperando un clic para BFS o Dijkstra
                    // Qué sucede: Permite al jugador seleccionar un tanque propio para el turno actual.
                    // Por qué sucede: Un jugador puede seleccionar cualquiera de sus tanques disponibles en su turno.
                    // Qué deberíamos esperar: 'selectedTank' apunta al tanque elegido y se muestra un mensaje de confirmación.
                    for (Tank& tank : tanks) {
                        if ((currentPlayer == 1 && (tank.getColor() == Tank::BLUE || tank.getColor() == Tank::RED)) ||
                            (currentPlayer == 2 && (tank.getColor() == Tank::CYAN || tank.getColor() == Tank::YELLOW))) {
                            if (tank.getX() == mouseX && tank.getY() == mouseY) {
                                selectedTank = &tank;
                                std::cout << "Tanque seleccionado en (" << tank.getX() << ", " << tank.getY() << ")\n";
                                break;
                            }
                        }
                    }
                }
            }

            // Detectar la tecla M para movimiento
            // Qué sucede: Al presionar la tecla M, el jugador decide mover el tanque seleccionado.
            // Por qué sucede: La tecla M activa el modo movimiento para el tanque seleccionado.
            // Qué deberíamos esperar: El tanque se mueve usando BFS, Dijkstra o movimiento aleatorio según el color y la probabilidad.
            if (event.type == sf::Event::KeyPressed && selectedTank != nullptr && !powerUsed) {
                if (event.key.code == sf::Keyboard::M && selectedPower == '\0') {
                    selectedPower = 'M';
                    powerUsed = true;
                    if (selectedTank->getColor() == Tank::BLUE || selectedTank->getColor() == Tank::CYAN) {
                        int randomDecision = std::rand() % 2;
                        if (randomDecision == 0) {
                            std::cout << "Usando BFS para mover tanque azul/celeste\n";
                            waitingForBFSClick = true; // Esperar clic para definir destino
                        } else {
                            std::cout << "Usando movimiento aleatorio para tanque azul/celeste\n";
                            currentPath = moveRandomly(selectedTank->getX(), selectedTank->getY(), gameMap, tanks);
                        }
                    } else if (selectedTank->getColor() == Tank::RED || selectedTank->getColor() == Tank::YELLOW) {
                        int randomDecision = std::rand() % 10;
                        if (randomDecision < 8) {
                            std::cout << "Usando Dijkstra para mover tanque rojo/amarillo\n";
                            waitingForDijkstraClick = true; // Esperar clic para definir destino
                        } else {
                            std::cout << "Usando movimiento aleatorio para tanque rojo/amarillo\n";
                            currentPath = moveRandomly(selectedTank->getX(), selectedTank->getY(), gameMap, tanks);
                        }
                    }
                } 
            }

            // Detectar la tecla D para activar el modo disparo
            // Qué sucede: Al presionar la tecla D, el jugador activa el modo disparo para el tanque seleccionado.
            // Por qué sucede: Permite al jugador disparar con el tanque seleccionado.
            // Qué deberíamos esperar: El modo disparo se activa y se espera la selección de un objetivo.
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D && selectedTank != nullptr && !powerUsed && !powerUpActivated) {
                if (selectedPower == '\0') {  // No permitir cambiar de modo si ya hay otro activo
                    selectedPower = 'D';
                    powerUsed = true;
                    isShootingMode = true; // Activar el modo disparo correctamente
                    std::cout << "Modo disparo activado\n";
                }
            }

            // Detectar click en el objetivo al disparar
            // Qué sucede: Se dispara hacia una celda específica seleccionada por el jugador.
            // Por qué sucede: Permite definir la dirección del disparo del tanque.
            // Qué deberíamos esperar: Una nueva bala creada y disparada hacia la posición seleccionada.
            if (isShootingMode && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !hasShot) {
                int targetX = event.mouseButton.x / cellSize;
                int targetY = event.mouseButton.y / cellSize;

                // Crear una nueva bala desde la posición del tanque seleccionado hacia el objetivo
                activeBullet = new Bullet(selectedTank->getX(), selectedTank->getY(), targetX, targetY, selectedTank->getId());

                // Limpiar el modo disparo y marcar que ya ha disparado
                isShootingMode = false;
                hasShot = true;
            }

            // Activar Power-up con la tecla P
            // Qué sucede: Al presionar la tecla P, el jugador puede activar un power-up si está disponible.
            // Por qué sucede: Los power-ups brindan ventajas adicionales y hacen el juego más interesante.
            // Qué deberíamos esperar: Un power-up se activa y se realiza su efecto en el turno.
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P && !powerUsed) {
                if (playerPowerUp[currentPlayer - 1] != NONE && !powerUpConsumed) {
                    isPowerUpActive = true;
                    powerUpActivated = true;
                    powerUpConsumed = true;
                    std::cout << "Power-up activado: " << playerPowerUp[currentPlayer - 1] << "\n";
                }
            }
        }

        // Lógica de power-ups
        // Qué sucede: Con una probabilidad del 30%, asigna un power-up al jugador actual.
        // Por qué sucede: Proporciona aleatoriedad y ventaja temporal para los jugadores.
        // Qué deberíamos esperar: Si el jugador tiene suerte, se le asigna un power-up.
        if (!isPowerUpActive && std::rand() % 100 < 30) {  // Probabilidad del 30%
            playerPowerUp[currentPlayer - 1] = static_cast<PowerUp>(std::rand() % 4 + 1);  // Asignar aleatoriamente uno de los 4 power-ups
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
        // Qué sucede: Actualiza la posición de la bala en cada frame y la destruye si es necesario.
        // Por qué sucede: Permite el movimiento y el impacto de la bala en el juego.
        // Qué deberíamos esperar: La bala se mueve y se destruye si colisiona.
        if (activeBullet != nullptr) {
            bool destroyBullet = false;
            activeBullet->update(gameMap, tanks, destroyBullet);
            if (destroyBullet) {
                delete activeBullet;
                activeBullet = nullptr;
            }
        }

        // Remover tanques destruidos
        // Qué sucede: Elimina de la lista los tanques que han sido destruidos.
        // Por qué sucede: Mantener la lista de tanques actualizada al eliminar los que ya no están activos.
        // Qué deberíamos esperar: Los tanques destruidos son eliminados de 'tanks'.
        tanks.erase(std::remove_if(tanks.begin(), tanks.end(),
            [](const Tank& tank) { return tank.isDestroyed(); }), tanks.end());

        // Actualizar el texto del turno y el temporizador
        // Qué sucede: Se actualizan los textos en pantalla del turno actual y del tiempo restante.
        // Por qué sucede: Proporcionar al jugador información visual actualizada del estado del juego.
        // Qué deberíamos esperar: Textos con el turno y el tiempo actualizado en cada frame.
        int remainingTime = 300 - globalClock.getElapsedTime().asSeconds();
        globalTimerText.setString("Tiempo: " + std::to_string(remainingTime / 60) + ":" + std::to_string(remainingTime % 60));
        turnText.setString("Turno del Jugador: " + std::to_string(currentPlayer));

        // Cambiar de turno cada 15 segundos o al cumplir los turnos extra por power-up
        // Qué sucede: Cambia al siguiente jugador después de 15 segundos o si se terminan los turnos adicionales.
        // Por qué sucede: Garantiza que el juego avance y permite que cada jugador tenga su turno.
        // Qué deberíamos esperar: Cambio de jugador cuando se cumple el tiempo o los turnos adicionales.
        if (turnClock.getElapsedTime().asSeconds() >= 15.0f || turnControl[currentPlayer - 1] > 0) {
            if (turnControl[currentPlayer - 1] > 0) {
                turnControl[currentPlayer - 1]--;  // Reducir turnos extra si hay
            } else {
                currentPlayer = (currentPlayer == 1) ? 2 : 1;  // Cambiar al otro jugador
            }

            turnClock.restart();
            powerUsed = false;
            selectedPower = '\0';
            selectedTank = nullptr;
            currentPath.clear();
            hasShot = false;
            powerUpActivated = false;  // Reiniciar estado de power-up
            powerUpConsumed = false;

            // Limpiar bala activa al final del turno
            delete activeBullet;
            activeBullet = nullptr;
        }

        // Limpiar ventana
        // Qué sucede: Borra todo el contenido de la ventana antes de redibujarla.
        // Por qué sucede: Asegura que no queden residuos de los dibujos anteriores.
        // Qué deberíamos esperar: Ventana limpia lista para el siguiente frame.
        window.clear(sf::Color::White);

        // Dibujar el mapa y los tanques
        // Qué sucede: Dibuja el mapa y los tanques en sus posiciones actuales.
        // Por qué sucede: Para mostrar la representación visual del estado actual del juego.
        // Qué deberíamos esperar: Un mapa con los tanques dibujados en sus respectivas posiciones.
        gameMap.draw(window, cellSize);
        for (Tank& tank : tanks) {
            tank.draw(window, cellSize);
        }

        // Dibujar la ruta planificada en verde
        // Qué sucede: Dibuja la ruta calculada que seguirá el tanque seleccionado.
        // Por qué sucede: Permite al jugador visualizar hacia dónde se moverá su tanque.
        // Qué deberíamos esperar: Una línea verde que muestra la ruta del tanque.
        if (!currentPath.empty()) {
            for (const Cell& cell : currentPath) {
                sf::RectangleShape pathRect(sf::Vector2f(cellSize, cellSize));
                pathRect.setPosition(cell.x * cellSize, cell.y * cellSize);
                pathRect.setFillColor(sf::Color::Green);
                window.draw(pathRect);
            }
        }

        // Mover el tanque seleccionado según la ruta calculada
        // Qué sucede: Actualiza la posición del tanque según la ruta calculada.
        // Por qué sucede: Para que el tanque siga la ruta planificada por BFS o Dijkstra.
        // Qué deberíamos esperar: El tanque se mueve paso a paso hacia su destino.
        if (!currentPath.empty() && selectedTank != nullptr) {
            Cell nextMove = currentPath.front();
            currentPath.erase(currentPath.begin());
            selectedTank->setPosition(nextMove.x, nextMove.y);
        }

        // Dibujar la bala
        // Qué sucede: Dibuja la bala si está activa.
        // Por qué sucede: Para representar visualmente la bala en la pantalla.
        // Qué deberíamos esperar: Una bala dibujada en su posición actual.
        if (activeBullet != nullptr) {
            activeBullet->draw(window, cellSize);
        }

        // Dibujar el texto del turno, el temporizador global y el power-up actual
        // Qué sucede: Se dibujan los textos que muestran información relevante del juego.
        // Por qué sucede: Mantener al jugador informado del estado del turno, tiempo y power-ups.
        // Qué deberíamos esperar: Textos correctamente posicionados en la pantalla.
        window.draw(turnText);
        window.draw(globalTimerText);
        window.draw(powerUpText);

        // Mostrar el contenido dibujado en la ventana
        // Qué sucede: Actualiza la ventana con todo lo que se ha dibujado.
        // Por qué sucede: Hace que los cambios sean visibles para el jugador.
        // Qué deberíamos esperar: La ventana mostrando el estado actualizado del juego.
        window.display();
    }

    return 0;
}
