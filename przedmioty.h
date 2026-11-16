#pragma once
#include <string>

using namespace std;

class Postac;

class Przedmiot {
    private:
        string nazwa;

    public:
        Przedmiot(string nowaNazwa);
        virtual ~Przedmiot() {}

        string getNazwa() const;

        virtual void uzyj(Postac& wlasciciel, Postac& wrog) = 0;
        virtual string opis() const = 0;
        virtual bool jestBronia() const = 0;
};

class Bron : public Przedmiot {
    private:
        int bonus;

    public:
        Bron(string nazwaBroni, int bonusBroni);
        void uzyj(Postac& wlasciciel, Postac& wrog) override;
        string opis() const override;
        bool jestBronia() const override;
};

class Mikstura : public Przedmiot {
    private:
        int leczenie;

    public:
        Mikstura(string nazwaMikstury, int ileLeczy);
        void uzyj(Postac& wlasciciel, Postac& wrog) override;
        string opis() const override;
        bool jestBronia() const override;
};

const int MAX_MIEJSC = 5;

class Ekwipunek {
    private:
        Przedmiot* przedmioty[MAX_MIEJSC];
        int liczba;

    public:
        Ekwipunek();
        ~Ekwipunek();

        void dodaj(Przedmiot* nowy);
        void usun(int numer);
        void zamien(int numer, Przedmiot* nowy);
        Przedmiot* znajdz(string nazwa) const;
        Przedmiot* pobierz(int numer) const;

        int ile() const;
        int getMaxMiejsc() const;
        void wypisz() const;
};