TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    lex.yy.c \
    parser.tab.c

HEADERS += \
    lexer.h \
    bison.h \
    structs.h \
    func.h \
    printTree.h

DISTFILES += \
    parser.y \
    lexer.l
