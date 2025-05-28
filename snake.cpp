#include "snake.h"
#include "screen.h"
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip> // Dla std::setw i std::setfill

// Funkcja pomocnicza do porównywania dwóch punktów
bool pointsEqual(const CPoint& p1, const CPoint& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

// Konstruktor klasy CSnake
CSnake::CSnake(CRect r, char _c /*=' '*/) :
    CFramedWindow(r, _c),
    isPaused(true),
    showGameOver(false),  // Inicjalizacja flagi "GAME OVER"
    instructions("Sterowanie: WSAD, Pauza: P, Restart: R"),
    pauseInstructions("TRYB PAUZY\n\nSTEROWANIE - WSAD\nTRYB PAUZY / TRYB GRY - P\nRESTART - R"),
    dir(RIGHT),
    level(1),
    score(0),
    baseSpeed(2500),
    levelUpCounter(0)  // Inicjalizacja licznika
{
    resetGame();
}

// Resetowanie gry do stanu początkowego
void CSnake::resetGame() {
    snake.clear();
    apples.clear();
    CPoint midPoint(geom.topleft.x + geom.size.x / 2, geom.topleft.y + geom.size.y / 2);
    snake.push_back(midPoint);  // Głowa węża
    snake.push_back(CPoint(midPoint.x - 1, midPoint.y));  // Pierwszy segment
    snake.push_back(CPoint(midPoint.x - 2, midPoint.y));  // Drugi segment
    dir = RIGHT;
    level = 1;
    score = 0;
    isPaused = true;
    levelUpCounter = 0;  // Reset licznika
    showGameOver = false;  // Reset flagi "GAME OVER"
    generateApple();
}

// Przełączanie stanu pauzy
void CSnake::togglePause() {
    isPaused = !isPaused;
}

// Metoda rysująca elementy gry
void CSnake::paint() {
    CFramedWindow::paint();
    
    // Wyświetlanie informacji o punktach
    std::ostringstream score_info;
    score_info << "| PUNKTY: " << std::setw(3) << std::setfill('0') << score << " |";
    gotoyx(geom.topleft.y - 1, geom.topleft.x + geom.size.x - score_info.str().length() - 1);
    printl(score_info.str().c_str());

    // Wyświetlanie informacji o poziomie
    std::ostringstream level_info;
    level_info << "| POZIOM: " << level << " |";
    gotoyx(geom.topleft.y - 1, geom.topleft.x);
    printl(level_info.str().c_str());

    // Wyświetlanie komunikatu "LEVEL UP"
    if (levelUpCounter > 0) {
        std::string levelUpMsg = "LEVEL UP";
        gotoyx(geom.topleft.y - 1, geom.topleft.x + (geom.size.x - levelUpMsg.length()) / 2);
        printl(levelUpMsg.c_str());
        levelUpCounter--;
    }

    // Rysowanie jabłek
    for (const CPoint& apple : apples) {
        gotoyx(apple.y, apple.x);
        printc('O');
    }

    // Rysowanie węża
    bool first = true;
    for (const CPoint& p : snake) {
        gotoyx(p.y, p.x);
        printc(first ? 'X' : '*');
        if (first) {
            first = false;
        }
    }

    // Wyświetlanie komunikatu "GAME OVER"
    if (showGameOver) {
        std::string gameOverMsg = "GAME OVER";
        gotoyx(geom.topleft.y - 1, geom.topleft.x + (geom.size.x - gameOverMsg.length()) / 2);
        printl(gameOverMsg.c_str());
    }

    // Wyświetlanie instrukcji w trybie pauzy
    if (isPaused) {
        std::istringstream iss(pauseInstructions);
        std::string line;
        int line_count = 2;
        while (std::getline(iss, line)) {
            int centered_x = geom.topleft.x + (geom.size.x - line.length()) / 2;
            gotoyx(geom.topleft.y + line_count, centered_x);
            printl(line.c_str());
            line_count++;
        }
    }
}

// Obsługa zdarzeń klawiatury
bool CSnake::handleEvent(int key) {
	if(CFramedWindow::handleEvent(key)){
		return true;
	}
    if (key == 'p' || key == 'P') {
        togglePause();
        return true;
    } else if (key == 'r' || key == 'R') {
        resetGame();
        return true;
    }
    if (!isPaused) {
        Direction newDir = dir; // Zmienna do przechowywania nowego kierunku
        switch (key) {
            case 'w':
            case 'W':
                newDir = UP;
                break;
            case 's':
            case 'S':
                newDir = DOWN;
                break;
            case 'a':
            case 'A':
                newDir = LEFT;
                break;
            case 'd':
            case 'D':
                newDir = RIGHT;
                break;
        }

        // Sprawdzenie, czy nowy kierunek nie jest przeciwny do obecnego
        if (!((dir == UP && newDir == DOWN) ||
              (dir == DOWN && newDir == UP) ||
              (dir == LEFT && newDir == RIGHT) ||
              (dir == RIGHT && newDir == LEFT))) {
            dir = newDir; // Ustawienie nowego kierunku, jeśli nie jest przeciwny
            moveSnake();
            return true;
        }
    }
    return false;
}

// Przesunięcie ramki okna oraz elementów gry
void CSnake::move(const CPoint & delta) {
    geom.topleft += delta;  // Przesunięcie ramki okna

    // Przesunięcie wszystkich jabłek
    for (auto& apple : apples) {
        apple += delta;
    }

    // Przesunięcie wszystkich segmentów węża
    for (auto& segment : snake) {
        segment += delta;
    }
}

// Ruch węża
void CSnake::moveSnake() {
    static int last_move = clock();
    int current_time = clock();

    if (current_time - last_move < calculateSpeed()) {
        return;
    }

    last_move = current_time;

    CPoint head = snake.front();
    switch (dir) {
        case UP:    head.y--; break;
        case DOWN:  head.y++; break;
        case LEFT:  head.x--; break;
        case RIGHT: head.x++; break;
    }

    // Przejście na przeciwną stronę planszy, gdy wąż osiągnie krawędź
    if (head.x < geom.topleft.x + 1) head.x = geom.topleft.x + geom.size.x - 2;
    else if (head.x >= geom.topleft.x + geom.size.x - 1) head.x = geom.topleft.x + 1;

    if (head.y < geom.topleft.y + 1) head.y = geom.topleft.y + geom.size.y - 2;
    else if (head.y >= geom.topleft.y + geom.size.y - 1) head.y = geom.topleft.y + 1;

    // Sprawdzenie kolizji z jabłkami
    auto it = std::find_if(apples.begin(), apples.end(), [&head](const CPoint& apple) {
        return pointsEqual(apple, head);
    });
    if (it != apples.end()) {
        apples.erase(it);
        generateApple();
        score += 3;  // Zwiększamy liczbę punktów o 3 za każde zebrane jabłko
        if (score % 30 == 0) {
            increaseLevel(); // Zwiększ poziom po zebraniu 30 punktów
            levelUpCounter = 100; // Ustawienie licznika na 100 ruchów
        }
        snake.push_front(head);
        snake.push_back(snake.back());
        snake.push_back(snake.back());
    } else {
        // Sprawdzenie kolizji z samym sobą
        if (std::find_if(std::next(snake.begin()), snake.end(), [&head](const CPoint& segment) {
            return pointsEqual(segment, head);
        }) != snake.end()) {
            isPaused = true; // Zatrzymanie gry
            showGameOver = true; // Ustawienie flagi do wyświetlenia komunikatu "GAME OVER"
            return; // Zakończenie ruchu węża
        }
        snake.push_front(head);
        snake.pop_back();
    }
}

// Generowanie nowego jabłka na planszy
void CSnake::generateApple() {
    if (apples.empty()) { // Generujemy jabłko tylko jeśli nie ma żadnego na planszy
        srand(time(NULL));
        bool placed = false;
        while (!placed) {
            // Zmiana zakresu generowania jabłek, aby unikać krawędzi
            int x = geom.topleft.x + 2 + rand() % (geom.size.x - 4);
            int y = geom.topleft.y + 2 + rand() % (geom.size.y - 4);
            CPoint newApple(x, y);

            bool collision = false;
            for (const CPoint& p : snake) {
                if (p.x == newApple.x && p.y == newApple.y) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                apples.push_back(newApple);
                placed = true;
            }
        }
    }
}

// Zwiększanie poziomu gry
void CSnake::increaseLevel() {
    level++;
    levelUpCounter = 100;  // Ustawienie licznika na 100 ruchów po zdobyciu nowego poziomu
}

// Obliczanie prędkości węża w zależności od poziomu
int CSnake::calculateSpeed() {
    return baseSpeed / level; // Zmniejsz opóźnienie z każdym poziomem
}
