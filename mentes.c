#include <stdio.h>
#include<stdbool.h>
#include "fajlkezelo.h"
#include "debugmalloc.h"
#include "rendeleskezelo.h"


int maximum(Asztal *asztalok){
    
    int m = 1;

    for (Asztal *mozgo = asztalok; mozgo != NULL; mozgo = mozgo->kov){
        if(mozgo->rendelesszam > m) m = mozgo ->rendelesszam;
    }
    return m;
}

void asztalokMentes(Asztal *asztalok, Rendeles **rendelesek, const char *fajlNev) {
    FILE *fajl = fopen(fajlNev, "w");
    if (fajl == NULL) {
        perror("A mentes nem sikerult!");
        return;
    }
    fprintf(fajl, "0\n"); //tetves fajlbeolvasas.....

    const Asztal *aktualis = asztalok;
    while (aktualis != NULL) {
        if(aktualis->rendelesszam != 0 && aktualis->foglalt){ //ha van nem lezart rendeles azt irja

            // adatok irasa: asztalId, rendelesszam, osszeg, termekek
            fprintf(fajl, "%d\n", aktualis->id);
            fprintf(fajl, "%d\n", aktualis->rendelesszam);
            //fprintf(fajl, "%d\n", aktualis->foglalt);
            fprintf(fajl, "%d\n", rendelesek[aktualis->id][(aktualis->rendelesszam)-1].osszeg);
            
            //termekek mentese
            //fprintf(fajl, "Termekek:\n");
            MenuElem *menuAktualis = rendelesek[aktualis->id][(aktualis->rendelesszam)-1].termekek;
            while (menuAktualis != NULL) {
                fprintf(fajl, "%s- %d\n", menuAktualis->nev, menuAktualis->ar);
                menuAktualis = menuAktualis->kovetkezo;
            }
        }
        fprintf(fajl, "\n");
        aktualis = aktualis->kov;
    }

    fclose(fajl);
}



Rendeles **rendelesBetoltes(Asztal *asztalok, const char *fajlNev) {
    FILE *fp = fopen(fajlNev, "r");
    if (fp == NULL || fgetc(fp) == '\n') {      //ha nem letezik vagz ures a fajl, akkor nem 
        return NULL;
    }
    //rewind(fp);
    Rendeles **rendelesek = (Rendeles **)malloc(len(asztalok) * sizeof(Rendeles *));

    char sor[256];
    while (fgets(sor, sizeof(sor), fp)) {
        if (sor[0] == '\n') {
            continue; 
        }

        int id = 0;
        int rendelesszam = 0;

        // Asztal adatok betöltése
        sscanf(sor, "%d", &id);
        fgets(sor, sizeof(sor), fp);
        sscanf(sor, "%d", &rendelesszam);
        rendelesszam = rendelesszam-1;
        rendelesek[id] = (Rendeles *)malloc(rendelesszam+1 * sizeof(Rendeles));


        fgets(sor, sizeof(sor), fp);
        sscanf(sor, "%d", &rendelesek[id][rendelesszam].osszeg);
        keres(asztalok, id)->foglalt = true;
        keres(asztalok, id)->rendelesszam = rendelesszam+1;
        //Fromatum: asztalId;ferohely;mag;szel;x;y;rendelesszam;foglalt;osszeg


        for (int j = 0; j < rendelesszam; j++) {
            
            rendelesek[id][j].termekek = NULL;
            rendelesek[id][j].lezarva = true;
            rendelesek[id][j].osszeg = -1;
        }
        rendelesek[id][rendelesszam].lezarva = false;


        // Rendelések betöltése
        //fgets(sor, sizeof(sor), fp); // termekek
        MenuElem *elozoMenu = NULL;
        rendelesek[id][rendelesszam].termekek = NULL;
        while (fgets(sor, sizeof(sor), fp) && sor[0] != '\n') {
            char nev[100];
            int ar;
            if (sscanf(sor, "%[^-] - %d", nev, &ar) == 2) {
                MenuElem *ujMenu = (MenuElem *)malloc(sizeof(MenuElem));
                ujMenu->nev = strdup(nev);
                ujMenu->ar = ar;
                ujMenu->kovetkezo = NULL;

                if (elozoMenu == NULL) {
                    rendelesek[id][rendelesszam].termekek = ujMenu;
                } else {
                    elozoMenu->kovetkezo = ujMenu;
                }
                elozoMenu = ujMenu;
            }
        }
    }

    fclose(fp);
    return rendelesek;
}

int rendelesMax(char *fajlnev){
    FILE *fp = fopen(fajlnev, "r");
    if(fp == NULL){
        return 1;
    }
    int rendelesszam;
    fscanf(fp, "%d", &rendelesszam);
    fclose(fp);
    return rendelesszam;
}