#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "fajlkezelo.h"


bool alaprajzLefoglal(char **alaprajz, int szelesseg, int magassag){
    alaprajz = (char**) malloc(magassag * sizeof(char*));
    if(alaprajz == NULL) return false;
    for (int i = 0; i < magassag; i++){
        alaprajz[i] = (char*) malloc(szelesseg * sizeof(char));
        if(alaprajz[i] == NULL) return false;
    }
    
    return true;
}


/*
    *Beolvassa az asztalokat a fajlbol
*/
bool alaprajzBeolvas(){
    char *alaprajz;
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
        printf("%c", c);

    }
    alaprajzMagassag--;
    
    
    
    

    fclose(fp);
    return true;
}


