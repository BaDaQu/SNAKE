#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"  // Dołączanie nagłówka systemu okien
#include <string>
#include <list>
#include <vector>

// Enum do definiowania kierunków ruchu węża
enum Direction { UP, DOWN, LEFT, RIGHT };

// Klasa CSnake dziedziczy po CFramedWindow
class CSnake : public CFramedWindow {
    std::list<CPoint> snake;  // Lista punktów reprezentujących węża
    std::vector<CPoint> apples;  // Wektor punktów reprezentujących jabłka
    bool isPaused;  // Flaga informująca, czy gra jest wstrzymana
    bool showGameOver;  // Flaga do wyświetlania komunikatu "GAME OVER"
    std::string instructions;  // Instrukcje dla gracza
    std::string pauseInstructions;  // Instrukcje wyświetlane w trybie pauzy
    Direction dir;  // Kierunek ruchu węża
    int level;  // Poziom gry
    int score;  // Wynik gracza
    int baseSpeed;  // Podstawowa prędkość węża
    int levelUpCounter;  // Licznik do śledzenia, ile ruchów ma być wyświetlany komunikat "LEVEL UP"

public:
    // Konstruktor klasy CSnake, przyjmuje prostokąt jako argument
    CSnake(CRect r, char _c = ' ');

    // Metoda do przełączania stanu pauzy
    void togglePause();

    // Metoda do rysowania elementów gry
    void paint() override;

    // Metoda do obsługi zdarzeń klawiatury
    bool handleEvent(int key) override;

    // Metoda do ruchu węża
    void moveSnake();

    // Metoda do generowania jabłka na planszy
    void generateApple();

    // Metoda do zwiększania poziomu gry
    void increaseLevel();

    // Metoda do obliczania prędkości węża w zależności od poziomu
    int calculateSpeed();

    // Metoda do resetowania gry do stanu początkowego
    void resetGame();

    // Metoda do przesuwania ramki okna oraz elementów gry
    void move(const CPoint & delta) override;
};

#endif