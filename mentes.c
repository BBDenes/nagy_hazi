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

int getMax(const char *fajlnev){
    FILE *fp = fopen(fajlnev, "r");
    if(fp == NULL){
        return 1;
    }
    return (int) fgetc(fp);
    fclose(fp);
}

void asztalokMentes(Asztal *asztalok, Rendeles **rendelesek, const char *fajlNev) {
    FILE *fajl = fopen(fajlNev, "w");
    if (fajl == NULL) {
        perror("A mentes nem sikerult!");
        return;
    }
    fprintf(fajl, "%d\n", maximum(asztalok));

    const Asztal *aktualis = asztalok;
    while (aktualis != NULL) {
        if(aktualis->rendelesszam != 0){

            // adatok irasa: asztalId, rendelesszam, osszeg, termekek
            fprintf(fajl, "%d\n", aktualis->id);
            fprintf(fajl, "%d\n", aktualis->ferohely);
            fprintf(fajl, "%d\n", aktualis->rendelesszam);
            fprintf(fajl,"%d;%d;%d;%d\n", aktualis->x, aktualis->y, aktualis->szel, aktualis->mag);
            fprintf(fajl, "%d\n", rendelesek[aktualis->id][(aktualis->rendelesszam)-1].osszeg);
            
            //termekek mentese
            //fprintf(fajl, "Termekek:\n");
            MenuElem *menuAktualis = rendelesek[aktualis->id][(aktualis->rendelesszam)-1].termekek;
            while (menuAktualis != NULL) {
                fprintf(fajl, "%s - %d\n", menuAktualis->nev, menuAktualis->ar);
                menuAktualis = menuAktualis->kovetkezo;
            }
        }
        fprintf(fajl, "\n");
        aktualis = aktualis->kov;
    }

    fclose(fajl);
}


Asztal *asztalokBetoltes(Rendeles **rendelesek, const char *fajlNev) {
    FILE *fp = fopen(fajlNev, "r");
    if (fp == NULL || fgetc(fp) == '\n') {      //ha nem letezik vagz ures a fajl, akkor nem 
        printf("Nem talalhato mentett allapot, uj munkamanet kezdodik!");
        return NULL;
    }

    Asztal *asztalok = NULL;
    Asztal *elozo = NULL;

    char sor[256];
    while (fgets(sor, sizeof(sor), fp)) {
        if (sor[0] == '\n') {
            continue; 
        }

        Asztal *ujAsztal = (Asztal *)malloc(sizeof(Asztal));
        ujAsztal->kov = NULL;
        ujAsztal->foglalt = true;

        // Asztal adatok betöltése
        sscanf(sor, "%d", &ujAsztal->id);
        fgets(sor, sizeof(sor), fp);
        sscanf(sor, "%d", &ujAsztal->rendelesszam);
        fgets(sor, sizeof(sor), fp);
        sscanf(sor, "%d", &rendelesek[ujAsztal->id][(ujAsztal->rendelesszam)-1].osszeg);
        //Fromatum: asztalId;ferohely;mag;szel;x;y;rendelesszam;foglalt;osszeg

        for (int i = 0; i < (ujAsztal->rendelesszam)-1; i++)
        {
            rendelesek[ujAsztal->id][i].lezarva = true;
        }
        

        fgets(sor, sizeof(sor), fp);
        sscanf(sor, "%d;%d;%d;%d;%d;%d;%d", &ujAsztal->ferohely, &ujAsztal->mag, &ujAsztal->szel, &ujAsztal->x, &ujAsztal->y, &ujAsztal->rendelesszam, &ujAsztal->foglalt);

        // Rendelések betöltése
        fgets(sor, sizeof(sor), fp); // termekek
        MenuElem *elozoMenu = NULL;
        while (fgets(sor, sizeof(sor), fp) && sor[0] != '\n') {
            char nev[100];
            int ar;
            if (sscanf(sor, "%[^-] - %d", nev, &ar) == 2) {
                MenuElem *ujMenu = (MenuElem *)malloc(sizeof(MenuElem));
                ujMenu->nev = strdup(nev);
                ujMenu->ar = ar;
                ujMenu->kovetkezo = NULL;

                if (elozoMenu == NULL) {
                    rendelesek[ujAsztal->id][(ujAsztal->rendelesszam)-1].termekek = ujMenu;
                } else {
                    elozoMenu->kovetkezo = ujMenu;
                }
                elozoMenu = ujMenu;
            }
        }

        
        if (asztalok == NULL) {
            asztalok = ujAsztal;
        } else {
            elozo->kov = ujAsztal;
        }
        elozo = ujAsztal;
    }

    fclose(fp);
    return asztalok;
}

int rendelesMax(char *fajlnev){
    FILE *fp = fopen(fajlnev, "r");
    if(fp == NULL){
        return 1;
    }
    int rendelesszam = (int) fgetc(fp);
    fclose(fp);
    return rendelesszam;
}
