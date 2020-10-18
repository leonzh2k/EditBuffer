#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "./include/EditBuffer.h"
#define MAX_LOAD_STRING 5000


int main() {
    printf("******************************************************************************************\n");
    printf("*                                  EDIT BUFFER                                           *\n");
    printf("******************************************************************************************\n");
    SEditBufferRef EditBuffer = EditBufferCreate();
    printOptions();
    char input;
    char trashChar;
    printf("Input: ");
    scanf(" %c", &input);
    while (input != 'q') {
        scanf("%c", &trashChar);
        switch(input) {
            case('l'): ;
                loadStringIntoBuffer(EditBuffer);
                break;
            case('m'):
                moveBufferCursor(EditBuffer);
                break;
            case('i'):
                insertCharsIntoBuffer(EditBuffer);
                break;
            case('d'):
                deleteCharsFromBuffer(EditBuffer);
                break;
            case('c'):
                printCharsInBuffer(EditBuffer);
                break;
            case('s'):
                printBufferSize(EditBuffer);
                break;
            case('v'):
                PrintEditBuffer(EditBuffer);
                break;
            case('h'):
                printOptions();
                break;
            default:
                printf("Invalid Input\n");
        }
        printf("Input: ");
        scanf(" %c", &input);
    }
    // printf("The # of characters in the buffer is %d\n", EditBufferSize(EditBuffer));
    EditBufferDestroy(EditBuffer);
    printf("Thank you for using this Edit Buffer.\n");
    return 0;
}

//gcc -Wall -Werror main.c src/EditBuffer.c -o EditBufferMain.exe
//./EditBuffer

void printOptions() {
    newLine();
    printf("============================OPTIONS=================================\n");
    printf("l - load a string into the buffer (will clear the buffer first)\n");
    printf("m - move the cursor\n");
    printf("i - insert a certain number of characters after the cursor\n");
    printf("d - delete a certain number of characters after the cursor\n");
    printf("c - get the # of characters in the buffer\n");
    printf("s - get the size of the buffer\n");
    printf("v - view the buffer\n");
    printf("h - print options\n");
    printf("q - quit\n");
    newLine();
}

void loadStringIntoBuffer(SEditBufferRef EditBuffer) {
    // scanf("%c", &trashChar); //catches the newline in input stream. If removed fgets below will read the newline entered when scanning "input" and thus will not work
    char loadString[MAX_LOAD_STRING];  
    newLine();
    printf("String to load (5000 chars max): ");
    fgets(loadString, 5000, stdin);
    loadString[strcspn(loadString, "\n")] = 0; //https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
    // printf("string to be loaded: %s\n", loadString);
    EditBufferLoad(EditBuffer, loadString);
    printf("String loaded.\n\n");
}

void moveBufferCursor(SEditBufferRef EditBuffer) {
    newLine();
    if (EditBufferChars(EditBuffer) == 0) {
        printf("No characters in buffer; cannot move cursor!\n\n");
    }
    else {
        int offset, origin = 4; char trashChar; //init origin to some value that will be repalced later

        while (origin != EDIT_BUFFER_ORIGIN_BEGINNING && origin != EDIT_BUFFER_ORIGIN_CURRENT && origin != EDIT_BUFFER_ORIGIN_END) {
            
            printf("Choose an origin:\n");
            printf("===================\n");
            printf("0 - beginning (cursor will be moved so that there will be *offset* characters between cursor and beginning of buffer)\n");
            printf("1 - current (cursor will be moved relative to its current position)\n");
            printf("2 - end (cursor will be moved so that there will be *offset* characters between cursor and end of buffer)\n");
            printf(": ");
            scanf(" %d", &origin);
            scanf("%c", &trashChar); //catches newline
        }
        //scanf("%c", &trashChar); catches newline
        printf("Offset (integers only): ");
        scanf(" %d", &offset);
        EditBufferMoveCursor(EditBuffer, offset, origin);
    }
}

void insertCharsIntoBuffer(SEditBufferRef EditBuffer) {
    newLine();
    char insertString[MAX_LOAD_STRING]; 
    char trashChar;
    int charsToRead = -1000; //init to some crap value
    // scanf("%c", &trashChar);
    printf("String to read chars from (5000 chars max): ");
    fgets(insertString, 5000, stdin);
    insertString[strcspn(insertString, "\n")] = 0; //https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
    while (charsToRead > strlen(insertString) || charsToRead < 0) {
        printf("Number of characters to read from the string: ");
        scanf(" %d", &charsToRead);
        scanf("%c", &trashChar); //catches newline
    }
    
    EditBufferInsert(EditBuffer, insertString, charsToRead);
    printf("String inserted.\n");
    newLine();
}

void deleteCharsFromBuffer(SEditBufferRef EditBuffer) {
    newLine();
    char trashChar;
    int charsToDelete = -1;
    if (EditBufferChars(EditBuffer) == 0) {
        printf("No characters in the buffer. Deletion not performed.\n");
    }
    else {
        scanf("%c", &trashChar);
        printf("Number of characters to delete: ");
        scanf(" %d", &charsToDelete);
        printf("%d characters deleted.\n", EditBufferDelete(EditBuffer, charsToDelete));
    }
    newLine();
}

void printCharsInBuffer(SEditBufferRef EditBuffer) {
    newLine();
    printf("The # of characters in the buffer is %d\n", EditBufferChars(EditBuffer));
    newLine();
}

void printBufferSize(SEditBufferRef EditBuffer) {
    newLine();
    printf("Size of the buffer is %d\n", EditBufferSize(EditBuffer));
    newLine();
}

void newLine() {
    printf("\n");
}