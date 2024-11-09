#ifndef RENDELESKEZELO_H
#define RENDELESKEZELO_H
    #include "fajlkezelo.h"
    Rendeles **rendelesekLetrehoz(int sorok, int oszlopok);
    bool rendelesHozzaad(Rendeles **rendelesek, int asztal, int ujMeret);
    bool rendelesElvesz(Rendeles **rendelesek, int asztal, int ujMeret);
    void rendelesekKiir(Rendeles **rendelesek, int sor, int oszlop);
    void rendelesFelszabadit(Rendeles **rendelesek, Asztal *asztalok);
    bool termekElvesz(Rendeles **rendelesek, int sor, int oszlop);
    bool menuBeolvas(char *fajlnev ,MenuElem **menu);
    void ujAsztal(Asztal *asztalok, Rendeles **rendelesek);


#endif