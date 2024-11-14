#ifndef FAJLKEZELO_H
#define FAJLKEZELO_H

    typedef struct Asztal{
        int id, ferohely, x, y, szel, mag, rendelesszam; //az asztal férőhelyeinek száma, az alaprajzon elfoglalt bal felső sarok x, y koordinátája és az asztal fizikai méretei
        bool foglalt;
        struct Asztal *kov;
    }Asztal;


    typedef struct MenuElem{
        int id;
        char *nev;
        int ar;
        struct MenuElem *kovetkezo;
    }MenuElem;

    typedef struct Rendeles{
        MenuElem *termekek;
        bool lezarva;
        int osszeg;

    }Rendeles;

    typedef struct Alaprajz{
        char **adat;
        int szelesseg;
        int magassag;
    }Alaprajz;

    Asztal *asztalLetrehoz(int id, int ferohely, int x, int y, int szelesseg, int magassag);
    Alaprajz *alaprajzBeolvas(Asztal **asztalok);
    bool menuBeolvas(char *fajlnev ,MenuElem **menu);
    void alaprajzFelszabadit(char **alaprajz, int sor);
    void alaprajzKiir(char **alaprajz, int mag);
    Asztal *asztalBeolvas(Alaprajz *alaprajz, Asztal *asztalok);
    void menuFree(MenuElem **menu);

#endif