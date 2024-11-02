#ifndef FAJLKEZELO_H
#define FAJLKEZELO_H

typedef struct Asztal{
    int id, ferohely, x, y, szel, mag; //az asztal férőhelyeinek száma, az alaprajzon elfoglalt bal felső sarok x, y koordinátája és az asztal fizikai méretei
    bool foglalt;
}Asztal;

typedef struct Rendeles{
    char *termekek;
    bool lezarva;
    int osszeg;

}Rendeles;

bool alaprajzLefoglal(char **alaprajz, int szelesseg, int magassag);


#endif