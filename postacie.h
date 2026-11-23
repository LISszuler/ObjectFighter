// ============================================================
// postacie.h - plik naglowkowy
// Tutaj opisujemy klasy postaci: Postac, Wojownik i Lucznik.
// Samo dzialanie metod jest w pliku postacie.cpp.
// ============================================================

#pragma once   // ten plik zostanie wczytany tylko raz, nawet jesli kilka plikow go wlaczy

#include <string>        // potrzebne do typu string (tekst)
#include <iostream>      // potrzebne do wypisywania na ekran (ostream)
#include "przedmioty.h"  // Postac ma w sobie Ekwipunek, wiec musimy znac jego pelna definicje

using namespace std;   // dzieki temu piszemy string zamiast std::string

// ============================================================
// POSTACIE
// ============================================================

// Klasa bazowa dla wszystkich postaci (Wojownik i Lucznik dziedzicza po niej)
class Postac {
private:
    string imie;      // imie postaci
    int zycie;             // aktualne zycie
    int zycieMax;          // maksymalne zycie
    int atak;              // sila ataku
    bool specjalnaUzyta;   // czy zdolnosc specjalna zostala juz uzyta (true/false)
    Ekwipunek ekwipunek;   // kazda postac MA ekwipunek (to jest tzw. kompozycja)

public:
    Postac(string imiePostaci, int startoweZycie, int startowyAtak);   // konstruktor parametryczny
    virtual ~Postac() {}                             // wirtualny destruktor

    // gettery - tylko odczyt pol prywatnych
    string getImie() const;
    int getZycie() const;
    int getZycieMax() const;
    int getAtak() const;
    bool getSpecjalnaUzyta() const;
    Ekwipunek& getEkwipunek();   // zwraca ekwipunek (przez referencje, zeby mozna go zmieniac)

    // settery - zmiana pol prywatnych
    void setImie(string noweImie);
    void setZycie(int noweZycie);
    void setAtak(int nowyAtak);
    void setSpecjalnaUzyta(bool uzyta);

    void atakuj(Postac& cel);   // atak podstawowy (bez broni) - dziala zawsze
    bool zyje() const;   // zwraca true, jesli postac ma wiecej niz 0 zycia

    // Przeciazone operatory
    bool operator==(const Postac& inna) const;   // dwie postacie sa rowne, gdy maja to samo imie
    Postac& operator-=(int obrazenia);           // postac -= 10  odejmuje 10 zycia

    // Metody wirtualne - kazda klasa pochodna robi to po swojemu
    virtual void pasywna() = 0;                    // zdolnosc pasywna
    virtual void specjalna(Postac& cel) = 0;       // zdolnosc specjalna (raz na mecz)
    virtual string nazwaKlasy() const = 0;    // np. "Wojownik"
    virtual string nazwaSpecjalnej() const = 0;   // nazwa zdolnosci specjalnej
};

// Przeciazony operator <<. Dzieki niemu mozemy napisac: cout << postac
// (to zwykla funkcja, a nie metoda klasy, bo po lewej stronie stoi cout)
ostream& operator<<(ostream& os, const Postac& p);

// Wojownik - dziedziczy po Postac
class Wojownik : public Postac {
public:
    Wojownik();                     // konstruktor domyslny (bez imienia)
    Wojownik(string imieWojownika);     // konstruktor parametryczny (z imieniem)
    void pasywna() override;
    void specjalna(Postac& cel) override;
    string nazwaKlasy() const override;
    string nazwaSpecjalnej() const override;
};

// Lucznik - tez dziedziczy po Postac
class Lucznik : public Postac {
public:
    Lucznik();
    Lucznik(string imieLucznika);
    void pasywna() override;
    void specjalna(Postac& cel) override;
    string nazwaKlasy() const override;
    string nazwaSpecjalnej() const override;
};

