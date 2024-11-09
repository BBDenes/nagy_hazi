#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fajlkezelo.h"


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
    uj->kov = NULL;
    
   

    return uj;
}


/*
    *Beolvassa az asztalok helyet es ferohelyszamat a fajlbol @return a kesz alaprajz, hiba eseten NULL
*/
char **alaprajzBeolvas(Asztal *asztalok){

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
    asztalBeolvas(alaprajz, alaprajzMagassag, alaprajzSzelesseg, asztalok);
    return alaprajz;
}

/* Termek hozzaadasa az etterem menujenek listajahoz @return ha hiba tortent, false, egyeb esetben true*/
bool termekHozzaad(Rendeles **rendelesek, int sor, int oszlop, char *nev, int ar){
    MenuElem *ujTermek = (MenuElem*) malloc(sizeof(MenuElem));
    if(ujTermek == NULL) return false;

    ujTermek->nev = strdup(nev);
    ujTermek->ar = ar;
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
void asztalBeolvas(char **alaprajz, int sorok, int oszlopok, Asztal *asztalok) {
    int asztalId = 0;
    Asztal *uj1 = asztalLetrehoz(asztalId++, 15, 20, 10, 5, 4);
    asztalok = asztalListaHozzaad(asztalok, uj1);
    uj1 = asztalLetrehoz(asztalId++, 20, 5, 10, 3, 3);
    asztalok = asztalListaHozzaad(asztalok, uj1);
    return;
    //ez alatt nem jo

    for (int i = 0; i < sorok; i++) {
        for (int j = 0; j < oszlopok; j++) {
            if (alaprajz[i][j] == '#') {
                int szelesseg = 0, magassag = 0;
                while (j + szelesseg < oszlopok && alaprajz[i][j + szelesseg] == '#') {
                    szelesseg++;
                }
                while (i + magassag < sorok && alaprajz[i + magassag][j] == '#') {
                    magassag++;
                }
                Asztal *uj = asztalLetrehoz(asztalId++, 0, j, i, szelesseg, magassag);
                asztalListaHozzaad(asztalok, uj);
            }
        }
    }
}

