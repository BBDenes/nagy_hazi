#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct Asztal{
    int id, ferohely, x, y, szel, mag, rendelesszam; //az asztal férőhelyeinek száma, az alaprajzon elfoglalt bal felső sarok x, y koordinátája és az asztal fizikai méretei
    bool foglalt;
}Asztal;

typedef struct AsztalLista {
    Asztal *adat;
    int meret;
} AsztalLista;

typedef struct MenuElem {
    char *nev;
    int ar;
    struct MenuElem *kovetkezo;
} MenuElem;

typedef struct Rendeles {
    MenuElem *termekek;
    bool lezarva;
    int osszeg;
} Rendeles;

/*letrehozza a rendelesek tombot @return a rendelesekre mutato pointer*/
Rendeles **rendelesekLetrehoz(int rows, int cols) {
    Rendeles **rendelesek = (Rendeles **)malloc(rows * sizeof(Rendeles *));
    for (int i = 0; i < rows; i++) {
        rendelesek[i] = (Rendeles *)malloc(cols * sizeof(Rendeles));
        for (int j = 0; j < cols; j++) {
            rendelesek[i][j].termekek = NULL;
            rendelesek[i][j].lezarva = false;
            rendelesek[i][j].osszeg = 0;
        }
    }
    return rendelesek;
}


/*Hozzaad egy rendeles a megadott asztalhoz, @return semmi*/
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
// Function to print all rendelesek in the array
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

void rendelesFelszabadit(Rendeles **rendelesek, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
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

bool termekElvesz(Rendeles **rendelesek, int sor, int oszlop){
    if(rendelesek[sor][oszlop].termekek == NULL) return true;
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

}





int main() {
    Rendeles **rendelesek = rendelesekLetrehoz(2, 1);

    // Example usage
    rendelesHozzaad(rendelesek, 0,0);
    rendelesHozzaad(rendelesek, 0, 1);
    rendelesHozzaad(rendelesek, 1, 0);

    rendelesekKiir(rendelesek, 2, 2);

    termekHozzaad(rendelesek, 0, 0, "Pizza", 1500);
    termekHozzaad(rendelesek, 0, 0, "Masik pizza", 1450);
    termekHozzaad(rendelesek, 0, 1, "Hambi", 2500);
    termekHozzaad(rendelesek, 1, 0, "Valami", 1999);

    rendelesekKiir(rendelesek, 2, 2);
    

    //rendelesTorol(rendelesek, 0);

    // Free allocated memory
    return 0;
}
