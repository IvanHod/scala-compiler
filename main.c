#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "semantic.h"
#include "lexer.h"
#include "bison.h"
#include "structs.h"
#include "printTree.h"

#define PATH_TO_DIR "/home/ivan/qt-creator-projects/scala-compiler/tests/"
#define COUNT_TEST 1

bool isOpenFile(FILE *file, char* name) {
    if( file != NULL)
        return true;
    else {
        printf("Error: Can`t open file - %s.\n", name);
        return false;
    }
}

char* replace(char* str, char* in, char* out) {
    char *outStr = (char *)malloc(sizeof(str) + sizeof(out));
    char *strIn = strstr(str, in);
    if( strIn != NULL ) {
        int endSimbol = strIn - str;
        strncpy(outStr, str, endSimbol);
        strcat(outStr, out);

    } else
        strcpy(outStr, str);
    return outStr;
}

int main(int argc, char *argv[])
{
    char files[COUNT_TEST][30] = {
        //"declaration_vars.txt",
        "arithmetical_operations.txt"
        //"logical_operation.txt",
        //"if_statements.txt"
        //"loop.txt",
        //"functions.txt",
        //"classes.txt",
        //"printf.txt"
        //"objects.txt"
        //"match.txt"
    };
    int i = 0;
    for( i = 0; i < COUNT_TEST; i++ ) {
        char file_path[100] = PATH_TO_DIR;
        strcat(file_path, files[i]);
        FILE *file = fopen( file_path, "r");
        if( isOpenFile(file, files[i] ) ) {
            yyin = file;
            yyparse();
            char writeFile_path[100] = PATH_TO_DIR;
            strcat(writeFile_path, replace( files[i], ".txt", ".xml") );
            writeFile = fopen(writeFile_path, "wb" );
            if( isOpenFile(writeFile, files[i]) ) {
                printf_root();
                fclose(writeFile);
            }
            printf("\nBegin semantic analize\n");
            if( !doSemantic(root) )
                printf("\nSemantic not complite\n");

        }
    }
    return 0;
}
