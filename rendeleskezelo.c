#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fajlkezelo.h"
#include "rendeleskezelo.h"
#include "debugmalloc.h"
/*letrehozza a rendelesek tombot @return a rendelesekre mutato pointer*/
Rendeles **rendelesekLetrehoz(int sorok, int oszlopok) {
    Rendeles **rendelesek = (Rendeles **)malloc(sorok * sizeof(Rendeles *));
    for (int i = 0; i < sorok; i++) {
        rendelesek[i] = (Rendeles *)malloc(oszlopok * sizeof(Rendeles));
        for (int j = 0; j < oszlopok; j++) {
            rendelesek[i][j].termekek = NULL;
            rendelesek[i][j].lezarva = false;
            rendelesek[i][j].osszeg = 0;
        }
    }
    return rendelesek;
}

/*Hozzaad egy rendelest a megadott asztalhoz, @return semmi*/
bool rendelesHozzaad(Rendeles **rendelesek, int asztal, int ujMeret) {
    ujMeret++;
    Rendeles *ujsor = (Rendeles*) malloc(ujMeret*sizeof(Rendeles));
    if(ujsor == NULL) return false;

    for (int i = 0; i < ujMeret; i++) {
        ujsor[i] = rendelesek[asztal][i];
    }
    ujsor[ujMeret].lezarva = false;
    ujsor[ujMeret].osszeg = 0;
    ujsor[ujMeret].termekek = NULL;


    free(rendelesek[asztal]);
    rendelesek[asztal] = ujsor;
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

    //system("cls");
    printf("Melyik asztalt szeretne megnyitni? (1-%d), vagy -1 a visszalepeshez)\n", meret);
    //input error kezeles.....
    int in;
    scanf("%i", &in);
    if(in == -1){
        return;
    }
    in--;
    Asztal *kivalasztott = keres(asztalok, in);
    while(kivalasztott->foglalt){
        printf("A kivalasztott asztal mar foglalt, valasszon masikat!\n");
        scanf("%i", &in);
        if(in == -1){
            return;
        }
        in--;
        kivalasztott = keres(asztalok, in);
    }
    if(kivalasztott->rendelesszam != 0){
        rendelesHozzaad(rendelesek, kivalasztott->id, kivalasztott->rendelesszam+1);
        kivalasztott->rendelesszam++;
    }
    rendelesek[in][kivalasztott->rendelesszam].osszeg = 0;
    rendelesek[in][kivalasztott->rendelesszam].lezarva = false;
    kivalasztott->foglalt = true;
    kivalasztott->rendelesszam++;
    return;
}

/*Torli a megadott sor utolso termeket*/
bool rendelesElvesz(Rendeles **rendelesek, int asztal, int ujMeret){
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

    for (int i = 0; i < sor; i++) {
        printf("Asztal %d\n", i);
        oszlop = keres(asztalok, i)->rendelesszam;
        for(int j = 0; j < oszlop; j++){
            printf("Rendeles %d: osszeg: %d, lezarva: %s\n", j, rendelesek[i][j].osszeg, rendelesek[i][j].lezarva? "true":"false");
            MenuElem *jelenlegi = rendelesek[i][j].termekek;
            while (jelenlegi != NULL) {
                printf("  - Termek neve: %s, ara: %d\n", jelenlegi->nev, jelenlegi->ar);
                jelenlegi = jelenlegi->kovetkezo;
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
            printf("%d", rendelesek[i][j].osszeg);
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
    MenuElem *mozgo = rendelesek[sor][oszlop].termekek;
    MenuElem *elozo = NULL;

    while(mozgo->kovetkezo != NULL){
        elozo = mozgo;
        mozgo = mozgo->kovetkezo;
    }

    if (elozo == NULL) {
        rendelesek[sor][oszlop].termekek = NULL;
    } else {
        elozo->kovetkezo = NULL;
    }
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

/*Bemenetkent az asztalok es rendelesek listajat, valamint az etterem menujet keri. Modositja a megadott asztal utolso rendeleset*/
void rendelesKezel(Asztal *asztalok, Rendeles **rendelesek, MenuElem *menu){
    printf("Melyik asztal rendeleset szeretne kezelni? (1-%d)", len(asztalok));
    int asztalId = 0;
    scanf("%d", &asztalId);
    
    // Rendeles jelenlegiRendeles = rendelesek[--asztalId][keres(asztalok, asztalId)->rendelesszam];

    int valasztas;
    printf("Mit szeretne tenni?\n\t-1. Termek hozzaadasa\n\t-2. Utolso termek elvetele a rendelesbol\n\t-3. Rendeles lezarasa\n");
    scanf("%d", &valasztas);
    asztalId--;
    switch (valasztas)
    {
        case 1:
            int termekId = 0;
            
            do
            {
                printf("Irja be a termek azonositojat, ha vegzett, -1");
                scanf("%d", &termekId);
                if(termekId == -1) break;
                
                MenuElem *jelenlegi = menu;
                while(jelenlegi->id != termekId) jelenlegi = jelenlegi->kovetkezo;
                termekHozzaad(rendelesek, asztalId, keres(asztalok, asztalId)->rendelesszam-1, jelenlegi);
            } while (termekId != -1);
            
        break;
        case 2:
            termekElvesz(rendelesek, asztalId, keres(asztalok, asztalId)->rendelesszam);
        break;
        case 3:
            //rendelesLezar();
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