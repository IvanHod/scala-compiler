#include <stdio.h>
#include "lexer.h"
#include "bison.h"
#include "structs.h"
#include <string.h>
#include "printTree.h"

#define PATH_TO_DIR "/home/ivan/qt-creator-projects/scala-compiler/"
#define FILE_NAME "simpleExample.txt"

int main(int argc, char *argv[])
{
    char path_to_file[100] = PATH_TO_DIR;
    strcat(path_to_file, FILE_NAME);
    FILE *file = fopen( path_to_file, "r");
    if( file != NULL ) {
        yyin = file;
        int r = yyparse();
        printf("%d", r);
    } else
        printf("error: Can`t open file.");
    root;
    printf_root();
    return 0;
}
