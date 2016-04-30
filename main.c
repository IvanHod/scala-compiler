#include <stdio.h>
#include "lexer.h"
#include "bison.h"
#include "structs.h"
#include <string.h>
#include "printTree.h"
#include <stdbool.h>

#define PATH_TO_DIR "/home/ivan/qt-creator-projects/scala-compiler/tests/"
#define COUNT_TEST 5

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
        "declaration_vars.txt",
        "arithmetical_operations.txt",
        "logical_operation.txt",
        "if_statements.txt",
        "loop.txt"
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
        }
    }
    return 0;
}
