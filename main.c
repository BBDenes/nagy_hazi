#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include "debugmalloc.h"
#include "fajlkezelo.h"



void rendelesKiir(Rendeles ** rendelesek, AsztalLista *asztalok);

int fomenu(void);
void dintomb_felszabadit(AsztalLista *dt);
bool asztalLista_foglal(AsztalLista *dt, int meret);
void ujAsztal(const AsztalLista *asztalok, const int meret, Rendeles **rendelesek);
Rendeles** rendelesMalloc(const int meret);
bool rendelesHozzaad(Rendeles** rendelesek, int asztalId,int regiHossz, int ujHossz);
void rendelesFree(Rendeles** rendelesek, int meret);

void rendelesMngmt(AsztalLista *asztalok, Rendeles **rendelesek, int asztalszam);



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

    while(choice != -1){
        switch (choice){
            case 1:
                ujAsztal(&asztalok, asztalok.meret, rendelesek);
            break;
            case 2:
                printf("Melyik asztal rendeleset szeretne kezelni? (1-%d)\n", asztalok.meret);
                int asztalSzam;
                scanf("%d",&asztalSzam);
                rendelesMngmt(&asztalok, rendelesek, asztalSzam);
                
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
    free(asztalok.adat);
    free(rendelesek[0]);
    free(rendelesek);
    return 0;
    
}

void ujAsztal(const AsztalLista *asztalok, const int meret, Rendeles **rendelesek){
    system("cls");
    printf("Melyik asztalt szeretne megnyitni? (1-%d), vagy -1 a visszalepeshez)\n", asztalok->meret);
    //input error handling.....
    int in;
    scanf("%i", &in);
    if(in == -1){
        return;
    }
    while(asztalok->adat[in-1].foglalt){
        printf("A kivalasztott asztal mar foglalt, valasszon masikat!\n");
        scanf("%i", &in);
        if(in == -1){
            fomenu();
            return;
        }
    }
    in--;
    Asztal jelenlegiAsztal = asztalok->adat[in];
    if(jelenlegiAsztal.rendelesszam != 0){
        rendelesHozzaad(rendelesek, in, jelenlegiAsztal.rendelesszam-1, jelenlegiAsztal.rendelesszam); //lefoglal uj memoriateruletet az uj rendeles szamara
    }
    rendelesek[in][jelenlegiAsztal.rendelesszam].osszeg = 0;
    rendelesek[in][jelenlegiAsztal.rendelesszam].lezarva = false;
    asztalok->adat[in].foglalt = true;
    asztalok->adat[in].rendelesszam++;
    rendelesKiir(rendelesek, asztalok);
    return;
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

    int sorszam = (regiHossz < ujHossz) ? regiHossz : ujHossz;      //mettol meddig masoljon, hogy csokkentesre is lehessen hasznalni
    for (int i = 0; i < sorszam; i++) {
        ujsor[i] = rendelesek[asztalId][i];
    }

    free(rendelesek[asztalId]);
    rendelesek[asztalId] = ujsor;
    return true;

}

void rendelesKiir(Rendeles ** rendelesek, AsztalLista *asztalok){
    for (int i = 0; i < asztalok->meret; i++)
    {
        printf("Asztal %d:\n Ferohely: %d, Foglalt: %d", i ,asztalok->adat[i].ferohely, asztalok->adat[i].foglalt);
        
        int a = asztalok->adat[i].rendelesszam;
        for (int j = 0; j < a; j++)
        {
            printf("Rendeles %d:\n Lezarva: %d, Osszeg: %d\n", j, rendelesek[i][j].lezarva, rendelesek[i][j].osszeg);
        }
        
    }
    
}

void rendelesMngmt(AsztalLista *asztalok, Rendeles **rendelesek, int asztalszam){
    system("cls");
    printf("1. Termek hozzaadasa a rendeleshez\n2. Az utolso termek eltavolitasa a rendelesbol\n3. Rendeles lezarasa es a nyugta nyomtatasa.\n");
    int c;
    scanf("%d", &c);

    switch (c)
    {
    case 1:
        
        break;
    
    default:
        break;
    }

}