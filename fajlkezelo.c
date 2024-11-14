#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fajlkezelo.h"
#include "debugmalloc.h"


Asztal *asztalLetrehoz(int id, int ferohely, int x, int y, int szelesseg, int magassag) {
    Asztal *uj = (Asztal *) malloc(sizeof(Asztal));
    if (!uj) {
        return NULL;
    }

    uj->id = id;
    uj->ferohely = ferohely;
    uj->x = x;
    uj->y = y;
    uj->szel = szelesseg;
    uj->mag = magassag;
    uj->foglalt = false;
    uj->rendelesszam = 0;
    uj->kov = NULL;
    
   

    return uj;
}


/*
    *Beolvassa az asztalok helyet es ferohelyszamat a fajlbol @return a kesz alaprajz, hiba eseten NULL
*/
Alaprajz *alaprajzBeolvas(Asztal **asztalok){

    FILE *fp;
    fp = fopen("asztalok.txt", "r");
    if (fp == NULL) {
        perror("Fajl megnyitasa sikertelen");
        return false;
    }
    char c;
    int alaprajzSzelesseg = 0;
    int alaprajzMagassag = 0;
    int sorHossz = 0;
    
    while ((c = fgetc(fp)) != EOF)
    {   
        if(c == '1') break;
        if(c == '\n'){
            if(sorHossz > alaprajzSzelesseg) alaprajzSzelesseg = sorHossz;
            sorHossz = 0;
            alaprajzMagassag++;
        }
        sorHossz++;
        // printf("%c", c);

    }
    alaprajzMagassag--;

    rewind(fp); //vissza a fajl elejere, hogy be lehessen olvasni kulon az asztalokat

    char **alaprajz = (char**) malloc(alaprajzMagassag * sizeof(char*));
        if (!alaprajz) {
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < alaprajzMagassag; i++) {
        alaprajz[i] = (char *)malloc((alaprajzSzelesseg + 1) * sizeof(char));
        if (alaprajz[i] == NULL) {
            alaprajzFelszabadit(alaprajz, i); //felszabaditja az eddig lefoglalt memoriat   
            fclose(fp);
            return NULL;
        }
    }

    for (int i = 0; i < alaprajzMagassag; i++) {
        fgets(alaprajz[i], alaprajzSzelesseg + 1, fp);
        alaprajz[i][strcspn(alaprajz[i], "\n")] = '\0'; //az enterek helyett beszur egy string vegjelet, hogy egy sor legyen
    }



    fclose(fp);
    //*asztalok = asztalBeolvas(alaprajz, alaprajzMagassag, alaprajzSzelesseg, asztalok);
    Alaprajz *a;
    a->adat = alaprajz;
    a->szelesseg = alaprajzSzelesseg;
    a->magassag = alaprajzMagassag;
    return a;
}


void alaprajzFelszabadit(char **alaprajz, int sor) {
    for (int i = 0; i < sor; i++) {
        free(alaprajz[i]);
    }
    free(alaprajz);
}

void alaprajzKiir(char **alaprajz, int mag){
    for(int i = 0; i < mag; i++){
        int j = 0;
        char c = alaprajz[i][0];
        while(c != '\0'){
            printf("%c", c);
            j++;
            c = alaprajz[i][j];
        }
        printf("\n");
    }
}

Asztal* asztalListaHozzaad(Asztal *asztalok, Asztal *ujAsztal){
    if (ujAsztal == NULL) {
        return false;
    }

    if (asztalok == NULL) {
        return ujAsztal;
    } else {
        Asztal *aktualis = asztalok;
        while (aktualis->kov != NULL) {
            aktualis = aktualis->kov;
        }
        aktualis->kov = ujAsztal;
    }

    return asztalok;

}

/*beolvassa az asztalokat az alaprajzbol*/
Asztal *asztalBeolvas(Alaprajz *alaprajz, Asztal *asztalok) {
    int asztalId = 0;
    Asztal *uj1 = asztalLetrehoz(asztalId++, 15, 20, 10, 5, 4);
    asztalok = asztalListaHozzaad(asztalok, uj1);
    uj1 = asztalLetrehoz(asztalId++, 20, 5, 10, 3, 3);
    asztalok = asztalListaHozzaad(asztalok, uj1);
    return asztalok;
    //ez alatt nem jo

    for (int i = 0; i < alaprajz->magassag; i++) {
        for (int j = 0; j < alaprajz->szelesseg; j++) {
            if (alaprajz->adat[i][j] == '#') {
                int szelesseg = 0, magassag = 0;
                while (j + szelesseg < alaprajz->szelesseg && alaprajz->adat[i][j + szelesseg] == '#') {
                    szelesseg++;
                }
                while (i + magassag < alaprajz->magassag && alaprajz->adat[i + magassag][j] == '#') {
                    magassag++;
                }
                Asztal *uj = asztalLetrehoz(asztalId++, 0, j, i, szelesseg, magassag);
                asztalok = asztalListaHozzaad(asztalok, uj);
            }
        }
    }
    return asztalok;
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

void menuFree(MenuElem **menu){
    MenuElem *elem = *menu;

        while(elem != NULL){
        MenuElem *kovetkezo = elem->kovetkezo;
        free(elem);
        elem = kovetkezo;
    }
}
