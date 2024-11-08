#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "fajlkezelo.h"

Asztal *ujAsztal(int id, int ferohely, int x, int y, int szelesseg, int magassag) {
    Asztal *uj = (Asztal *)malloc(sizeof(Asztal));
    if (!uj) {
        return NULL;
    }

    uj->id = id;
    uj->ferohely = ferohely;
    uj->x = x;
    uj->y = y;
    uj->szel = szelesseg;
    uj->mag = magassag;

    return uj;
}


/*
    *Beolvassa az asztalok helyet es ferohelyszamat a fajlbol @return a kesz alaprajz, hiba eseten NULL
*/
char **alaprajzBeolvas(AsztalLista *asztalok){

    FILE *fp;
    fp = fopen("asztalok.txt", "r");
    if (fp == NULL) {
        perror("Fajl megnyitasa sikertelen");
        return false;
    }
    char c;
    int alaprajzSzelesseg = 0;
    int alaprajzMagassag = 0;
    int sorHossz = 0;
    
    c = fgetc(fp);
    while ((c = fgetc(fp)) != EOF)
    {   
        if(c == '1') break;
        if(c == '\n'){
            if(sorHossz > alaprajzSzelesseg) alaprajzSzelesseg = sorHossz;
            sorHossz = 0;
            alaprajzMagassag++;
        }
        sorHossz++;
        // printf("%c", c);

    }
    alaprajzMagassag--;

    rewind(fp); //vissza a fajl elejere, hogy be lehessen olvasni kulon az asztalokat

    char **alaprajz = (char**) malloc(alaprajzMagassag * sizeof(char*));
        if (!alaprajz) {
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < alaprajzMagassag; i++) {
        alaprajz[i] = (char *)malloc((alaprajzSzelesseg + 1) * sizeof(char));
        if (alaprajz[i] == NULL) {
            alaprajzFelszabadit(alaprajz, i); //felszabaditja az eddig lefoglalt memoriat   
            fclose(fp);
            return NULL;
        }
    }

    for (int i = 0; i < alaprajzMagassag; i++) {
        fgets(alaprajz[i], alaprajzSzelesseg + 1, fp);
        alaprajz[i][strcspn(alaprajz[i], "\n")] = '\0'; //az enterek helyett beszur egy string vegjelet, hogy egy sor legyen
    }



    fclose(fp);
    return true;
}

void alaprajzFelszabadit(char **alaprajz, int sor) {
    for (int i = 0; i < sor; i++) {
        free(alaprajz[i]);
    }
    free(alaprajz);
}

alaprajzKiir(char **alaprajz){

}

