// ============================================================
// przedmioty.h - plik naglowkowy
// Tutaj opisujemy klasy zwiazane z przedmiotami:
// Przedmiot, Bron, Mikstura oraz Ekwipunek.
// Samo dzialanie metod jest w pliku przedmioty.cpp.
// ============================================================

#pragma once   // ten plik zostanie wczytany tylko raz, nawet jesli kilka plikow go wlaczy

#include <string>    // potrzebne do typu string (tekst)

using namespace std;   // dzieki temu piszemy string zamiast std::string (we wszystkich plikach, ktore wczytaja ten naglowek)

// Zapowiedz klasy Postac. Klasa Przedmiot uzywa Postaci (w metodzie uzyj),
// ale Postac jest zdefiniowana w innym pliku (postacie.h), wiec musimy
// kompilatorowi powiedziec, ze taka klasa istnieje.
class Postac;

// ============================================================
// PRZEDMIOTY
// ============================================================

// Klasa bazowa dla wszystkich przedmiotow (bron i mikstura dziedzicza po niej)
class Przedmiot {
private:                  // private = pole widoczne tylko wewnatrz tej klasy
    string nazwa;    // nazwa przedmiotu, np. "Miecz"

public:                   // public = to moze uzywac kazdy
    Przedmiot(string nowaNazwa);   // konstruktor - tworzy przedmiot o podanej nazwie
    virtual ~Przedmiot() {}         // wirtualny destruktor - potrzebny, zeby delete
                                    // usuwalo wlasciwy typ (Bron albo Mikstura)

    string getNazwa() const;   // getter - zwraca nazwe (const = nic nie zmienia)

    // Metody wirtualne z "= 0" (czysto wirtualne) - kazdy przedmiot MUSI je
    // zrobic po swojemu. Przez to klasa Przedmiot jest abstrakcyjna
    // (nie da sie stworzyc zwyklego Przedmiotu, tylko Bron albo Miksture).
    virtual void uzyj(Postac& wlasciciel, Postac& wrog) = 0;  // co robi przedmiot
    virtual string opis() const = 0;                     // tekst opisujacy przedmiot
    virtual bool jestBronia() const = 0;                      // czy to bron?
};

// Bron - dziedziczy po Przedmiot (public Przedmiot)
class Bron : public Przedmiot {
private:
    int bonus;   // ile ta bron dodaje do ataku

public:
    Bron(string nazwaBroni, int bonusBroni);                  // konstruktor
    void uzyj(Postac& wlasciciel, Postac& wrog) override;  // override = nadpisujemy metode z klasy bazowej
    string opis() const override;
    bool jestBronia() const override;
};

// Mikstura - tez dziedziczy po Przedmiot
class Mikstura : public Przedmiot {
private:
    int leczenie;   // ile zycia przywraca mikstura

public:
    Mikstura(string nazwaMikstury, int ileLeczy);
    void uzyj(Postac& wlasciciel, Postac& wrog) override;
    string opis() const override;
    bool jestBronia() const override;
};

// ============================================================
// EKWIPUNEK - lista przedmiotow nalezaca do postaci (max 5 sztuk)
// ============================================================
const int MAX_MIEJSC = 5;   // ile przedmiotow maksymalnie miesci sie w ekwipunku

class Ekwipunek {
private:
    Przedmiot* przedmioty[MAX_MIEJSC];   // tablica WSKAZNIKOW na przedmioty
                                         // (wskazniki, bo w jednej tablicy sa i bronie, i mikstury)
    int liczba;                          // ile miejsc w tablicy jest teraz zajetych

public:
    Ekwipunek();    // konstruktor
    ~Ekwipunek();   // destruktor - usunie wszystkie przedmioty z pamieci

    void dodaj(Przedmiot* nowy);                  // DODAWANIE przedmiotu
    void usun(int numer);                         // USUWANIE przedmiotu
    Przedmiot* znajdz(string nazwa) const;   // WYSZUKIWANIE po nazwie (nullptr jak nie ma)
    Przedmiot* pobierz(int numer) const;          // zwraca przedmiot z danego miejsca

    int ile() const;           // ile przedmiotow jest teraz w ekwipunku
    int getMaxMiejsc() const;  // ile miejsc ma ekwipunek
    void wypisz() const;       // wypisuje zawartosc ekwipunku na ekran
};

