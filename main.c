#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"

typedef struct Rendeles{
    char *termekek;
    int osszeg;

}Rendeles;

typedef struct Asztal{
    int ferohely, x, y, szel, mag; //az asztal férőhelyeinek száma, az alaprajzon elfoglalt bal felső sarok x, y koordinátája és az asztal fizikai méretei
    Rendeles *rendelesek;
    
}Asztal;

int fomenu(void);
void ujAsztal(void);
void rendelesMngmt(void);
void zaras(void);
void fajlBeolvas(Asztal *asztalok, char *alaprajz);

int main(void){
    //fajlBeolvas(); coming soon
    Asztal asztalok;
    int choice = fomenu();

    switch (choice){
        case 1:
            //ujAsztal();
        break;
        case 2:
           //rendelesMngmt();
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
    return 0;
}

int fomenu(void){
    int c;
    printf("------Fomenu - A megfelelo sorszam beirasaval lehet valasztani. ------ ");
    printf("1. Uj asztal nyitasa. \n2. Meglevo rendeles kezelese. \n3. Korabbi rendelesek megtekintese. \n4. Zaras.\n");
    printf("A valasztott menupont (1-4):\n");
    scanf("%d", &c);
    
    /*TODO: input hibakezelés*/
    
    return c;
}