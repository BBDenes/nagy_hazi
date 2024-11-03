#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include "debugmalloc.h"
#include "fajlkezelo.h"


typedef struct AsztalLista {
    Asztal *adat;
    int meret;
} AsztalLista;


int fomenu(void);
void dintomb_felszabadit(AsztalLista *dt);
bool asztalLista_foglal(AsztalLista *dt, int meret);
void ujAsztal(const AsztalLista *asztalok, const int meret, Rendeles **rendelesek);
Rendeles** rendelesMalloc(const int meret);
bool rendelesHozzaad(Rendeles** rendelesek, int asztalId,int regiHossz, int ujHossz);
void rendelesFree(Rendeles** rendelesek, int meret);



int main(void){
    Rendeles **rendelesek;    
    Asztal asztal1, asztal2;
    asztal1.ferohely = 5;
    asztal1.id = 0;
    asztal1.mag = 5;
    asztal1.szel = 7;
    asztal1.x = 5;
    asztal1.y = 5;
    asztal1.rendelesszam = 0;
    asztal1.foglalt = false;
    asztal2.ferohely = 15;      //debug celokra amig nincs fajlbeolvasas...
    asztal2.id = 1;
    asztal2.mag = 3;
    asztal2.szel = 7;
    asztal2.x = 15;
    asztal2.y = 5;
    asztal2.foglalt = false;
    asztal2.rendelesszam = 0;
    AsztalLista asztalok;
    asztalLista_foglal(&asztalok, 2);
    asztalok.adat[0] = asztal1;
    asztalok.adat[1] = asztal2;
    rendelesek = rendelesMalloc(asztalok.meret);
    if(rendelesek == NULL) return 1;


    int choice = fomenu();

    switch (choice){
        case 1:
            ujAsztal(&asztalok, asztalok.meret, rendelesek);
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
    free(rendelesek[0]);
    free(rendelesek);
    return 0;
    
}

void ujAsztal(const AsztalLista *asztalok, const int meret, Rendeles **rendelesek){
    printf("Melyik asztalt szeretne megnyitni? (1-%i, vagy -1 a visszalepeshez)", asztalok->meret);
    //input error handling.....
    int in;
    scanf("%i", &in);
    if(in == -1){
        fomenu();
        return;
    }
    while(asztalok->adat[in-1].foglalt){
        printf("A kivalasztott asztal foglalt, valasszon masikat!");
        scanf("%i", &in);
    }
    in--;
    Asztal jelenlegiAsztal = asztalok->adat[in];
    if(jelenlegiAsztal.rendelesszam != 0){
        rendelesHozzaad(rendelesek, in,jelenlegiAsztal.rendelesszam-1, jelenlegiAsztal.rendelesszam); //lefoglal uj memoriateruletet az uj rendeles szamara
    }
    rendelesek[in][jelenlegiAsztal.rendelesszam].osszeg = 0;
    printf("%d", rendelesek[in][jelenlegiAsztal.rendelesszam].osszeg);
    asztalok->adat[in].foglalt = true;
    asztalok->adat[in].rendelesszam++;
    
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

void dintomb_felszabadit(AsztalLista *dt) {
    free(dt->adat);
}

bool asztalLista_foglal(AsztalLista *dt, int meret) {
    dt->meret = meret;
    dt->adat = (Asztal*) malloc(meret * sizeof(Asztal));
    return dt->adat != NULL;
}

Rendeles** rendelesMalloc(const int meret){
    Rendeles** lista = (Rendeles**) malloc(meret * sizeof(Rendeles*));
    if(lista == NULL) return NULL;
    for (int i = 0; i < meret; i++){
        lista[i] = (Rendeles*) malloc(sizeof(Rendeles));

        if(lista[i] == NULL){
            for (int j = 0; j < i; j++) {
                free(lista[j]);
            }
            return NULL;
        }
    }
    return lista;
}

void rendelesFree(Rendeles** rendelesek, int meret){
    for (int i = 0; i < meret; i++)
        free(rendelesek[i]);
    free(rendelesek);
}

bool rendelesHozzaad(Rendeles** rendelesek, int asztalId, int regiHossz, int ujHossz){
    Rendeles *ujsor = (Rendeles*) malloc(ujHossz * sizeof(Rendeles));
    if(ujsor == NULL) return false;

    int sorszam = (regiHossz < ujHossz) ? regiHossz : ujHossz;
    for (int i = 0; i < sorszam; i++) {
        ujsor[i] = rendelesek[asztalId][i];
    }

    free(rendelesek[asztalId]);
    rendelesek[asztalId] = ujsor;
    return true;

}