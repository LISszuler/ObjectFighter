#include "postacie.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Postac::Postac(string imiePostaci, int startoweZycie, int startowyAtak) {
    imie = imiePostaci;
    zycie = startoweZycie;
    zycieMax = startoweZycie;
    atak = startowyAtak;
    specjalnaUzyta = false;
}

string Postac::getImie() const { return imie; }
int Postac::getZycie() const { return zycie; }
int Postac::getZycieMax() const { return zycieMax; }
int Postac::getAtak() const { return atak; }
bool Postac::getSpecjalnaUzyta() const { return specjalnaUzyta; }
Ekwipunek& Postac::getEkwipunek() { return ekwipunek; }

void Postac::setImie(string noweImie) {
    imie = noweImie;
}

void Postac::setZycie(int noweZycie) {
    if (noweZycie < 0) noweZycie = 0;
    if (noweZycie > zycieMax) noweZycie = zycieMax;
    zycie = noweZycie;
}

void Postac::setAtak(int nowyAtak) {
    atak = nowyAtak;
}

void Postac::setSpecjalnaUzyta(bool uzyta) {
    specjalnaUzyta = uzyta;
}

bool Postac::zyje() const {
    return zycie > 0;
}

void Postac::atakuj(Postac& cel) {
    int obrazenia = atak + rand() % 4;
    cel -= obrazenia;
    cout << imie << " atakuje " << cel.getImie()
         << " i zadaje " << obrazenia << " obrazen!" << endl;
}

bool Postac::operator==(const Postac& inna) const {
    return imie == inna.imie;
}

Postac& Postac::operator-=(int obrazenia) {
    setZycie(zycie - obrazenia);
    return *this;
}

ostream& operator<<(ostream& os, const Postac& p) {
    os << p.getImie() << " (" << p.nazwaKlasy() << ")"
       << " | zycie: " << p.getZycie() << "/" << p.getZycieMax()
       << " | atak: " << p.getAtak()
       << " | specjalna: ";
    if (p.getSpecjalnaUzyta()) {
        os << "uzyta";
    } else {
        os << "dostepna";
    }
    return os;
}

Wojownik::Wojownik() : Postac("Wojownik", 120, 12) {}

Wojownik::Wojownik(string imieWojownika) : Postac(imieWojownika, 120, 12) {}

void Wojownik::pasywna() {
    int stare = getZycie();
    setZycie(getZycie() + 3);
    cout << "[Pasywna] " << getImie() << " regeneruje "
         << getZycie() - stare << " zycia." << endl;
}

void Wojownik::specjalna(Postac& cel) {
    int obrazenia = getAtak() * 2;
    cel -= obrazenia;
    setSpecjalnaUzyta(true);
    cout << getImie() << " uzywa szarzy i zadaje " << obrazenia << " obrazen!" << endl;
}

string Wojownik::nazwaKlasy() const { return "Wojownik"; }
string Wojownik::nazwaSpecjalnej() const { return "Szarza (podwojny atak)"; }

Lucznik::Lucznik() : Postac("Lucznik", 90, 15) {}

Lucznik::Lucznik(string imieLucznika) : Postac(imieLucznika, 90, 15) {}

void Lucznik::pasywna() {
    setAtak(getAtak() + 1);
    cout << "[Pasywna] " << getImie() << " skupia sie - atak +1." << endl;
}

void Lucznik::specjalna(Postac& cel) {
    int obrazenia = 3 * (getAtak() / 2);
    cel -= obrazenia;
    setSpecjalnaUzyta(true);
    cout << getImie() << " uzywa deszczu strzal i zadaje " << obrazenia << " obrazen!" << endl;
}

string Lucznik::nazwaKlasy() const { return "Lucznik"; }
string Lucznik::nazwaSpecjalnej() const { return "Deszcz strzal (3 strzaly)"; }
