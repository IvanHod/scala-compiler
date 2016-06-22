TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    lex.yy.c \
    parser.tab.c \
    semantic.c \
    collection/array.c \
    collection/deque.c \
    collection/hashset.c \
    collection/hashtable.c \
    collection/list.c \
    collection/queue.c \
    collection/slist.c \
    collection/stack.c \
    collection/treeset.c \
    collection/treetable.c \
    func.c \
    generationCode.c

HEADERS += \
    lexer.h \
    bison.h \
    structs.h \
    func.h \
    printTree.h \
    semantic.h \
    collection/array.h \
    collection/common.h \
    collection/deque.h \
    collection/hashset.h \
    collection/hashtable.h \
    collection/list.h \
    collection/queue.h \
    collection/slist.h \
    collection/stack.h \
    collection/treeset.h \
    collection/treetable.h \
    generationCode.h

DISTFILES += \
    parser.y \
    lexer.l
