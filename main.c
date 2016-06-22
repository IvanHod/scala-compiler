#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "semantic.h"
#include "lexer.h"
#include "bison.h"
#include "structs.h"
#include "printTree.h"
#include "generationCode.h"

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

int main(int argc, char *argv[])
{
    char files[COUNT_TEST][30] = {
        "declaration_vars.txt",
        //"arithmetical_operations.txt"
        //"logical_operation.txt"
        //"if_statements.txt"
        //"loop.txt",
        //"functions.txt"
        //"match.txt"
        //"errors_var.txt"
        //"errors_func.txt"
        //"errors_match.txt"
        "errors_if.txt"
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
            if( !doSemantic(root) )
                printf("\n!!!! ERROR: Semantic not complite\n");
            else {
                printf("Semantic checking successful\n");
                char writeFile_path[100] = PATH_TO_DIR;
                strcat(writeFile_path, replace( files[i], ".txt", "_semantic.xml") );
                writeFile = fopen(writeFile_path, "wb" );
                if( isOpenFile(writeFile, files[i]) ) {
                    printf_root();
                    fclose(writeFile);
                    generateCode(root);
                }
            }

        }
    }
    return 0;
}
