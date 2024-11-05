#ifndef FAJLKEZELO_H
#define FAJLKEZELO_H

typedef struct Asztal{
    int id, ferohely, x, y, szel, mag, rendelesszam; //az asztal férőhelyeinek száma, az alaprajzon elfoglalt bal felső sarok x, y koordinátája és az asztal fizikai méretei
    bool foglalt;
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

typedef struct RendelesLista{
    Rendeles **rendelesek;
    int sorszam;
    int oszlopszam;
}RendelesLista;

typedef struct AsztalLista {
    Asztal *adat;
    int meret;
} AsztalLista;

bool alaprajzLefoglal(char **alaprajz, int szelesseg, int magassag);


#endif