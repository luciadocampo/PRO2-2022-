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

    /* Objetivo: alta de un nuevo producto
     * Entradas: lista, id del producto, id del usuario, categoria y precio del producto
     * Salidas: lista
     * Precondiciones: existe el producto (la posición en la lista no es nula)
     * Postcondiciones: cambia la lista (tiene un elemento mas por lo que se modifican las posiciones)
     * */

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

void Delete(tList *list, tProductId product){

    /* Objetivo: baja de un producto
     * Entradas: lista y id del producto
     * Salidas: lista
     * Precondiciones: existe el producto (la posición en la lista no es nula)
     * Postcondiciones: cambia la lista (tiene un elemento menos por lo que se modifican las posiciones)
     * */

    tPosL pos;
    tItemL item;
    char* categoria = malloc(sizeof(char)); //reserva de memoria para la categoría

    if((pos = findItem(product, *list)) != LNULL){ //si la posición no es nula:
        item = getItem(pos, *list);   //metemos el item en una variable
        if(item.productCategory == 0){
            strcpy(categoria, "book");
        }else{
            strcpy(categoria, "painting");
        }
        printf("* Delete: product %s seller %s category %s price %.2f bids %d\n",
               item.productId, item.seller, categoria, item.productPrice, item.bidCounter);
        while(!isEmptyStack(item.bidStack)){ //mientras la pila no está vacía eliminamos el elemento que se encuentra en la cima
            pop(&item.bidStack);
        }
        deleteAtPosition(pos, list); //quitamos el producto de la lista
    }else { //si la posición es nula devuelve error
        printf("+ Error: Delete not possible\n");
    }
}

void Bid(tList *list, tProductId product, tUserId user, tProductPrice price){

    /* Objetivo: puja por determinado producto
     * Entradas: lista, id del producto, id del usuario y el precio del producto
     * Salidas: lista
     * Precondiciones: existe el producto en la lista
     * Postcondiciones: las pujas de un usuario cambian en la lista
     * */

    tPosL pos;
    if((pos = findItem(product, *list)) != LNULL){ //Comprueba si el producto esta en la lista
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
                strcpy(itemS.bidder, user);
                itemS.productPrice = price;
                push(itemS, &item.bidStack); //inserta el elemento en la cima de la pila
                item.bidCounter++;  //incrementa la cuenta de pujas del usuario
                updateItem(item, pos, list); //actualiza el contenido de esa posicion

                printf("* Bid: product %s bidder %s category %s price %.2f bids  %d\n",
                       item.productId, user, categoria, peek(item.bidStack).productPrice, item.bidCounter);
                return;
            }
        }
    }
    printf("+ Error: Bid not possible\n");
}

void Award(tList *list, tProductId product){

    /* Objetivo: se asigna el ganador de una puja
     * Entradas: lista, id del producto
     * Salidas:
     * Precondiciones:
     * Postcondiciones:
     * */

    tPosL pos;
    tItemL item;
    char* categoria = malloc(sizeof(char)); //reserva de memoria para la categoría

    if((pos = findItem(product, *list)) != LNULL){ //si la posición no es nula:
        item = getItem(pos, *list); //metemos el item en una variable
        if(!isEmptyStack(item.bidStack)){ //comprobamos que la pila no está vacía (hay pujas)
            if(item.productCategory == 0){
                strcpy(categoria, "book");
            }else{
                strcpy(categoria, "painting");
            }
            printf("* Award: product %s bidder %s category %s price %.2f\n",
                   item.productId, peek(item.bidStack).bidder, categoria, peek(item.bidStack).productPrice);
            while(!isEmptyStack(item.bidStack)){ //mientras la pila no está vacía eliminamos el elemento que se encuentra en la cima
                pop(&item.bidStack);
            }
            deleteAtPosition(pos, list); //quitamos el producto de la lista
            return;
        }
    }
    printf("+ Error: Award not possible\n");

}

void Withdraw(tList *list, tProductId product, tUserId user){

    /* Objetivo: la máxima puja actual del producto es retirada
     * Entradas: lista, id del usuario, id del producto
     * Salidas:
     * Precondiciones:
     * Postcondiciones:
     * */

    tPosL pos;
    tItemL item;

    char* categoria = malloc(sizeof(char)); //reserva de memoria para la categoría

    if((pos = findItem(product, *list)) != LNULL){ //si la posición no es nula:
        item = getItem(pos, *list); //metemos el item en una variable
        if(!isEmptyStack(item.bidStack)){ //que haya pujas (la pila no está vacía)
            if(strcmp(user, peek(item.bidStack).bidder) == 0){ //que el user sea el que pujó
                if(item.productCategory == 0){
                    strcpy(categoria, "book");
                }else{
                    strcpy(categoria, "painting");
                }
                printf("* Withdraw: product %s bidder %s category %s price %.2f bids  %d\n", product,
                       peek(item.bidStack).bidder, categoria, peek(item.bidStack).productPrice, item.bidCounter);
                pop(&item.bidStack); //se elimina la puja
                item.bidCounter--; //se decrementa el contador de pujas
                updateItem(item, pos, list);
                return;
            }
        }
    }
    printf("+ Error: Withdraw not possible\n");
}

void Remove(tList *list){

    /* Objetivo: elimina los productos sin pujas
     * Entradas: lista
     * Salidas:
     * Precondiciones: la lista no está vacía
     * Postcondiciones:
     * */

    tPosL pos;
    tItemL item;
    char* categoria = malloc(sizeof(char)); //reserva de memoria para la categoría
    int prod_sin_pujas = 0;

    if(!isEmptyList(*list)){ //comprobamos que hay elementos en la lista
        pos = first(*list);
        while(pos != LNULL){
            item = getItem(pos, *list);  //metemos el item en una variable
            if(isEmptyStack(item.bidStack)){ //si el producto no tiene pujas lanzamos el siguiente mensaje, quitamos el producto de la lista y actualizamos el contador de "productos sin puja"
                if(item.productCategory == 0){
                    strcpy(categoria, "book");
                }else{
                    strcpy(categoria, "painting");
                }
                printf("Removing product %s seller %s category %s price %.2f bids  %d\n",
                       item.productId, item.seller, categoria, item.productPrice, item.bidCounter);
                deleteAtPosition(pos, list); //quitamos el producto de la lista
                prod_sin_pujas = 1;
            }
            pos = next(pos, *list); //pasamos al siguiente producto
        }
    }
    if(prod_sin_pujas == 0){ //si no hay productos sin puja lanza error
        printf("+ Error: Remove not possible");
    }
}

void Stats(tList *list){

    /* Objetivo: listado de los productos actuales y sus datos
     * Entradas: lista
     * Salidas: no hay
     * Precondiciones: la lista no está vacía
     * Postcondiciones: no hay
     * */

    tPosL pos;
    tItemL item;
    tItemL max;
    max.bidCounter = 0;

    //declaramos y inicializamos atributos
    int numProductsBook = 0;
    int numProductsPainting = 0;
    float suma_book = 0;
    float suma_painting = 0;
    float average_book = 0;
    float average_painting = 0;
    char* categoria = malloc(sizeof(char)); //reserva de memoria para la categoría
    float incremento;
    float incremento_max = 0;

    if(!isEmptyList(*list)){
        pos = first(*list);
        while(pos != LNULL){
            item = getItem(pos, *list);
            if (item.productCategory == 0) {
                strcpy(categoria, "book");
            } else {
                strcpy(categoria, "painting");
            }
            if(item.bidCounter == 0){
                printf("Product %s seller %s category %s price %.2f. No bids\n",
                       item.productId, item.seller, categoria, item.productPrice);
            } else{
                incremento = ((peek(item.bidStack).productPrice - item.productPrice) / item.productPrice) * 100;
                if(incremento > incremento_max){
                    max = item;
                    incremento_max = incremento;
                }
                printf("Product %s seller %s category %s price %.2f bids %d top bidder %s\n",
                       item.productId, item.seller, categoria, item.productPrice, item.bidCounter,
                       peek(item.bidStack).bidder);
            }
            if(strcmp(categoria, "book") == 0){
                numProductsBook++;
                suma_book += item.productPrice;
                average_book = (suma_book / (float) numProductsBook);
            } else{
                numProductsPainting++; //incrementa e contador de los productos que son cuadros
                suma_painting += item.productPrice;
                average_painting = (suma_painting / (float) numProductsPainting); //calcula la media de cuadros
            }
            pos = next(pos, *list);
        }
        printf("\nCategory  Products    Price    Average\n");
        printf("Book      %2d %13.2f %10.2f\n", numProductsBook, suma_book, average_book);
        printf("Painting  %2d %13.2f %10.2f\n", numProductsPainting, suma_painting, average_painting);
        if(max.bidCounter == 0){ //si en ninguno han habido pujas
            printf("Top bid not possible\n");
        } else{
            if(max.productCategory == 0){
                strcpy(categoria, "book");
            }else{
                strcpy(categoria, "painting");
            }
            printf("Top bid: Product %s seller %s category %s price %.2f bidder %s top price %.2f increase %.2f%\n",
                   max.productId, max.seller, categoria, max.productPrice, peek(max.bidStack).bidder,
                   peek(max.bidStack).productPrice, incremento_max);
        }

    } else{ //devuelve error si la lista está vacía
        printf("+ Error: Stats not possible\n");
    }
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
            printf("%s %c: product %s seller %s category %s price %s\n", commandNumber, command, param1,
                   param2, param3, param4);
            New(list, param1, param2, char_to_category(param3), (tProductPrice)strtod(param4, NULL));
            break;

        case 'S':
            printf("********************\n");
            printf("%s %c\n", commandNumber, command);
            Stats(list);
            break;

        case 'B':
            printf("********************\n");
            printf("%s %c: product %s bidder %s price %s\n", commandNumber, command, param1, param2, param3);
            Bid(list, param1, param2, (tProductPrice)strtod(param3, NULL));
            break;

        case 'D':
            printf("********************\n");
            printf("%s %c: product %s\n", commandNumber, command, param1);
            Delete(list, param1);
            break;

        case 'A':
            printf("********************\n");
            printf("%s %c: product %s\n", commandNumber, command, param1);
            Award(list, param1);
            break;

        case 'W':
            printf("********************\n");
            printf("%s %c: product %s bidder %s\n", commandNumber, command, param1, param2);
            Withdraw(list, param1, param2);
            break;

        case 'R':
            printf("********************\n");
            printf("%s %c\n", commandNumber, command);
            Remove(list);
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






