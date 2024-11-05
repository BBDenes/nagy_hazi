#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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

bool termekHozzaad(Rendeles **rendelesek, MenuElem *menu, int asztalszam, int rendelesszam);
void termekElvesz(Rendeles **rendelesek, int asztalszam, int rendelesszam);

void rendelesMngmt(AsztalLista *asztalok, Rendeles **rendelesek, MenuElem *menu, int asztalszam);

void rendelesZar(Rendeles **rendelesek, AsztalLista *asztalok, MenuElem *menu, int asztalszam, int rendelesszam){
    //if (rendelesek[asztalszam][rendelesszam].lezarva) return;


    
    printf("-----------------Nyugta-----------------");
    //nyugtaaaaa nyomtatas asztalszam, rendelesszam, termekek, aruk, osszeguk, datum, fiszemfaszom...

    /* peldakod a datumhoz
        #include <stdio.h>
        #include <time.h>

        int main()
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        }
    */
    
    asztalok->adat[asztalszam].foglalt = false;
    rendelesek[asztalszam][rendelesszam].lezarva = true;

}


int main(void){
    Rendeles **rendelesek;
    AsztalLista asztalok;
    MenuElem *menu;

    MenuElem m1, m2;
    m1.nev = malloc((strlen("Termek1")+1) * sizeof(char));
    strcpy(m1.nev, "Termek1");
    m1.ar = 100;

    //menu = menuBeolvas();

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
    asztalLista_foglal(&asztalok, 2);
    asztalok.adat[0] = asztal1;
    asztalok.adat[1] = asztal2;
    rendelesek = rendelesekLetrehoz(1, 0);
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
                rendelesMngmt(&asztalok, rendelesek, menu, asztalSzam);
                
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

Rendeles **rendelesekLetrehoz(int rows, int cols) {
    Rendeles **rendelesek = (Rendeles**)malloc(rows * sizeof(Rendeles*));
    for (int i = 0; i < rows; i++) {
        rendelesek[i] = (Rendeles*)malloc(cols * sizeof(Rendeles));
        for (int j = 0; j < cols; j++) {
            rendelesek[i][j].termekek = NULL;
            rendelesek[i][j].lezarva = false;
            rendelesek[i][j].osszeg = 0;
        }
    }
    return rendelesek;
}

void rendelesekFelszabadit(Rendeles **rendelesek, int sor, int oszlop) {
    for (int i = 0; i < sor; i++) {
        for (int j = 0; j < oszlop; j++) {
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

void rendelesMngmt(AsztalLista *asztalok, Rendeles **rendelesek, MenuElem *menu, int asztalszam){
    system("cls");
    printf("1.: Termek hozzaadasa a rendeleshez\n2.: Az utolso termek eltavolitasa a rendelesbol\n3.: Az utolso rendeles lezarasa es a nyugta nyomtatasa.\n-1.: visszalepes");
    int c;
    scanf("%d", &c);
    //hibakereses obviously
    switch (c)
    {
        case 1:
            system("cls");
            printf("A jelenlegi menu:\n");

            

            printf("A valasztott termekek egyenkent, enterrel elvalasztva: (-1, ha vegzett):");
            int termekInd;
            scanf("%d", &termekInd);
            while(termekInd != 1){
                termekHozzaad(rendelesek, menu, termekInd, asztalszam);
            }
        
        break;
        case 2:
            termekElvesz(rendelesek, asztalszam, asztalok->adat[asztalszam].rendelesszam);
        break;
        case 3:
            rendelesZar(rendelesek,asztalok, menu, asztalszam, asztalok->adat[asztalszam].rendelesszam);
        break;
        default:
            printf("Helytelen bemenet!");
        break;
    }

}


bool termekHozzaad(Rendeles **rendelesek, MenuElem *menu, int asztalszam, int rendelesszam){
    MenuElem *ujTermek;
    ujTermek = (MenuElem*) malloc(sizeof(MenuElem));
    strcpy(ujTermek->nev, "Teszt");
    ujTermek->ar = 100;
    
    MenuElem *mozgo = rendelesek[asztalszam][rendelesszam].termekek;
    while (mozgo->kovetkezo != NULL){
        mozgo = mozgo->kovetkezo;
    }
    mozgo->kovetkezo = ujTermek;
    ujTermek->kovetkezo = NULL;
}

void termekElvesz(Rendeles **rendelesek, int asztalszam, int rendelesszam){
    MenuElem *i = rendelesek[asztalszam][rendelesszam].termekek;
    while (i->kovetkezo != NULL){
        i = i->kovetkezo;
    }
    rendelesek[asztalszam][rendelesszam].osszeg -= i->ar;
    free(i);
}

