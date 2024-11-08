#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "fajlkezelo.h"

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
                asztalKezel(*rendelesek, *asztalok);
                
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