/*
 * TITLE: PROGRAMMING II LABS
 * SUBTITLE: Practical 1
 * AUTHOR 1: Daniel Rodriguez Sanchez LOGIN 1: daniel.rodriguez.sanchez1
 * AUTHOR 2: Lucia Docampo Rodriguez LOGIN 2: lucia.docampo
 * GROUP: 4.2
 * DATE: 23 / 02 / 22
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define MAX_BUFFER 255

#ifdef DYNAMIC_LIST
#include "dynamic_list.h"
#endif
#ifdef STATIC_LIST
#include "static_list.h"
#endif

//NEW: alta nuevo producto
void New(tList *list, tProductId product, tUserId user, tProductCategory category, tProductPrice price){
    tItemL item;
    tPosL pos;
    pos = findItem(product, *list);
    if(pos == LNULL){
        strcpy(item.productId, product);
        strcpy(item.seller, user);
        item.productCategory = category;
        item.productPrice = price;
        item.bidCounter = 0;
        if(insertItem(item, LNULL, list)){
            char* categoria = malloc(sizeof(char));
            if(item.productCategory == 0){
                strcpy(categoria, "book");
            }else{
                strcpy(categoria, "painting");
            }
            printf("* New: product %s seller %s category %s price %.2f\n", item.productId, item.seller, categoria, item.productPrice);
        } else {
            printf("+ Error: New not possible\n");
        }
    }else {
        printf("+ Error: New not possible\n");
    }
}

//DELETE: baja producto
void Delete(tList *list, tProductId product){

}

//BID: puja por un producto
void Bid(tList *list, tProductId product, tUserId user, tProductPrice price){

}

//STATS: Lista productos actuales y sus datos
void Stats(tList *list){

}

tProductCategory char_to_category(char* string){
    tProductCategory category;
    if(strcmp(string, "book") == 0){
        return category = book;
    }
    if(strcmp(string, "painting") == 0){
        return category = painting;
    }
    printf("Categoria inexistente");
    return 0;
}

void processCommand(tList *list, char *commandNumber, char command, char *param1, char *param2, char *param3, char *param4) {

    switch (command) {
        case 'N':
            printf("********************\n");
            printf("%s %c: product %s seller %s category %s price %s\n", commandNumber, command, param1, param2, param3, param4);
            New(list, param1, param2, char_to_category(param3), (tProductPrice)strtod(param4, NULL));
            break;
        case 'S':
            printf("********************\n");
            printf("%s %c\n", commandNumber, command);
            break;
        case 'B':
            printf("********************\n");
            printf("%s %c: product %s bidder %s price %s\n", commandNumber, command, param1, param2, param4);
            break;
        case 'D':
            printf("********************\n");
            printf("%s %c: product %s\n", commandNumber, command, param1);
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






