#ifndef RENDELESKEZELO_H
#define RENDELESKEZELO_H
    #include "fajlkezelo.h"


    Rendeles **rendelesekLetrehoz(int sorok, int oszlopok);
    bool rendelesHozzaad(Rendeles **rendelesek, int asztal, int ujMeret);
    bool rendelesElvesz(Rendeles **rendelesek, int asztal, int ujMeret);
    void rendelesekKiir(Rendeles **rendelesek, Asztal *asztalok);
    void rendelesFree(Rendeles **rendelesek, Asztal *asztalok);
    bool termekElvesz(Rendeles **rendelesek, int sor, int oszlop);
    bool termekHozzaad(Rendeles **rendelesek, int sor, int oszlop, MenuElem *termek);

    void ujAsztal(Asztal *asztalok, Rendeles **rendelesek);
    int len(Asztal *asztalok);
    Asztal *keres(Asztal *asztalok, int index);
    void asztalFree(Asztal **elso);
    void rendelesKezel(Asztal *asztalok, Rendeles **rendelesek, MenuElem *menu);

#endif