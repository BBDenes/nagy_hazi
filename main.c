#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "fajlkezelo.h"


typedef struct AsztalLista {
    Asztal *adat;
    int meret;
} AsztalLista;

bool asztalLista_foglal(AsztalLista *dt, int meret) {
    dt->meret = meret;
    dt->adat = (Asztal*) malloc(meret * sizeof(Asztal));
    return dt->adat != NULL;    /* sikerült? */
}
 
void dintomb_felszabadit(AsztalLista *dt) {
    free(dt->adat);
}



int fomenu(void);  
void ujAsztal(void);
void rendelesMngmt(void);
void zaras(void);


int main(void){
    
    Asztal asztal1, asztal2;
    asztal1.ferohely = 5;
    asztal1.id = 0;
    asztal1.mag = 5;
    asztal1.szel = 7;
    asztal1.x = 5;
    asztal1.y = 5;
    asztal2.ferohely = 15;      //debug celokra amig nincs fajlbeolvasas...
    asztal2.id = 1;
    asztal2.mag = 3;
    asztal2.szel = 7;
    asztal2.x = 15;
    asztal2.y = 5;
    AsztalLista asztalok;
    asztalLista_foglal(&asztalok, 2);
    asztalok.adat[0] = asztal1;
    asztalok.adat[1] = asztal2;
    rendelesFoglal


    int choice = fomenu();

    switch (choice){
        case 1:
            ujAsztal();
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
    free(asztalok.adat);
    return 0;
    
}

void ujAsztal(AsztalLista const *asztalok, Rendeles *rendelesek){
    printf("Melyik asztalt szeretne megnyitni? (1-%i)", asztalok->meret);
    //input error handling.....
    int in;
    scanf("%i", &in);
    while(asztalok->adat[in-1].foglalt){
        printf("A kivalasztott asztal foglalt, valasszon masikat!");
        scanf("%i", &in);
    }
    asztalok->adat[in-1].foglalt = true;
    rendelesHozzaad(rendelesek[in-1]);
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
