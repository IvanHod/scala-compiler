#include <stdio.h>
#include "lexer.h"
#include "bison.h"
#include "structs.h"
#include <string.h>
#include "printTree.h"
#include <stdbool.h>

#define PATH_TO_DIR "/home/ivan/qt-creator-projects/scala-compiler/"
#define COUNT_TEST 3

bool isOpenFile(FILE *file, char* name) {
    if( file != NULL)
        return true;
    else {
        printf("Error: Can`t open file - %s.\n", name);
        return false;
    }
}

int main(int argc, char *argv[])
{
    char files[COUNT_TEST][30] = {
        "declaration_vars.txt",
        "arithmetical_operations.txt",
        "logical_operation.txt"
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
            strcat(writeFile_path, files[i]);
            writeFile = fopen(writeFile_path, "wb" );
            if( isOpenFile(writeFile, files[i]) ) {
                printf_root();
                fclose(writeFile);
            }
        }
    }
    return 0;
}
