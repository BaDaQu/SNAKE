# Gra Snake w C++ z biblioteką ncurses

## Opis Projektu

Projekt implementuje klasyczną grę Snake w języku C++, wykorzystując bibliotekę `ncurses` do obsługi interfejsu tekstowego w konsoli. Gra jest zbudowana w oparciu o prosty, obiektowy system okienkowy, który zarządza wyświetlaniem elementów na ekranie oraz interakcją z użytkownikiem. Główna logika gry, obejmująca ruch węża, mechanikę zbierania jabłek, system punktacji oraz poziomy trudności, jest zaimplementowana w klasie `CSnake`.

### System Okienkowy

System okienkowy, którego definicje znajdują się w plikach `winsys.h` i implementacja w `winsys.cpp`, dostarcza zestaw podstawowych klas abstrakcyjnych:
*   `CView`: Reprezentuje ogólny, abstrakcyjny widok na ekranie.
*   `CWindow`: Proste okno, które może być wypełnione określonym znakiem.
*   `CFramedWindow`: Rozszerzenie `CWindow`, dodające możliwość rysowania ramki wokół okna.
*   `CInputLine`: Specjalny typ okna z ramką, umożliwiający użytkownikowi wprowadzanie tekstu.
*   `CGroup` i `CDesktop`: Klasy te odpowiadają za zarządzanie hierarchią wielu widoków (okien) oraz głównym cyklem życia aplikacji i obsługą zdarzeń.

Interakcja z biblioteką `ncurses` (inicjalizacja ekranu, rysowanie znaków, pobieranie danych wejściowych) jest zamknięta w modułach `screen.h` i `screen.cpp`, tworząc warstwę abstrakcji ułatwiającą pracę z terminalem.

### Logika Gry (CSnake)

Główna mechanika gry, zaimplementowana w klasie `CSnake` (`snake.h`, `snake.cpp`), zawiera następujące elementy:
*   **Ruch węża:** Kontrolowany przez gracza za pomocą klawiszy WSAD, umożliwiający poruszanie się w czterech podstawowych kierunkach.
*   **Zbieranie jabłek:** Wąż rośnie po zjedzeniu jabłka, które pojawia się losowo na planszy.
*   **System punktacji i poziomów:** Gracz zdobywa punkty za każde zebrane jabłko. Osiągnięcie określonego progu punktowego prowadzi do awansu na kolejny poziom, co skutkuje zwiększeniem prędkości węża.
*   **Pauza i Restart:** Gra oferuje możliwość wstrzymania rozgrywki (klawisz P) oraz jej zresetowania do stanu początkowego (klawisz R).
*   **Warunki zakończenia gry:** Kolizja węża z własnym ogonem kończy grę, wyświetlając komunikat "GAME OVER". W tej implementacji wąż przechodzi na przeciwną stronę planszy po dotarciu do jej krawędzi.

### Struktury Pomocnicze i Organizacja

Do reprezentacji pozycji i obszarów na ekranie wykorzystywane są struktury `CPoint` (punkt o współrzędnych x, y) oraz `CRect` (prostokąt zdefiniowany przez lewy górny róg i rozmiar), zadeklarowane w pliku `cpoint.h`.

Plik `main.cpp` pełni rolę punktu startowego aplikacji, gdzie inicjalizowany jest obiekt `CDesktop` (pulpit) i dodawane są do niego instancje okien, w tym główne okno gry `CSnake`.

Proces budowania projektu jest zarządzany przez `Makefile`, który kompiluje poszczególne moduły, umieszcza pliki obiektowe w dedykowanym folderze `obj`, a następnie linkuje je w celu stworzenia finalnego pliku wykonywalnego o nazwie `winsys`.

Całość projektu stanowi praktyczny przykład implementacji prostej gry konsolowej z wykorzystaniem podejścia obiektowego oraz biblioteki `ncurses` do tworzenia interaktywnego interfejsu użytkownika w trybie tekstowym.
