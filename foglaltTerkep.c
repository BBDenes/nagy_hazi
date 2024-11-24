#include <stdio.h>
#include <stdbool.h>
#include "fajlkezelo.h"
#include "econio.h"



void terkepKiir(Alaprajz *alaprajz, Asztal *asztalok){

    int alacsony = 1; //zold
    int kozepes = 2; //sarga
    int nagy = 3; //piros

    for (int i = 0; i < alaprajz->magassag; i++){
        for (int j = 0; j < alaprajz->szelesseg; j++){
            if(alaprajz->adat[i][j] == '\0'){
                printf("\n");
            }else{
                if(alaprajz->adat[i][j] == '#'){
                    int rendelesszam = rendszam(asztalok, i, j);
                    if (rendelesszam >= nagy){
                        econio_textcolor(COL_LIGHTRED);
                    }else if(rendelesszam >= kozepes){
                        econio_textcolor(COL_YELLOW);
                    }else{
                        econio_textcolor(COL_GREEN);
                    }
                }else{
                    econio_textcolor(COL_RESET);
                }
                printf("%c", alaprajz->adat[i][j]);
            }
        }
    }


    econio_textcolor(COL_RESET);
    
}

int rendszam(Asztal *asztalok, int sor, int oszlop){
    for(Asztal *asztal = asztalok; asztal != NULL; asztal = asztal->kov){
        if((sor >= asztal->y && sor <= asztal->y + asztal->mag) && (oszlop >= asztal->x && oszlop <= asztal->x + asztal->szel)){
            return asztal->rendelesszam;
        }
    }
    return 0;
    
}