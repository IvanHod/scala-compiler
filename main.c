#include <stdio.h>
#include "lexer.h"
#include "bison.h"

int main(int argc, char *argv[])
{
    FILE *file = fopen("/home/ivan/qt-creator-projects/scala-compiler/example.txt", "r");
    if( file != NULL ) {
        yyin = file;
        int o = yyparse();
        printf("%d", o);
    }
    return 0;
}
