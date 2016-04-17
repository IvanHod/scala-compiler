#include <stdio.h>
#include "lexer.h"
#include "bison.h"
#include "structs.h"

int main(int argc, char *argv[])
{
    FILE *file = fopen("/home/ivan/qt-creator-projects/scala-compiler/simpleExample.txt", "r");
    if( file != NULL ) {
        yyin = file;
        int r = yyparse();
        printf("%d", r);
    } else
        printf("error: Can`t open file.");
    return 0;
}
