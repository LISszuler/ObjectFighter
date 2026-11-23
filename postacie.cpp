// ============================================================
// postacie.cpp - plik implementacyjny
// Tutaj jest kod metod klas Postac, Wojownik i Lucznik
// zadeklarowanych w postacie.h
// Zapis Klasa::metoda oznacza "metoda nalezaca do klasy Klasa".
// ============================================================

#include "postacie.h"   // nasze deklaracje postaci
#include <iostream>     // cout - wypisywanie
#include <cstdlib>      // rand() - losowanie liczb

using namespace std;    // dzieki temu piszemy cout zamiast std::cout

// ============================================================
// POSTAC
// ============================================================

// Konstruktor: ustawia imie, zycie i atak
Postac::Postac(string imiePostaci, int startoweZycie, int startowyAtak) {
    imie = imiePostaci;
    zycie = startoweZycie;
    zycieMax = startoweZycie;    // na starcie zycie jest rowne maksimum
    atak = startowyAtak;
    specjalnaUzyta = false;      // zdolnosc specjalna jeszcze nie uzyta
}

// Gettery - zwracaja wartosci pol
string Postac::getImie() const { return imie; }
int Postac::getZycie() const { return zycie; }
int Postac::getZycieMax() const { return zycieMax; }
int Postac::getAtak() const { return atak; }
bool Postac::getSpecjalnaUzyta() const { return specjalnaUzyta; }
Ekwipunek& Postac::getEkwipunek() { return ekwipunek; }

// Settery - zmieniaja wartosci pol
void Postac::setImie(string noweImie) {
    imie = noweImie;
}

void Postac::setZycie(int noweZycie) {
    if (noweZycie < 0) noweZycie = 0;                    // zycie nie moze byc ujemne
    if (noweZycie > zycieMax) noweZycie = zycieMax;      // zycie nie moze przekroczyc maksimum
    zycie = noweZycie;
}

void Postac::setAtak(int nowyAtak) {
    atak = nowyAtak;
}

void Postac::setSpecjalnaUzyta(bool uzyta) {
    specjalnaUzyta = uzyta;
}

// Postac zyje, jesli ma wiecej niz 0 zycia
bool Postac::zyje() const {
    return zycie > 0;
}

// Atak podstawowy: dziala zawsze, nie potrzebuje broni ani ekwipunku
void Postac::atakuj(Postac& cel) {
    int obrazenia = atak + rand() % 4;   // sila ataku + losowa liczba od 0 do 3
    cel -= obrazenia;                    // odejmujemy zycie celowi (nasz operator -=)
    cout << imie << " atakuje " << cel.getImie()
         << " i zadaje " << obrazenia << " obrazen!" << endl;
}

// Operator ==: postacie sa rowne, gdy maja to samo imie
bool Postac::operator==(const Postac& inna) const {
    return imie == inna.imie;
}

// Operator -=: odejmuje zycie. Zwraca *this (czyli ta sama postac)
Postac& Postac::operator-=(int obrazenia) {
    setZycie(zycie - obrazenia);   // setZycie pilnuje, zeby zycie nie spadlo ponizej 0
    return *this;
}

// Operator <<: wypisuje postac na ekran, np. cout << postac
ostream& operator<<(ostream& os, const Postac& p) {
    os << p.getImie() << " (" << p.nazwaKlasy() << ")"                      // imie i klasa
       << " | zycie: " << p.getZycie() << "/" << p.getZycieMax()            // zycie
       << " | atak: " << p.getAtak()                                        // atak
       << " | specjalna: ";
    if (p.getSpecjalnaUzyta()) {
        os << "uzyta";
    } else {
        os << "dostepna";
    }
    return os;   // zwracamy os, zeby mozna bylo lancuchowo laczyc <<
}

// ============================================================
// WOJOWNIK
// ============================================================

// Konstruktor domyslny: ustawia domyslne imie "Wojownik" i statystyki 120 zycia, 12 ataku
Wojownik::Wojownik() : Postac("Wojownik", 120, 12) {}

// Konstruktor z imieniem: wywoluje konstruktor Postaci z imieniem gracza
Wojownik::Wojownik(string imieWojownika) : Postac(imieWojownika, 120, 12) {}

// Pasywna zdolnosc Wojownika: regeneracja +3 zycia po kazdej akcji
void Wojownik::pasywna() {
    int stare = getZycie();            // zycie przed leczeniem
    setZycie(getZycie() + 3);          // dodajemy 3 zycia
    cout << "[Pasywna] " << getImie() << " regeneruje "
         << getZycie() - stare << " zycia." << endl;   // ile faktycznie wyleczono
}

// Zdolnosc specjalna Wojownika: Szarza = podwojny atak
void Wojownik::specjalna(Postac& cel) {
    int obrazenia = getAtak() * 2;     // podwojna sila ataku
    cel -= obrazenia;                  // odejmujemy zycie celowi
    setSpecjalnaUzyta(true);           // zapamietujemy, ze zdolnosc zostala uzyta
    cout << getImie() << " uzywa szarzy i zadaje " << obrazenia << " obrazen!" << endl;
}

string Wojownik::nazwaKlasy() const { return "Wojownik"; }
string Wojownik::nazwaSpecjalnej() const { return "Szarza (podwojny atak)"; }

// ============================================================
// LUCZNIK
// ============================================================

// Konstruktor domyslny: imie "Lucznik", 90 zycia, 15 ataku
Lucznik::Lucznik() : Postac("Lucznik", 90, 15) {}

// Konstruktor z imieniem
Lucznik::Lucznik(string imieLucznika) : Postac(imieLucznika, 90, 15) {}

// Pasywna zdolnosc Lucznika: skupienie, atak +1 po kazdej akcji
void Lucznik::pasywna() {
    setAtak(getAtak() + 1);
    cout << "[Pasywna] " << getImie() << " skupia sie - atak +1." << endl;
}

// Zdolnosc specjalna Lucznika: Deszcz strzal = 3 strzaly po pol ataku
void Lucznik::specjalna(Postac& cel) {
    int obrazenia = 3 * (getAtak() / 2);   // trzy strzaly, kazda zadaje polowe ataku
    cel -= obrazenia;
    setSpecjalnaUzyta(true);
    cout << getImie() << " uzywa deszczu strzal i zadaje " << obrazenia << " obrazen!" << endl;
}

string Lucznik::nazwaKlasy() const { return "Lucznik"; }
string Lucznik::nazwaSpecjalnej() const { return "Deszcz strzal (3 strzaly)"; }
