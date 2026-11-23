// ============================================================
// przedmioty.cpp - plik implementacyjny
// Tutaj jest kod metod klas Przedmiot, Bron, Mikstura i Ekwipunek
// zadeklarowanych w przedmioty.h
// Zapis Klasa::metoda oznacza "metoda nalezaca do klasy Klasa".
// ============================================================

#include "przedmioty.h"   // nasze deklaracje przedmiotow
#include "postacie.h"     // potrzebujemy pelnej klasy Postac (wywolujemy jej metody)
#include <iostream>       // cout - wypisywanie
#include <cstdlib>        // rand() - losowanie liczb
#include <stdexcept>      // runtime_error - wyjatki

using namespace std;      // dzieki temu piszemy cout zamiast std::cout

// ============================================================
// PRZEDMIOT
// ============================================================

// Konstruktor: ustawia nazwe przedmiotu
Przedmiot::Przedmiot(string nowaNazwa) {
    nazwa = nowaNazwa;   // do pola klasy (nazwa) wpisujemy wartosc z parametru (nowaNazwa)
}

// Getter: zwraca nazwe
string Przedmiot::getNazwa() const {
    return nazwa;
}

// ============================================================
// BRON
// ============================================================

// Konstruktor broni. Po dwukropku wywolujemy konstruktor klasy bazowej (Przedmiot)
Bron::Bron(string nazwaBroni, int bonusBroni) : Przedmiot(nazwaBroni) {
    bonus = bonusBroni;   // zapisujemy bonus do ataku
}

// Uzycie broni = atak. Wlasciciel atakuje wroga.
void Bron::uzyj(Postac& wlasciciel, Postac& wrog) {
    // obrazenia = atak postaci + bonus broni + losowa liczba od 0 do 3
    int obrazenia = wlasciciel.getAtak() + bonus + rand() % 4;
    wrog -= obrazenia;   // uzywamy naszego operatora -= (odejmuje wrogowi zycie)
    cout << wlasciciel.getImie() << " atakuje " << wrog.getImie()
         << " uzywajac: " << getNazwa()
         << " i zadaje " << obrazenia << " obrazen!" << endl;
}

// Opis broni, np. "Miecz (+6 do ataku)"
string Bron::opis() const {
    return getNazwa() + " (+" + to_string(bonus) + " do ataku)";   // to_string zamienia liczbe na tekst
}

// To jest bron, wiec zwracamy true
bool Bron::jestBronia() const {
    return true;
}

// ============================================================
// MIKSTURA
// ============================================================

// Konstruktor mikstury
Mikstura::Mikstura(string nazwaMikstury, int ileLeczy) : Przedmiot(nazwaMikstury) {
    leczenie = ileLeczy;   // zapisujemy, ile zycia przywraca mikstura
}

// Uzycie mikstury = leczenie wlasciciela.
// Drugi parametr (wrog) nie jest tu potrzebny, wiec nie dajemy mu nazwy.
void Mikstura::uzyj(Postac& wlasciciel, Postac& /*wrog*/) {
    int stare = wlasciciel.getZycie();             // zapamietujemy zycie przed leczeniem
    wlasciciel.setZycie(stare + leczenie);         // zwiekszamy zycie (setZycie pilnuje maksimum)
    cout << wlasciciel.getImie() << " pije: " << getNazwa()
         << " i leczy sie o " << wlasciciel.getZycie() - stare << " zycia." << endl;
}

// Opis mikstury, np. "Mala mikstura (+15 zycia)"
string Mikstura::opis() const {
    return getNazwa() + " (+" + to_string(leczenie) + " zycia)";
}

// To nie jest bron
bool Mikstura::jestBronia() const {
    return false;
}

// ============================================================
// EKWIPUNEK
// ============================================================

// Konstruktor: na starcie ekwipunek jest pusty
Ekwipunek::Ekwipunek() {
    liczba = 0;                          // zero zajetych miejsc
    for (int i = 0; i < MAX_MIEJSC; i++) {
        przedmioty[i] = nullptr;         // puste miejsca wskazuja na "nic"
    }
}

// Destruktor: usuwa z pamieci wszystkie przedmioty (utworzone przez new)
Ekwipunek::~Ekwipunek() {
    for (int i = 0; i < ile(); i++) {   // przechodzimy po wszystkich przedmiotach
        delete przedmioty[i];           // usuwamy przedmiot z pamieci
    }
}

// Ile przedmiotow jest w ekwipunku
int Ekwipunek::ile() const {
    return liczba;
}

// Ile miejsc ma ekwipunek
int Ekwipunek::getMaxMiejsc() const {
    return MAX_MIEJSC;
}

// WYSZUKIWANIE: szuka przedmiotu o podanej nazwie
Przedmiot* Ekwipunek::znajdz(string nazwa) const {
    for (int i = 0; i < ile(); i++) {
        if (przedmioty[i]->getNazwa() == nazwa) {   // znaleziono
            return przedmioty[i];                   // zwracamy ten przedmiot
        }
    }
    return nullptr;   // nullptr = "nic", czyli nie znaleziono
}

// Zwraca przedmiot z miejsca o danym numerze (liczonym od 0)
Przedmiot* Ekwipunek::pobierz(int numer) const {
    if (numer < 0 || numer >= ile()) {   // numer poza zakresem?
        // throw = rzucamy wyjatek (blad), ktory zlapie catch w main.cpp
        throw runtime_error("Nie ma takiego przedmiotu w ekwipunku!");
    }
    return przedmioty[numer];
}

// DODAWANIE: dodaje przedmiot do ekwipunku
void Ekwipunek::dodaj(Przedmiot* nowy) {
    // Regula 1: nie moze byc wiecej niz 5 przedmiotow
    if (ile() >= MAX_MIEJSC) {
        throw runtime_error("Ekwipunek jest pelny!");
    }
    // Regula 2: bronie nie moga sie powtarzac (mikstury moga)
    if (nowy->jestBronia() && znajdz(nowy->getNazwa()) != nullptr) {
        throw runtime_error("Masz juz ta bron: " + nowy->getNazwa());
    }
    przedmioty[liczba] = nowy;   // wstawiamy na pierwsze wolne miejsce
    liczba++;                    // mamy o jeden przedmiot wiecej
}

// USUWANIE: usuwa przedmiot z danego miejsca
void Ekwipunek::usun(int numer) {
    Przedmiot* p = pobierz(numer);                  // pobierz rzuci wyjatek, jesli numer jest zly
    delete p;                                       // usuwamy przedmiot z pamieci

    // Przesuwamy kolejne przedmioty o jedno miejsce w lewo, zeby nie zostala dziura
    for (int i = numer; i < liczba - 1; i++) {
        przedmioty[i] = przedmioty[i + 1];
    }
    liczba--;                       // mamy o jeden przedmiot mniej
    przedmioty[liczba] = nullptr;   // ostatnie miejsce jest teraz puste
}

// Wypisuje ekwipunek jako ponumerowana liste
void Ekwipunek::wypisz() const {
    for (int i = 0; i < ile(); i++) {
        cout << "  " << i + 1 << ". " << przedmioty[i]->opis() << endl;   // i+1, bo gracz liczy od 1
    }
}
