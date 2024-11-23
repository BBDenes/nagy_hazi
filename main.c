#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "fajlkezelo.h"
#include "rendeleskezelo.h"
#include "mentes.h"
#include "debugmalloc.h"

int fomenu(void){
    int c;
    //system("cls");
    printf("------Fomenu - A megfelelo sorszam beirasaval lehet valasztani. ------ \n");
    printf("1. Uj asztal nyitasa. \n2. Meglevo rendeles kezelese. \n3. Korabbi rendelesek megtekintese. \n4. Zaras.\n\n");

    c = scanf_int("A valasztott menupont (1-4), vagy -1 a kilepeshez:");
    
    printf("\n");
    /*TODO: input hibakezelés*/
    
    return c;
}



int main() {
    MenuElem *etteremMenu = NULL;
    Asztal *asztalok = NULL;
    char *mentesFajlnev = "mentes.txt";
    if(!menuBeolvas("menu.txt", &etteremMenu)){
        perror("Menu beolvasasa sikertelen!");
        exit(-1);
    }

    Alaprajz *alaprajz = alaprajzBeolvas(&asztalok);
    if(alaprajz == NULL){
        perror("Fajl beolvasasa sikertelen!");
        menuFree(&etteremMenu);
        exit(-2);
    }


    asztalok = asztalBeolvas(alaprajz, asztalok);
    
    Rendeles **rendelesek = rendelesBetoltes(asztalok, mentesFajlnev);
    if(rendelesek == NULL){
        printf("Nem sikerult betolteni a mentest, uj munkamenet kezdodik\n");
        rendelesek = rendelesekLetrehoz(len(asztalok), 1);
        if(*rendelesek == NULL){
            perror("Nem sikerult a rendelesek letrehozasa!");
            rendelesFree(rendelesek, asztalok);
            asztalFree(&asztalok);
            alaprajzFelszabadit(alaprajz);
            menuFree(&etteremMenu);
            
        }
    }

    //Rendeles **fajlbolRendeles = rendelesBetoltes(asztalok, mentesFajlnev);

    MenuElem *jelenlegi = etteremMenu;
    printf("Az etterem menuje:\n");
    while (jelenlegi->kovetkezo != NULL) {
        printf("\t- Azonosito: %d, Termek neve: %s, Termek ara: %d\n", jelenlegi->id, jelenlegi->nev, jelenlegi->ar);
        jelenlegi = jelenlegi->kovetkezo;
    }
    
    int valasztas = fomenu();

    while(valasztas != -1){
        switch (valasztas){
            case 1:
                ujAsztal(asztalok, rendelesek);
            break;
            case 2:
                rendelesKezel(asztalok, rendelesek, etteremMenu);
                asztalokMentes(asztalok, rendelesek, mentesFajlnev);
            break;
            case 3:
                rendelesekKiir(rendelesek, asztalok);
            break;
            case 4:
                //zaras();
            break;
            default:
                printf("Nem megfelelo menupont!");
            break;
        }
        //rendelesekKiir(rendelesek, asztalok);
        
        valasztas = fomenu();
    }

    

    rendelesFree(rendelesek, asztalok);
    alaprajzFelszabadit(alaprajz);

    asztalFree(&asztalok);
    //asztalFree(&fajlbolAsztalok);
    menuFree(&etteremMenu);
    return 0;
}