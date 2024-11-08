#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fajlkezelo.h"

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
void rendelesFelszabadit(Rendeles **rendelesek, AsztalLista *asztalok) {
    for (int i = 0; i < asztalok->meret; i++) {
        for (int j = 0; j < asztalok->adat[i].rendelesszam; j++) {
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
    char nev[50];

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

int fomenu(void){
    int c;
    system("cls");
    printf("------Fomenu - A megfelelo sorszam beirasaval lehet valasztani. ------ ");
    printf("1. Uj asztal nyitasa. \n2. Meglevo rendeles kezelese. \n3. Korabbi rendelesek megtekintese. \n4. Zaras.\n\n");
    printf("A valasztott menupont (1-4), vagy -1 a kilepeshez:\n");
    scanf("%d", &c);
    printf("\n");
    /*TODO: input hibakezelés*/
    
    return c;
}




int main() {
    Rendeles **rendelesek = rendelesekLetrehoz(2, 1);
    MenuElem *etteremMenu = NULL;
    AsztalLista *asztalok = NULL;
    menuBeolvas("menu.txt", &etteremMenu);
    char **alaprajz = alaprajzBeolvas(asztalok);

    MenuElem *current = etteremMenu;
    while (current->kovetkezo != NULL) {
        printf("ID: %d, Name: %s, Price: %d\n", current->id, current->nev, current->ar);
        current = current->kovetkezo;
    }
    alaprajzKiir(alaprajz, 20);
    
    int choice = fomenu();

    while(choice != -1){
        switch (choice){
            case 1:
                //ujAsztal(&asztalok, asztalok.meret, rendelesek);
            break;
            case 2:
                printf("Melyik asztal rendeleset szeretne kezelni? (1-%d)\n", asztalok->meret);
                int asztalSzam;
                while(scanf("%d", &asztalSzam) != 1){
                    printf("Helytelen bemenet!");
                    scanf("%d", &asztalSzam);
                }
                
            break;
            case 3:
                //rendlesLista();
            break;
            case 4:
                //zaras();
            break;
            default:
            break;
        }
        choice = fomenu();
    }

    
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

    rendelesFelszabadit(rendelesek, asztalok);
    return 0;
}
