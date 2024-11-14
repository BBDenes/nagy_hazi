#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "fajlkezelo.h"
#include "rendeleskezelo.h"
//#include "debugmalloc.h"

int fomenu(void){
    int c;
    //system("cls");
    printf("------Fomenu - A megfelelo sorszam beirasaval lehet valasztani. ------ \n");
    printf("1. Uj asztal nyitasa. \n2. Meglevo rendeles kezelese. \n3. Korabbi rendelesek megtekintese. \n4. Zaras.\n\n");
    printf("A valasztott menupont (1-4), vagy -1 a kilepeshez:\n");
    scanf("%d", &c);
    printf("\n");
    /*TODO: input hibakezelés*/
    
    return c;
}



int main() {
    MenuElem *etteremMenu = NULL;
    Asztal *asztalok = NULL;
    menuBeolvas("menu.txt", &etteremMenu);
    Alaprajz *alaprajz = NULL;// = alaprajzBeolvas();
    asztalok = asztalBeolvas(alaprajz, asztalok);
    Rendeles **rendelesek = rendelesekLetrehoz(len(asztalok), 1);

    MenuElem *jelenlegi = etteremMenu;
    printf("Az etterem menuje:\n");
    while (jelenlegi->kovetkezo != NULL) {
        printf("\t- Azonosito: %d, Termek neve: %s, Termek ara: %d\n", jelenlegi->id, jelenlegi->nev, jelenlegi->ar);
        jelenlegi = jelenlegi->kovetkezo;
    }
    
    int choice = fomenu();

    while(choice != -1){
        switch (choice){
            case 1:
                ujAsztal(asztalok, rendelesek);
            break;
            case 2:
                rendelesKezel(asztalok, rendelesek, etteremMenu);
            break;
            case 3:
                rendelesekKiir(rendelesek, asztalok);
            break;
            case 4:
                //zaras();
            break;
            default:
            break;
        }
        rendelesekKiir(rendelesek, asztalok);
        choice = fomenu();
    }

    

    rendelesFree(rendelesek, asztalok);
    //alaprajzFelszabadit(alaprajz->adat, alaprajz->magassag);
    asztalFree(&asztalok);
    menuFree(&etteremMenu);
    return 0;
}