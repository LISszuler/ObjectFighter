#pragma once
#include <string>
#include <iostream>
#include "przedmioty.h"

using namespace std;


class Postac {
    private:
        string imie;
        int zycie;
        int zycieMax;
        int atak;
        bool specjalnaUzyta;
        Ekwipunek ekwipunek;
    public:
        Postac(string imiePostaci, int startoweZycie, int startowyAtak);
        virtual ~Postac() {}

        string getImie() const;
        int getZycie() const;
        int getZycieMax() const;
        int getAtak() const;
        bool getSpecjalnaUzyta() const;
        Ekwipunek& getEkwipunek();

        void setImie(string noweImie);
        void setZycie(int noweZycie);
        void setAtak(int nowyAtak);
        void setSpecjalnaUzyta(bool uzyta);

        void atakuj(Postac& cel);
        bool zyje() const;

        bool operator==(const Postac& inna) const;
        Postac& operator-=(int obrazenia);

        virtual void pasywna() = 0;
        virtual void specjalna(Postac& cel) = 0;
        virtual string nazwaKlasy() const = 0;
        virtual string nazwaSpecjalnej() const = 0;
};

ostream& operator<<(ostream& os, const Postac& p);

class Wojownik : public Postac {
    public:
        Wojownik();
        Wojownik(string imieWojownika);
        void pasywna() override;
        void specjalna(Postac& cel) override;
        string nazwaKlasy() const override;
        string nazwaSpecjalnej() const override;
};

class Lucznik : public Postac {
public:
    Lucznik();
    Lucznik(string imieLucznika);
    void pasywna() override;
    void specjalna(Postac& cel) override;
    string nazwaKlasy() const override;
    string nazwaSpecjalnej() const override;
};