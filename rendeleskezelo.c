#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fajlkezelo.h"
#include "rendeleskezelo.h"
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

int len(Asztal *asztalok){
    int meret = 0;
    Asztal *mozgo = asztalok;

    while (mozgo != NULL) {
        meret++;
        mozgo = mozgo->kov;
    }
    return meret;
}

Asztal *keres(Asztal *asztalok, int index){
    Asztal *mozgo = asztalok;
    for(int i = 0; i < index; i++) mozgo = mozgo->kov;
    return mozgo;
}

/*megnyit egy uj asztalt, majd hozzaad egy rendelest */
void ujAsztal(Asztal *asztalok, Rendeles **rendelesek){
    
    int meret = len(asztalok);

    system("cls");
    printf("Melyik asztalt szeretne megnyitni? (1-%d), vagy -1 a visszalepeshez)\n", meret);
    //input error handling.....
    int in;
    scanf("%i", &in);
    if(in == -1){
        return;
    }
    Asztal *kivalasztott = keres(asztalok, in);


    while(kivalasztott->foglalt){
        printf("A kivalasztott asztal mar foglalt, valasszon masikat!\n");
        scanf("%i", &in);
        if(in == -1){
            return;
        }
        Asztal *kivalasztott = keres(asztalok, in);
    }
    in--;
    if(kivalasztott->rendelesszam != 0){
        rendelesHozzaad(rendelesek, kivalasztott->id, kivalasztott->rendelesszam+1);
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
void rendelesekKiir(Rendeles **rendelesek, int sor, int oszlop) {
    for (int i = 0; i < sor; i++) {
        printf("Asztal %d\n", i);
        for(int j = 0; j < oszlop; j++){
            printf("Rendeles %d: osszeg: %d, lezarva: %s\n", j, rendelesek[i][j].osszeg, rendelesek[i][j].lezarva? "true":"false");
            MenuElem *jelenlegi = rendelesek[i][j].termekek;
            while (jelenlegi != NULL) {
                printf("  - Termek nev: %s, ara: %d\n", jelenlegi->nev, jelenlegi->ar);
                jelenlegi = jelenlegi->kovetkezo;
            }
            printf("\n");
        }
    }
}

/* Felszabaditja a rendelesek tombhoz lefoglalt memoriat*/
void rendelesFelszabadit(Rendeles **rendelesek, Asztal *asztalok) {
    int meret = len(asztalok);

    for (int i = 0; i < meret; i++) {
        Asztal *kivalasztott = keres(asztalok, i);
        for (int j = 0; j < kivalasztott->rendelesszam; j++) {
            printf("%d", rendelesek[i][j].osszeg);
            MenuElem *jelenlegi = rendelesek[i][j].termekek;
            while (jelenlegi != NULL) {
                MenuElem *next = jelenlegi->kovetkezo;
                free(jelenlegi->nev);
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


/*Beolvassa a menut a megadott fajlbol @param fajlnev fajl neve @param menu a menu lancolt listaja*/
bool menuBeolvas(char *fajlnev ,MenuElem **menu){
    FILE *fp;

    fp = fopen(fajlnev, "r");
    if(fp == NULL){
        perror("Fajl beolvasasa sikertelen!");
        return false;
    }

    int id, ar;
    char nev[100];

    while (fscanf(fp, "%d;%49[^;];%d", &id, nev, &ar) == 3) {
        MenuElem *ujElem = (MenuElem *)malloc(sizeof(MenuElem));
        if(ujElem == NULL) return false;
        ujElem->id = id;
        ujElem->nev = strdup(nev);          //uj elem letrehozasa
        ujElem->ar = ar;
        ujElem->kovetkezo = NULL;

        if (*menu == NULL)   //elso elem?
        {
            *menu = ujElem;
        }else{
            MenuElem *jelenlegi = *menu;
            while(jelenlegi->kovetkezo != NULL){
                jelenlegi = jelenlegi->kovetkezo;
            }
            jelenlegi->kovetkezo = ujElem;

        }

        
    }
    return true;

    
}


