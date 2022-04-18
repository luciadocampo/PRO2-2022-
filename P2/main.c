/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 2
 * AUTHOR 1: Daniel Rodriguez Sanchez LOGIN 1: daniel.rodriguez.sanchez1
 * AUTHOR 2: Lucia Docampo Rodriguez LOGIN 2: lucia.docampo
 * GROUP: 4.2
 * DATE: 04 / 04 / 22
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "product_list.h"
#include "bid_stack.h"

#define MAX_BUFFER 255

void New(tList *list, tProductId product, tUserId user, tProductCategory category, tProductPrice price){
    tPosL pos;
    if((pos = findItem(product, *list)) != LNULL){ //si la posición no es nula devuelve error
        printf("+ Error: New not possible\n");
        return;
    }
    tItemL item;
    char* categoria = malloc(sizeof(char)); //reserva de memoria para la categoría

    //Metemos cada atributo en el item
    item.productPrice = price;
    item.productCategory = category;
    strcpy(item.productId, product);
    strcpy(item.seller, user);
    item.bidCounter = 0;

    createEmptyStack(&item.bidStack); //crea una pila vacía

    if(insertItem(item, list)){ //Inserta un nuevo producto en la lista
        if(item.productCategory == 0){
            strcpy(categoria, "book");
        }else{
            strcpy(categoria, "painting");
        }
        printf("* New: product %s seller %s category %s price %.2f\n",
               item.productId, item.seller, categoria, item.productPrice);
    } else { //Error si el elemento no se inserta en la lista
        printf("+ Error: New not possible\n");
    }
}

void Delete(){

}

void Bid(tList *list, tProductId product, tUserId user, tProductPrice price){
    tPosL pos;
    tPosS posS;

    if((pos = findItem(product, *list)) != LNULL){ //Comprueba si la posicion es nula o no
        tItemL item;
        tItemS itemS;
        char* categoria = malloc(sizeof(char)); //reserva de memoria para la categoría
        item = getItem(pos, *list); //metemos el item en una variable
        if(strcmp(user, item.seller) != 0){ //comprobamos que el que puja no es vendedor
            if(item.productPrice < price){ //Comprueba que el precio de la puja es mayor que el precio anterior

                if(item.productCategory == 0){
                    strcpy(categoria, "book");
                }else{
                    strcpy(categoria, "painting");
                }

                item.productPrice = price;
                item.bidCounter++;  //incrementa la cuenta de pujas del usuario
                updateItem(item, pos, list); //actualiza el contenido de esa posicion

                printf("* Bid: product %s seller %s category %s price %.2f bids %d\n",
                       item.productId, item.seller, categoria, item.productPrice, item.bidCounter);
                return;
            }
        }
    }
    printf("+ Error: Bid not possible\n");
}

void Award(){

}

void Withdraw(){

}

void Remove(){

}

void Stats(){

}

//Funcion que convierte un char a la categoria
tProductCategory char_to_category(char* string){
    tProductCategory category;
    if(strcmp(string, "book") == 0){
        category = book;
        return category;
    }
    if(strcmp(string, "painting") == 0){
        category = painting;
        return category;
    }
    printf("Categoria inexistente");
    return 0;
}


void processCommand(tList  *list, char *commandNumber, char command, char *param1, char *param2, char *param3, char *param4) {

    switch (command) {
        case 'N':
            printf("********************\n");
            printf("%s %c: product %s seller %s category %s price %s\n", commandNumber, command, param1, param2, param3, param4);
            New(list, param1, param2, char_to_category(param3), (tProductPrice)strtod(param4, NULL));

            break;
        case 'S':
            break;
        case 'B':
            break;
        case 'D':
            break;
        case 'A':
            break;
        case 'W':
            break;
        case 'R':
            break;
        default:
            break;
    }
}

void readTasks(char *filename) {
    FILE *f = NULL;
    char *commandNumber, *command, *param1, *param2, *param3, *param4;
    const char delimiters[] = " \n\r";
    char buffer[MAX_BUFFER];

    tList list;
    createEmptyList(&list);

    f = fopen(filename, "r");

    if (f != NULL) {

        while (fgets(buffer, MAX_BUFFER, f)) {
            commandNumber = strtok(buffer, delimiters);
            command = strtok(NULL, delimiters);
            param1 = strtok(NULL, delimiters);
            param2 = strtok(NULL, delimiters);
            param3 = strtok(NULL, delimiters);
            param4 = strtok(NULL, delimiters);

            processCommand(&list, commandNumber, command[0], param1, param2, param3, param4);
        }

        fclose(f);

    } else {
        printf("Cannot open file %s.\n", filename);
    }
}


int main(int nargs, char **args) {

    char *file_name = "new.txt";

    if (nargs > 1) {
        file_name = args[1];
    } else {
        #ifdef INPUT_FILE
        file_name = INPUT_FILE;
        #endif
    }

    readTasks(file_name);

    return 0;
}






