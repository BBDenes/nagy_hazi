#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

int mainMenu(void);
void newTable(void);
void manageOrder(void);
void close(void);

int main(void){
    int choice = mainMenu();

    switch (choice){
        case 1:
            //newTable();
        break;
        case 2:
           //manageOrder();
        break;
        case 3:
            //listOrders();
        break;
        case 4:
            //close();
        break;
        default:
        break;
    }
}

int mainMenu(void){
    int c;
    bool success = false;
    printf("------Fomenu - A megfelelo sorszam beirasaval lehet valasztani. ------ ");
    printf("1. Uj asztal nyitasa. \n2. Meglevo rendeles kezelese. \n3. Korabbi rendelesek megtekintese. \n4. Zaras.\n");
    printf("A valasztott menupont (1-4):\n");
    scanf("%d", &c);
    int read;
    while (!success)
    {
        read = scanf("%d", &c);
        if (read == 1)
        {
            success = true;
        }
        else{
            printf("Helytelen bemenet!\n");
            printf("A valasztott menupont (1-4):\n");
            read = scanf("%d", &c);
        }
        
        
    }
    
    
    return c;
}