#include "przedmioty.h"
#include "postacie.h"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

using namespace std;

Przedmiot::Przedmiot(string nowaNazwa) {
    nazwa = nowaNazwa;
}

string Przedmiot::getNazwa() const {
    return nazwa;
}

Bron::Bron(string nazwaBroni, int bonusBroni) : Przedmiot(nazwaBroni) {
    bonus = bonusBroni;
}

void Bron::uzyj(Postac& wlasciciel, Postac& wrog) {
    int obrazenia = wlasciciel.getAtak() + bonus + rand() % 4;
    wrog -= obrazenia;
    cout << wlasciciel.getImie() << " atakuje " << wrog.getImie()
         << " uzywajac: " << getNazwa()
         << " i zadaje " << obrazenia << " obrazen!" << endl;
}

string Bron::opis() const {
    return getNazwa() + " (+" + to_string(bonus) + " do ataku)";
}

bool Bron::jestBronia() const {
    return true;
}

Mikstura::Mikstura(string nazwaMikstury, int ileLeczy) : Przedmiot(nazwaMikstury) {
    leczenie = ileLeczy;
}

void Mikstura::uzyj(Postac& wlasciciel, Postac& /*wrog*/) {
    int stare = wlasciciel.getZycie();
    wlasciciel.setZycie(stare + leczenie);
    cout << wlasciciel.getImie() << " pije: " << getNazwa()
         << " i leczy sie o " << wlasciciel.getZycie() - stare << " zycia." << endl;
}

string Mikstura::opis() const {
    return getNazwa() + " (+" + to_string(leczenie) + " zycia)";
}

bool Mikstura::jestBronia() const {
    return false;
}

Ekwipunek::Ekwipunek() {
    liczba = 0;
    for (int i = 0; i < MAX_MIEJSC; i++) {
        przedmioty[i] = nullptr;
    }
}

Ekwipunek::~Ekwipunek() {
    for (int i = 0; i < ile(); i++) {
        delete przedmioty[i];
    }
}

int Ekwipunek::ile() const {
    return liczba;
}

int Ekwipunek::getMaxMiejsc() const {
    return MAX_MIEJSC;
}

Przedmiot* Ekwipunek::znajdz(string nazwa) const {
    for (int i = 0; i < ile(); i++) {
        if (przedmioty[i]->getNazwa() == nazwa) {
            return przedmioty[i];
        }
    }
    return nullptr;
}

Przedmiot* Ekwipunek::pobierz(int numer) const {
    if (numer < 0 || numer >= ile()) {
        throw runtime_error("Nie ma takiego przedmiotu w ekwipunku!");
    }
    return przedmioty[numer];
}

void Ekwipunek::dodaj(Przedmiot* nowy) {
    if (ile() >= MAX_MIEJSC) {
        throw runtime_error("Ekwipunek jest pelny!");
    }

    if (nowy->jestBronia() && znajdz(nowy->getNazwa()) != nullptr) {
        throw runtime_error("Masz juz ta bron: " + nowy->getNazwa());
    }

    przedmioty[liczba] = nowy;
    liczba++;
}

void Ekwipunek::usun(int numer) {
    Przedmiot* p = pobierz(numer);
    delete p;

    for (int i = numer; i < liczba - 1; i++) {
        przedmioty[i] = przedmioty[i + 1];
    }

    liczba--;
    przedmioty[liczba] = nullptr;
}

void Ekwipunek::zamien(int numer, Przedmiot* nowy) {
    Przedmiot* stary = pobierz(numer);

    if (nowy->jestBronia()) {
        Przedmiot* istniejacy = znajdz(nowy->getNazwa());
        if (istniejacy != nullptr && istniejacy != stary) {
            throw runtime_error("Masz juz ta bron: " + nowy->getNazwa());
        }
    }

    delete stary;
    przedmioty[numer] = nowy;
}

void Ekwipunek::wypisz() const {
    for (int i = 0; i < ile(); i++) {
        cout << "  " << i + 1 << ". " << przedmioty[i]->opis() << endl;
    }
}