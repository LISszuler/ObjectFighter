// ============================================================
// main.cpp - glowny plik gry ObjectFighters(); - menu i przebieg walki
// ============================================================

#include <iostream>    // cout, cin - wypisywanie i wczytywanie
#include <cstdlib>     // rand(), srand(), exit()
#include <ctime>       // time() - aktualny czas (do losowania)
#include <stdexcept>   // runtime_error - wyjatki
#include "postacie.h"   // klasy postaci
#include "przedmioty.h"  // klasy przedmiotow i ekwipunek

using namespace std;   // piszemy cout zamiast std::cout

// ------------------------------------------------------------
// Wczytuje od gracza liczbe z zakresu od min do max.
// Pyta tak dlugo, az gracz poda poprawna liczbe.
// ------------------------------------------------------------
int wczytaj(int min, int max) {
    int x;                                   // tu zapiszemy wpisana liczbe
    while (true) {                           // petla w nieskonczonosc, wyjdziemy przez return
        cin >> x;                            // wczytujemy liczbe z klawiatury
        if (cin.eof()) exit(0);              // jesli skonczylo sie wejscie - konczymy program
        if (cin.fail()) {                    // gracz wpisal cos, co nie jest liczba (np. litery)
            cin.clear();                     // czyscimy flage bledu
            cin.ignore(1000, '\n');          // wyrzucamy zle wpisane znaki
        } else if (x >= min && x <= max) {   // liczba jest w zakresie
            return x;                        // zwracamy ja i konczymy funkcje
        }
        cout << "Niepoprawna wartosc, sprobuj ponownie: ";
    }
}

// ------------------------------------------------------------
// Katalog przedmiotow: tworzy przedmiot o danym numerze (1-3).
// "new" tworzy obiekt w pamieci i zwraca na niego wskaznik.
// ------------------------------------------------------------
Przedmiot* stworzPrzedmiot(int numer) {
    if (numer == 1) return new Bron("Miecz", 6);
    if (numer == 2) return new Bron("Topor", 10);
    return new Mikstura("Mikstura zdrowia", 25);   // w pozostalym przypadku (3)
}

// ------------------------------------------------------------
// Wypisuje katalog przedmiotow do wyboru
// (opisy wpisane recznie - musza zgadzac sie z funkcja wyzej)
// ------------------------------------------------------------
void pokazKatalog() {
    cout << "Dostepne przedmioty (bronie nie moga sie powtarzac, mikstury moga):" << endl;
    cout << "  1. Miecz (+6 do ataku)" << endl;
    cout << "  2. Topor (+10 do ataku)" << endl;
    cout << "  3. Mikstura zdrowia (+25 zycia)" << endl;
}

// ------------------------------------------------------------
// Tworzy gracza: pyta o imie i klase, zwraca wskaznik na postac
// ------------------------------------------------------------
Postac* stworzGracza(int numer) {
    string imie;
    cout << "\n--- Gracz " << numer << " ---" << endl;
    cout << "Imie (wpisz - dla domyslnego): ";
    cin >> imie;                                                // wczytujemy imie (bez spacji)
    cout << "1. Wojownik (zycie 120, atak 12) - Szarza" << endl;
    cout << "2. Lucznik  (zycie 90, atak 15) - Deszcz strzal" << endl;
    cout << "Klasa: ";
    int wybor = wczytaj(1, 2);                                  // gracz wybiera 1 albo 2

    // Wskaznik typu Postac moze wskazywac na Wojownika ALBO Lucznika (polimorfizm).
    // Imie "-" oznacza: uzyj konstruktora domyslnego (bez imienia).
    Postac* gracz;
    if (wybor == 1 && imie == "-") {
        gracz = new Wojownik();         // konstruktor domyslny
    } else if (wybor == 1) {
        gracz = new Wojownik(imie);     // konstruktor z imieniem
    } else if (imie == "-") {
        gracz = new Lucznik();          // konstruktor domyslny
    } else {
        gracz = new Lucznik(imie);      // konstruktor z imieniem
    }

    // Jesli gracz wybral domyslne imie, zmieniamy je na "Gracz1" lub "Gracz2",
    // zeby dwie postacie nie mialy tego samego imienia
    if (imie == "-") {
        gracz->setImie("Gracz" + to_string(numer));
    }
    return gracz;
}

// ------------------------------------------------------------
// Gracz wybiera 5 przedmiotow do swojego ekwipunku
// ------------------------------------------------------------
void przygotujEkwipunek(Postac* gracz) {
    Ekwipunek& ekw = gracz->getEkwipunek();   // & = pracujemy na ORYGINALNYM ekwipunku, nie na kopii
    cout << "\n--- Ekwipunek gracza " << gracz->getImie()
         << " (" << ekw.getMaxMiejsc() << " miejsc) ---" << endl;
    cout << "Kazdy przedmiot mozna uzyc tylko raz (potem znika z ekwipunku)." << endl;

    // Petla: dopoki ekwipunek nie jest pelny, gracz wybiera kolejne przedmioty
    while (ekw.ile() < ekw.getMaxMiejsc()) {
        pokazKatalog();
        cout << "Wybierz przedmiot (zajete miejsca: " << ekw.ile()
             << "/" << ekw.getMaxMiejsc() << "): ";
        Przedmiot* p = stworzPrzedmiot(wczytaj(1, 3));   // tworzymy wybrany przedmiot
        try {                                            // probujemy dodac przedmiot...
            ekw.dodaj(p);                                // (tu moze zostac rzucony wyjatek)
            cout << "Dodano: " << p->getNazwa() << endl;
        } catch (runtime_error& e) {                     // ...a jesli sie nie udalo, lapiemy blad
            cout << "Blad: " << e.what() << endl;        // e.what() to tekst bledu
            delete p;                                    // przedmiot nie trafil do ekwipunku, wiec go usuwamy
        }
    }

    // Ekwipunek jest pelny - pokazujemy go graczowi
    cout << "\nTwoj ekwipunek jest gotowy:" << endl;
    ekw.wypisz();
}

// ------------------------------------------------------------
// Jedna akcja gracza w jego turze
// gracz - ten, kto teraz gra; wrog - jego przeciwnik
// ------------------------------------------------------------
void akcja(Postac* gracz, Postac* wrog) {
    Ekwipunek& ekw = gracz->getEkwipunek();

    // Pokazujemy stan obu postaci (dziala nasz operator <<)
    cout << *gracz << endl;    // * zamienia wskaznik na obiekt
    cout << *wrog << endl;
    cout << "Ekwipunek:" << endl;
    if (ekw.ile() == 0) {
        cout << "  (pusty)" << endl;
    } else {
        ekw.wypisz();
    }
    cout << "1. Atak podstawowy" << endl;
    cout << "2. Uzyj przedmiotu z ekwipunku (jednorazowo)" << endl;
    cout << "3. Zdolnosc specjalna: " << gracz->nazwaSpecjalnej();
    if (gracz->getSpecjalnaUzyta()) cout << " [UZYTA]";
    cout << endl;

    bool zrobione = false;   // czy gracz wykonal poprawna akcje
    while (!zrobione) {      // powtarzamy, dopoki nie wykona poprawnej akcji
        cout << "Akcja: ";
        int wybor = wczytaj(1, 3);

        if (wybor == 1) {                    // atak podstawowy - zawsze mozliwy
            gracz->atakuj(*wrog);
            zrobione = true;
        } else if (wybor == 2) {             // gracz chce uzyc przedmiotu
            if (ekw.ile() == 0) {            // nie ma czego uzyc
                cout << "Ekwipunek jest pusty!" << endl;
            } else {
                cout << "Numer przedmiotu: ";
                int numer = wczytaj(1, ekw.ile());
                try {
                    Przedmiot* p = ekw.pobierz(numer - 1);   // bierzemy przedmiot z ekwipunku
                    p->uzyj(*gracz, *wrog);                  // POLIMORFIZM: bron zaatakuje, mikstura wyleczy
                    ekw.usun(numer - 1);                     // kazdy przedmiot jest jednorazowy - usuwamy go
                    zrobione = true;                         // akcja wykonana
                } catch (runtime_error& e) {
                    cout << "Blad: " << e.what() << endl;
                }
            }
        } else {                             // gracz chce uzyc zdolnosci specjalnej
            if (!gracz->getSpecjalnaUzyta()) {               // jeszcze jej nie uzyl?
                gracz->specjalna(*wrog);                     // POLIMORFIZM: kazda klasa robi to inaczej
                zrobione = true;
            } else {
                cout << "Zdolnosc specjalna juz uzyta!" << endl;
            }
        }
    }
}

// ------------------------------------------------------------
// Tworzy bota: losowa postac i losowy ekwipunek
// ------------------------------------------------------------
Postac* stworzBota() {
    // Losowa klasa: rand() % 2 daje 0 albo 1
    Postac* bot;
    if (rand() % 2 == 0) {
        bot = new Wojownik("Bot");
    } else {
        bot = new Lucznik("Bot");
    }

    // Losowy ekwipunek: dobieramy przedmioty, az bedzie pelny
    Ekwipunek& ekw = bot->getEkwipunek();
    while (ekw.ile() < ekw.getMaxMiejsc()) {
        Przedmiot* p = stworzPrzedmiot(rand() % 3 + 1);   // losowy numer od 1 do 3
        try {
            ekw.dodaj(p);
        } catch (runtime_error& e) {
            delete p;   // np. powtorzona bron - losujemy jeszcze raz
        }
    }

    cout << "\nBot wybral klase: " << bot->nazwaKlasy() << endl;
    cout << "Ekwipunek bota:" << endl;
    ekw.wypisz();
    return bot;
}

// ------------------------------------------------------------
// Jedna akcja bota: losuje akcje, az trafi na taka, ktora mozna wykonac
// ------------------------------------------------------------
void akcjaBota(Postac* bot, Postac* wrog) {
    Ekwipunek& ekw = bot->getEkwipunek();

    // Pokazujemy stan obu postaci
    cout << *bot << endl;
    cout << *wrog << endl;

    bool zrobione = false;
    while (!zrobione) {
        int wybor = rand() % 3 + 1;                 // losowa akcja: 1, 2 albo 3
        if (wybor == 1) {                           // atak podstawowy (zawsze mozliwy)
            bot->atakuj(*wrog);
            zrobione = true;
        } else if (wybor == 2 && ekw.ile() > 0) {   // przedmiot (tylko jesli jakis jest)
            int numer = rand() % ekw.ile();         // losowy przedmiot (numeracja od 0)
            Przedmiot* p = ekw.pobierz(numer);
            p->uzyj(*bot, *wrog);
            ekw.usun(numer);                        // przedmiot jest jednorazowy
            zrobione = true;
        } else if (wybor == 3 && !bot->getSpecjalnaUzyta()) {   // specjalna (tylko raz)
            bot->specjalna(*wrog);
            zrobione = true;
        }
        // jesli wylosowana akcja jest niemozliwa, petla losuje jeszcze raz
    }
}

// ------------------------------------------------------------
// Wypisuje ekran koncowy: kto wygral i ile trwala walka
// ------------------------------------------------------------
void pokazKoniec(Postac* zwyciezca, int tury) {
    cout << "\n==============================" << endl;
    cout << "         KONIEC WALKI" << endl;
    cout << "==============================" << endl;
    cout << "Zwyciezca: " << zwyciezca->getImie() << " (" << zwyciezca->nazwaKlasy() << ")" << endl;
    cout << "Pozostale zycie: " << zwyciezca->getZycie() << "/" << zwyciezca->getZycieMax() << endl;
    cout << "Liczba tur: " << tury << endl;
    cout << "==============================" << endl;
}

// ------------------------------------------------------------
// Cala walka od przygotowania graczy do ekranu koncowego
// ------------------------------------------------------------
void graj(bool zBotem) {
    // --- Przygotowanie graczy ---
    Postac* gracz1 = stworzGracza(1);
    przygotujEkwipunek(gracz1);

    Postac* gracz2;
    if (zBotem) {
        gracz2 = stworzBota();              // przeciwnik to komputer
        if (*gracz2 == *gracz1) {           // gracz tez nazwal sie "Bot"?
            gracz2->setImie("Bot2");
        }
    } else {
        gracz2 = stworzGracza(2);           // przeciwnik to drugi gracz
        while (*gracz2 == *gracz1) {        // operator == porownuje imiona
            cout << "Gracze musza miec rozne imiona!" << endl;
            delete gracz2;                  // usuwamy zle utworzonego gracza
            gracz2 = stworzGracza(2);       // i tworzymy go od nowa
        }
        przygotujEkwipunek(gracz2);
    }

    // --- Losowanie, kto zaczyna ---
    Postac* aktywny = gracz1;      // aktywny = ten, kto teraz wykonuje ruch
    Postac* wrog = gracz2;         // wrog = jego przeciwnik
    if (rand() % 2 == 1) {         // rand() % 2 daje 0 albo 1
        aktywny = gracz2;          // jesli wylosowano 1, zaczyna gracz 2
        wrog = gracz1;
    }
    cout << "\nZaczyna " << aktywny->getImie() << "!" << endl;

    // --- Glowna petla walki ---
    int ruchy = 0;                 // liczymy ruchy graczy
    while (true) {
        cout << "\n=== Kolej: " << aktywny->getImie() << " ===" << endl;
        if (zBotem && aktywny == gracz2) {   // teraz ruch bota?
            akcjaBota(aktywny, wrog);        // bot losuje akcje
        } else {
            akcja(aktywny, wrog);            // gracz wybiera akcje
        }
        ruchy++;
        if (!wrog->zyje()) break;  // jesli wrog zginal - koniec walki

        aktywny->pasywna();        // po akcji dziala zdolnosc pasywna

        // Zamiana rol: aktywny staje sie wrogiem i odwrotnie
        Postac* pomoc = aktywny;   // zmienna pomocnicza do zamiany
        aktywny = wrog;
        wrog = pomoc;
    }

    // --- Ekran koncowy ---
    Postac* zwyciezca = gracz1;                  // zakladamy, ze wygral gracz 1
    if (!gracz1->zyje()) zwyciezca = gracz2;     // ale jesli on nie zyje, wygral gracz 2
    int tury = (ruchy + 1) / 2;                  // 1 tura = ruch obu graczy (liczymy w gore)
    pokazKoniec(zwyciezca, tury);

    // Sprzatanie: usuwamy postacie z pamieci (ekwipunek usunie sie razem z nimi)
    delete gracz1;
    delete gracz2;
}

// ------------------------------------------------------------
// main - tu startuje program: menu glowne
// ------------------------------------------------------------
int main() {
    srand(time(0));   // ustawiamy generator losowy na podstawie aktualnego czasu

    while (true) {    // menu wraca po kazdej walce
        cout << "\n=== ObjectFighters(); ===" << endl;
        cout << "1. Walka dwoch graczy" << endl;
        cout << "2. Walka z botem" << endl;
        cout << "3. Wyjscie" << endl;
        cout << "Wybor: ";
        int wybor = wczytaj(1, 3);
        if (wybor == 3) break;   // 3 = wyjscie z petli, czyli koniec programu
        graj(wybor == 2);        // 2 = walka z botem (true), 1 = dwoch graczy (false)
    }

    cout << "Dzieki za gre w ObjectFighters();" << endl;
    return 0;   // 0 oznacza, ze program zakonczyl sie poprawnie
}
