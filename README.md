# ObjectFighters();

Prosta turowa gra walki w konsoli, napisana w C++. Możesz zagrać we dwie osoby
na jednym komputerze albo z botem.

## Wymagania

Kompilator C++ obsługujący standard C++17, np. `g++`. Na Windowsie można go zainstalować
przez MSYS2 (pakiet `mingw-w64-ucrt-x86_64-gcc`) i dodać `C:\msys64\ucrt64\bin` do zmiennej PATH.

## Kompilacja

```bash
g++ -std=c++17 -o ObjectFighters main.cpp postacie.cpp przedmioty.cpp
```

## Uruchomienie

```bash
.\ObjectFighters.exe      # Windows (PowerShell)
```

## Jak grać

Wszystkie wybory robisz, wpisując numer opcji i naciskając Enter.

1. **Menu główne:** `1` walka dwóch graczy, `2` walka z botem, `3` wyjście.
2. **Postać:** wpisz imię (bez spacji, `-` oznacza imię domyślne) i wybierz klasę:
   `1` Wojownik albo `2` Łucznik.
3. **Ekwipunek:** wybierz 5 przedmiotów z katalogu: `1` Miecz, `2` Topór, `3` Mikstura
   zdrowia. Bronie nie mogą się powtarzać, mikstury mogą.
4. **Walka:** w swojej turze wybierz akcję:
   - `1` atak podstawowy,
   - `2` użycie przedmiotu (potem podajesz jego numer z ekwipunku),
   - `3` zdolność specjalna (tylko raz w walce).
5. **Koniec:** wygrywa ten, kto zbije życie przeciwnika do zera. Potem wracasz do
   menu głównego.

Każdy przedmiot jest jednorazowy i znika po użyciu. Po każdej akcji uruchamia się
zdolność pasywna postaci.

| Klasa | Życie | Atak | Pasywna | Specjalna |
|---|---|---|---|---|
| Wojownik | 120 | 12 | +3 życia po akcji | Szarża (podwójny atak) |
| Łucznik | 90 | 15 | +1 do ataku po akcji | Deszcz strzał (3 strzały) |

## Przykład działania

Menu główne:

```text
=== ObjectFighters(); ===
1. Walka dwoch graczy
2. Walka z botem
3. Wyjscie
Wybor: 1
```

Jedna tura (gracz wybrał zdolność specjalną):

```text
=== Kolej: Bob ===
Bob (Lucznik) | zycie: 90/90 | atak: 15 | specjalna: dostepna
Ala (Wojownik) | zycie: 120/120 | atak: 12 | specjalna: dostepna
Ekwipunek:
  1. Topor (+10 do ataku)
  2. Mikstura zdrowia (+25 zycia)
  3. Mikstura zdrowia (+25 zycia)
  4. Mikstura zdrowia (+25 zycia)
  5. Mikstura zdrowia (+25 zycia)
1. Atak podstawowy
2. Uzyj przedmiotu z ekwipunku (jednorazowo)
3. Zdolnosc specjalna: Deszcz strzal (3 strzaly)
Akcja: 3
Bob uzywa deszczu strzal i zadaje 21 obrazen!
[Pasywna] Bob skupia sie - atak +1.
```

## Architektura w skrócie

- `Postac` to klasa bazowa postaci, z której dziedziczą `Wojownik` i `Lucznik`.
- `Przedmiot` to klasa bazowa przedmiotów, z której dziedziczą `Bron` i `Mikstura`.
- `Ekwipunek` przechowuje do 5 przedmiotów i należy do postaci.
- `main.cpp` zawiera menu i pętlę walki.


## Pliki projektu

- `main.cpp` – menu i przebieg walki,
- `postacie.h`, `postacie.cpp` – postacie,
- `przedmioty.h`, `przedmioty.cpp` – przedmioty i ekwipunek,
- `UML.png` – schemat klas UML,
- `README.md` – ten plik.

## Autor

Cezary Greczyn-Czereśniowski
