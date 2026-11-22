#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include "postacie.h"
#include "przedmioty.h"

using namespace std;

int wczytaj(int min, int max) {
    int x;
    while (true) {
        cin >> x;
        if (cin.eof()) exit(0);
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
        } else if (x >= min && x <= max) {
            return x;
        }
        cout << "Niepoprawna wartosc, sprobuj ponownie: ";
    }
}

Przedmiot* stworzPrzedmiot(int numer) {
    if (numer == 1) return new Bron("Miecz", 6);
    if (numer == 2) return new Bron("Topor", 10);
    return new Mikstura("Mikstura zdrowia", 25);
}

void pokazKatalog() {
    cout << "Dostepne przedmioty (bronie nie moga sie powtarzac, mikstury moga):" << endl;
    cout << "  1. Miecz (+6 do ataku)" << endl;
    cout << "  2. Topor (+10 do ataku)" << endl;
    cout << "  3. Mikstura zdrowia (+25 zycia)" << endl;
}

Postac* stworzGracza(int numer) {
    string imie;
    cout << "\n--- Gracz " << numer << " ---" << endl;
    cout << "Imie (wpisz - dla domyslnego): ";
    cin >> imie;
    cout << "1. Wojownik (zycie 120, atak 12) - Szarza" << endl;
    cout << "2. Lucznik  (zycie 90, atak 15) - Deszcz strzal" << endl;
    cout << "Klasa: ";
    int wybor = wczytaj(1, 2);

    Postac* gracz;
    if (wybor == 1 && imie == "-") {
        gracz = new Wojownik();
    } else if (wybor == 1) {
        gracz = new Wojownik(imie);
    } else if (imie == "-") {
        gracz = new Lucznik();
    } else {
        gracz = new Lucznik(imie);
    }

    if (imie == "-") {
        gracz->setImie("Gracz" + to_string(numer));
    }
    return gracz;
}

void przygotujEkwipunek(Postac* gracz) {
    Ekwipunek& ekw = gracz->getEkwipunek();
    cout << "\n--- Ekwipunek gracza " << gracz->getImie()
         << " (" << ekw.getMaxMiejsc() << " miejsc) ---" << endl;
    cout << "Kazdy przedmiot mozna uzyc tylko raz (potem znika z ekwipunku)." << endl;

    while (ekw.ile() < ekw.getMaxMiejsc()) {
        pokazKatalog();
        cout << "Wybierz przedmiot (zajete miejsca: " << ekw.ile()
             << "/" << ekw.getMaxMiejsc() << "): ";
        Przedmiot* p = stworzPrzedmiot(wczytaj(1, 3));
        try {
            ekw.dodaj(p);
            cout << "Dodano: " << p->getNazwa() << endl;
        } catch (runtime_error& e) {
            cout << "Blad: " << e.what() << endl;
            delete p;
        }
    }

    cout << "\nTwoj ekwipunek jest gotowy:" << endl;
    ekw.wypisz();
}

void akcja(Postac* gracz, Postac* wrog) {
    Ekwipunek& ekw = gracz->getEkwipunek();

    cout << *gracz << endl;
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

    bool zrobione = false;
    while (!zrobione) {
        cout << "Akcja: ";
        int wybor = wczytaj(1, 3);

        if (wybor == 1) {
            gracz->atakuj(*wrog);
            zrobione = true;
        } else if (wybor == 2) {
            if (ekw.ile() == 0) {
                cout << "Ekwipunek jest pusty!" << endl;
            } else {
                cout << "Numer przedmiotu: ";
                int numer = wczytaj(1, ekw.ile());
                try {
                    Przedmiot* p = ekw.pobierz(numer - 1);
                    p->uzyj(*gracz, *wrog);
                    ekw.usun(numer - 1);
                    zrobione = true;
                } catch (runtime_error& e) {
                    cout << "Blad: " << e.what() << endl;
                }
            }
        } else {
            if (!gracz->getSpecjalnaUzyta()) {
                gracz->specjalna(*wrog);
                zrobione = true;
            } else {
                cout << "Zdolnosc specjalna juz uzyta!" << endl;
            }
        }
    }
}

Postac* stworzBota() {

    Postac* bot;
    if (rand() % 2 == 0) {
        bot = new Wojownik("Bot");
    } else {
        bot = new Lucznik("Bot");
    }

    Ekwipunek& ekw = bot->getEkwipunek();
    while (ekw.ile() < ekw.getMaxMiejsc()) {
        Przedmiot* p = stworzPrzedmiot(rand() % 3 + 1);
        try {
            ekw.dodaj(p);
        } catch (runtime_error& e) {
            delete p;
        }
    }

    cout << "\nBot wybral klase: " << bot->nazwaKlasy() << endl;
    cout << "Ekwipunek bota:" << endl;
    ekw.wypisz();
    return bot;
}

void akcjaBota(Postac* bot, Postac* wrog) {
    Ekwipunek& ekw = bot->getEkwipunek();

    cout << *bot << endl;
    cout << *wrog << endl;

    bool zrobione = false;
    while (!zrobione) {
        int wybor = rand() % 3 + 1;
        if (wybor == 1) {
            bot->atakuj(*wrog);
            zrobione = true;
        } else if (wybor == 2 && ekw.ile() > 0) {
            int numer = rand() % ekw.ile();
            Przedmiot* p = ekw.pobierz(numer);
            p->uzyj(*bot, *wrog);
            ekw.usun(numer);
            zrobione = true;
        } else if (wybor == 3 && !bot->getSpecjalnaUzyta()) {
            bot->specjalna(*wrog);
            zrobione = true;
        }

    }
}

void pokazKoniec(Postac* zwyciezca, int tury) {
    cout << "\n==============================" << endl;
    cout << "         KONIEC WALKI" << endl;
    cout << "==============================" << endl;
    cout << "Zwyciezca: " << zwyciezca->getImie() << " (" << zwyciezca->nazwaKlasy() << ")" << endl;
    cout << "Pozostale zycie: " << zwyciezca->getZycie() << "/" << zwyciezca->getZycieMax() << endl;
    cout << "Liczba tur: " << tury << endl;
    cout << "==============================" << endl;
}

void graj(bool zBotem) {

    Postac* gracz1 = stworzGracza(1);
    przygotujEkwipunek(gracz1);

    Postac* gracz2;
    if (zBotem) {
        gracz2 = stworzBota();
        if (*gracz2 == *gracz1) {
            gracz2->setImie("Bot2");
        }
    } else {
        gracz2 = stworzGracza(2);
        while (*gracz2 == *gracz1) {
            cout << "Gracze musza miec rozne imiona!" << endl;
            delete gracz2;
            gracz2 = stworzGracza(2);
        }
        przygotujEkwipunek(gracz2);
    }

    Postac* aktywny = gracz1;
    Postac* wrog = gracz2;
    if (rand() % 2 == 1) {
        aktywny = gracz2;
        wrog = gracz1;
    }
    cout << "\nZaczyna " << aktywny->getImie() << "!" << endl;

    int ruchy = 0;
    while (true) {
        cout << "\n=== Kolej: " << aktywny->getImie() << " ===" << endl;
        if (zBotem && aktywny == gracz2) {
            akcjaBota(aktywny, wrog);
        } else {
            akcja(aktywny, wrog);
        }
        ruchy++;
        if (!wrog->zyje()) break;

        aktywny->pasywna();

        Postac* pomoc = aktywny;
        aktywny = wrog;
        wrog = pomoc;
    }

    Postac* zwyciezca = gracz1;
    if (!gracz1->zyje()) zwyciezca = gracz2;
    int tury = (ruchy + 1) / 2;
    pokazKoniec(zwyciezca, tury);

    delete gracz1;
    delete gracz2;
}

int main() {
    srand(time(0));

    while (true) {
        cout << "\n=== ObjectFighters(); ===" << endl;
        cout << "1. Walka dwoch graczy" << endl;
        cout << "2. Walka z botem" << endl;
        cout << "3. Wyjscie" << endl;
        cout << "Wybor: ";
        int wybor = wczytaj(1, 3);
        if (wybor == 3) break;
        graj(wybor == 2);
    }

    cout << "Dzieki za gre w ObjectFighters();" << endl;
    return 0;
}
