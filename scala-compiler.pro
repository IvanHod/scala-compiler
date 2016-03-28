TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    lex.yy.c \
    y.tab.c

HEADERS += \
    lexer.h \
    bison.h

DISTFILES += \
    parser.y \
    lexer.l
