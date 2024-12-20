#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "fajlkezelo.h"
#include "rendeleskezelo.h"
#include "debugmalloc.h"


/*Hozzaad egy rendelest a megadott asztalhoz, @return semmi*/
bool rendelesHozzaad(Rendeles **rendelesek,Asztal *asztalok, int asztalId, int ujMeret) {
    Rendeles *ujsor = (Rendeles*) malloc((ujMeret+1)*sizeof(Rendeles));
    if(ujsor == NULL) return false;

    for (int i = 0; i < ujMeret; i++) {
        ujsor[i] = rendelesek[asztalId][i];
        //menuFree(&(rendelesek[asztalId][i].termekek));
        //free(&(rendelesek[asztalId][i]));
    }
    ujsor[ujMeret].lezarva = false;
    ujsor[ujMeret].osszeg = 0;
    ujsor[ujMeret].termekek = NULL;

    if (rendelesek[asztalId] != NULL) {
        free(rendelesek[asztalId]);
    }
    rendelesek[asztalId] = ujsor;
    return true;
    
}

/*visszaadja az asztalok lista hosszat*/
int len(Asztal *asztalok){
    int meret = 0;
    Asztal *mozgo = asztalok;

    while (mozgo != NULL) {
        meret++;
        mozgo = mozgo->kov;
    }
    return meret;
}

/*megkeresi az adott indexu asztalt, majd visszaadja az adott asztalra mutato pointert*/
Asztal *keres(Asztal *asztalok, int index){
    Asztal *mozgo = asztalok;
    for(int i = 0; i < index; i++) mozgo = mozgo->kov;
    return mozgo;
}

/*megnyit egy uj asztalt, majd hozzaad egy rendelest */
void ujAsztal(Asztal *asztalok, Rendeles **rendelesek){
    
    int meret = len(asztalok);

    
    int in;
    printf("Melyik asztalt szeretne megnyitni? (1-%d), vagy -1 a visszalepeshez)\n", meret);
    in = scanf_int("");
    while(in > meret || (in < 1 && in != -1)){
        in = scanf_int("A megadott szam a tartmoanyon kivul esik, valasszon masikat!\n");
    }
    if(in == -1){
        return;
    }
    in--;
    Asztal *kivalasztott = keres(asztalok, in);
    while(kivalasztott->foglalt){
        printf("A kivalasztott asztal mar foglalt, valasszon masikat!\n");
        do{
            in = scanf_int("");
        }while(in > meret && in < 1);
        if(in == -1){
            return;
        }
        in--;
        kivalasztott = keres(asztalok, in);
    }
    kivalasztott->rendelesszam++;
    if (rendelesHozzaad(rendelesek, asztalok, in, kivalasztott->rendelesszam) == false) {
        printf("Hiba: nem sikerult rendelest hozzaadni.\n");
        return;
    }

    // Rendelés alapértelmezett inicializálásas
    rendelesek[in][kivalasztott->rendelesszam-1].osszeg = 0;
    rendelesek[in][kivalasztott->rendelesszam-1].lezarva = false;
    kivalasztott->foglalt = true;

    return;
}

/*letrehozza a rendelesek tombot @return a rendelesekre mutato pointer*/
Rendeles **rendelesekLetrehoz(int sorok, int oszlopok) {
    Rendeles **rendelesek = (Rendeles **)malloc(sorok * sizeof(Rendeles *));
    if(rendelesek == NULL) return NULL;
    for (int i = 0; i < sorok; i++) {
        rendelesek[i] = (Rendeles *)malloc(oszlopok * sizeof(Rendeles));
        if(rendelesek[i] == NULL) return NULL;
        for (int j = 0; j < oszlopok; j++) {
            rendelesek[i][j].termekek = NULL;
            rendelesek[i][j].lezarva = false;
            rendelesek[i][j].osszeg = 0;
        }
    }
    return rendelesek;
}

/*Torli a megadott sor utolso rendeleset*/
bool rendelesElvesz(Rendeles **rendelesek, int asztal, int ujMeret){ //nem használt függvény????
    Rendeles *ujsor = (Rendeles*) malloc((ujMeret-1)*sizeof(Rendeles));
    if(ujsor == NULL) return false;

    for (int i = 0; i < ujMeret; i++) {
        ujsor[i] = rendelesek[asztal][i];
    }

    free(rendelesek[asztal]);
    rendelesek[asztal] = ujsor;
    return true;

}

/*Kiirja a rendeleseket asztalokra, majd azokon belul rendelesekre bontva*/
void rendelesekKiir(Rendeles **rendelesek, Asztal *asztalok) {
    int sor = 0;
    int oszlop = 0;
    sor = len(asztalok);

    system("cls");
    for (int i = 0; i < sor; i++) {
        printf("Asztal %d\n", i);
        oszlop = keres(asztalok, i)->rendelesszam;
        for(int j = 0; j < oszlop; j++){
            printf("\t- Rendeles %d: osszeg: %d, lezarva: %s\n", j, rendelesek[i][j].osszeg, rendelesek[i][j].lezarva? "igaz":"hamis");
            MenuElem *jelenlegi = rendelesek[i][j].termekek;
            if(!rendelesek[i][j].lezarva){
                while (jelenlegi != NULL) {
                    printf("\t\t- Termek neve: %s, ara: %d\n", jelenlegi->nev, jelenlegi->ar);
                    jelenlegi = jelenlegi->kovetkezo;
                }
            }
            printf("\n");
        }
    }
}

/* Felszabaditja a rendelesek tombhoz lefoglalt memoriat*/
void rendelesFree(Rendeles **rendelesek, Asztal *asztalok) {
    int meret = len(asztalok);

    for (int i = 0; i < meret; i++) {
        Asztal *kivalasztott = keres(asztalok, i);
        for (int j = 0; j < kivalasztott->rendelesszam; j++) {
            MenuElem *jelenlegi = rendelesek[i][j].termekek;
            while (jelenlegi != NULL) {
                MenuElem *next = jelenlegi->kovetkezo;
                free(jelenlegi);
                jelenlegi = next;
            }
        }
        free(rendelesek[i]);

    }
    free(rendelesek);
}


/*elveszi az utolso termeket a rendeles termekei kozul @return false ha baj van, true ha minden ok*/
bool termekElvesz(Rendeles **rendelesek, int sor, int oszlop){
    if(rendelesek[sor][oszlop].termekek == NULL) return false;
    MenuElem *menu = rendelesek[sor][oszlop].termekek;

    if(menu->kovetkezo == NULL){
//        free(menu->nev);
        free(menu);
    }

    MenuElem *mozgo = menu;
    while(mozgo->kovetkezo->kovetkezo != NULL){
        mozgo = mozgo->kovetkezo;
    }
    MenuElem *utolso = mozgo->kovetkezo;
    rendelesek[sor][oszlop].osszeg -= utolso->ar;
    mozgo->kovetkezo = NULL;
    //free(mozgo->nev);
    free(utolso);
    return true;

}




/* Termek hozzaadasa egy rendeles termekeihez @return ha hiba tortent, false, egyeb esetben true*/
bool termekHozzaad(Rendeles **rendelesek, int sor, int oszlop, MenuElem *termek){
    MenuElem *ujTermek = (MenuElem*) malloc(sizeof(MenuElem));
    if(ujTermek == NULL) return false;

    
    ujTermek->nev = strdup(termek->nev);
    ujTermek->ar = termek->ar;
    ujTermek->kovetkezo = NULL;

    if (rendelesek[sor][oszlop].termekek == NULL){
        rendelesek[sor][oszlop].termekek = ujTermek;
    }else{
        MenuElem *mozgo = rendelesek[sor][oszlop].termekek;

        while(mozgo->kovetkezo != NULL){
            mozgo = mozgo->kovetkezo;
        }
        mozgo->kovetkezo = ujTermek;
    }

    rendelesek[sor][oszlop].osszeg += ujTermek->ar;
    return true;
}

//kinyomtatja a rendeleshez tartozo nyugtat
void nyugtaNyomtat(Asztal *asztal, Rendeles rendeles){
    time_t timeNow = time(NULL);
    struct tm tm = *localtime(&timeNow);

    printf("-------------------------------\n");
    printf("           NYUGTA             \n");
    printf("-------------------------------\n");
    printf("Asztal azonositoja: %d\n", asztal->id);
    printf("Foglalasszam: %d\n\n", asztal->rendelesszam);
    printf("Idopont: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    printf("Tetel neve         Ar (Ft)\n");
    printf("-------------------------------\n");

    MenuElem *aktualis = rendeles.termekek;
    while (aktualis != NULL) {
        printf("%-18s %5d\n", aktualis->nev, aktualis->ar);
        aktualis = aktualis->kovetkezo;
    }
    printf("-------------------------------\n");
    printf("Osszesen:          %5d Ft\n", rendeles.osszeg);
    printf("-------------------------------\n");
    printf("     Koszonjuk a vasarlast!    \n");
    printf("-------------------------------\n");


}


/*Bemenetkent az asztalok es rendelesek listajat, valamint az etterem menujet keri. Modositja a megadott asztal utolso rendeleset*/
void rendelesKezel(Asztal *asztalok, Rendeles **rendelesek, MenuElem *menu){
    printf("Melyik asztal rendeleset szeretne kezelni? (1-%d), -1 a kilepeshez\n", len(asztalok));
    int asztalId = 0;
    asztalId = scanf_int("");
    while(asztalId > len(asztalok) || (asztalId < 1 && asztalId != -1)){
        asztalId = scanf_int("A megadott szam a tartmoanyon kivul esik, valasszon masikat!\n");
    }
    if(asztalId == -1) return;
    asztalId--;
    while (keres(asztalok, asztalId) == NULL || keres(asztalok, asztalId)->rendelesszam == 0)
    {
        printf("A kivalasztott asztalhoz nem tartozik rendeles, valasszon masikat!");
        printf("Melyik asztal rendeleset szeretne kezelni? (1-%d), -1 a kilepeshez\n", len(asztalok));
        asztalId = scanf_int("");
        while(asztalId > len(asztalok) || (asztalId < 1 && asztalId != -1)){
            asztalId = scanf_int("A megadott szam a tartmoanyon kivul esik, valasszon masikat!\n");
        }
        if(asztalId == -1) return;
        asztalId--;
    }
    
    
    // Rendeles jelenlegiRendeles = rendelesek[--asztalId][keres(asztalok, asztalId)->rendelesszam];

    int valasztas;
    printf("Mit szeretne tenni?\n\t-1. Termek hozzaadasa\n\t-2. Utolso termek elvetele a rendelesbol\n\t-3. Rendeles lezarasa\n");
    valasztas = scanf_int("A valasztott menupont: ");
    switch (valasztas)
    {
        case 1:
            int termekId = 0;
            
            do
            {
                printf("Irja be a termek azonositojat, ha vegzett, -1\n");
                termekId = scanf_int("");
                if(termekId == -1) break;
                
                MenuElem *jelenlegi = menu;
                while(jelenlegi->id != termekId) jelenlegi = jelenlegi->kovetkezo;
                if(!termekHozzaad(rendelesek, asztalId, keres(asztalok, asztalId)->rendelesszam-1, jelenlegi)){ //Ha memfoglalasi hiba van, visszadobja a menube
                    printf("Termek hozzaadasa nem sikerult, probalja meg kesobb!");
                    return;
                };
            } while (termekId != -1);
            
        break;
        case 2:
            if(!termekElvesz(rendelesek, asztalId, (keres(asztalok, asztalId)->rendelesszam)-1)){ //hiba eseten visszadob a menube
                printf("Termek elvetele nem sikerult!");
                return;
            };
        break;
        case 3:
            nyugtaNyomtat(keres(asztalok, asztalId), rendelesek[asztalId][(keres(asztalok, asztalId)->rendelesszam)-1]);
            rendelesek[asztalId][(keres(asztalok, asztalId)->rendelesszam)-1].lezarva = true;
            keres(asztalok, asztalId)->foglalt = false;
        break;
        
        default:
            printf("Nem megfelelo menupont!");
        break;
    }
}

void asztalFree(Asztal **elso){
    Asztal *elem = *elso;

        while(elem != NULL){
        Asztal *kovetkezo = elem->kov;
        free(elem);
        elem = kovetkezo;
    }

}

//lezarja a megmaradt rendeleseket es kiirja a napi foglaltsagi terkepet
void zaras(Rendeles **rendelesek, Asztal *asztalok, char *mentesFajlnev){
    int sorok = len(asztalok);
    for(int i = 0; i < sorok; i++){
        Asztal *asztal = keres(asztalok, i);
        int oszlopok = asztal->rendelesszam;
        for(int j = 0; j <= oszlopok; j++){
            
            if(asztal->rendelesszam !=0 && !rendelesek[i][j].lezarva){
                nyugtaNyomtat(asztal, rendelesek[i][(asztal->rendelesszam)-1]);
                rendelesek[i][(asztal->rendelesszam)-1].lezarva = true;
                asztal->foglalt = false;
            }
        }
    }

    FILE *fp = fopen(mentesFajlnev, "w");
    fprintf(fp, "1");
    fclose(fp);

}
