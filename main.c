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
    
}Asztal;

int fomenu(void);  
void ujAsztal(void);
void rendelesMngmt(void);
void zaras(void);
bool fajlBeolvas(Asztal *asztalok);
bool asztalMalloc(Asztal *asztalok, int meret);

int main(void){
    Asztal *asztalok;
    Rendeles **rendelesek;
    fajlBeolvas(asztalok);

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

bool asztalMalloc(Asztal *asztalok, int meret){
    asztalok = (Asztal*) malloc(meret * sizeof(Asztal));
    
}

bool fajlBeolvas(Asztal *asztalok){
    FILE *fp; /* fájl mutató (file pointer/handle) */
 
    fp = fopen("asztalok.txt", "r"); /* megnyitás */
    if (fp == NULL) {
        perror("Fajl megnyitasa sikertelen");
        return false;
    }
    char c;
    while (/* condition */)
    {
        /* code */
    }
    
    

    fclose(fp);
    return true;
}