#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "fajlkezelo.h"
#include "rendeleskezelo.h"


int fomenu(void){
    int c;
    //system("cls");
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
    Asztal *asztalok = NULL;
    menuBeolvas("menu.txt", &etteremMenu);
    char **alaprajz = alaprajzBeolvas(&asztalok);

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
                //rendeleskezel();
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
        rendelesekKiir(rendelesek, 2, 2);
        choice = fomenu();
    }

    
    // Example usage
    // rendelesHozzaad(rendelesek, 0,0);
    // rendelesHozzaad(rendelesek, 0, 1);
    // rendelesHozzaad(rendelesek, 1, 0);


    // termekHozzaad(rendelesek, 0, 0, "Pizza", 1500);
    // termekHozzaad(rendelesek, 0, 0, "Masik pizza", 1450);
    // termekHozzaad(rendelesek, 0, 1, "Hambi", 2500);
    // termekHozzaad(rendelesek, 1, 0, "Valami", 1999);

    // rendelesekKiir(rendelesek, 2, 2);
    

    // //rendelesTorol(rendelesek, 0);

    // rendelesFelszabadit(rendelesek, asztalok);
    return 0;
}