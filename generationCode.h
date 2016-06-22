#ifndef GENERATIONCODE
#define GENERATIONCODE

#ifndef O_BINARY
#define O_BINARY  0
#endif

#include "semantic.h"
#include "structs.h"
#include <fcntl.h>
#include <sys/io.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

struct Program* program;

int fd;
unsigned char u1;
char s1;
unsigned short int u2;
unsigned int u4;
short int s2;
int s4;
float sf4;

int fh;

extern struct Constant* constantCode;
extern struct Constant* constantClassString;
extern struct Constant* objectConstructorMethodRef;
extern struct Constant* constantObjectClass;
extern struct Constant* constantSourceFile;
extern struct Constant* constantSourceFileName;

extern struct Constant* constantClassRuntimeLib;
extern struct Constant* printStringMethodRef;
extern struct Constant* printIntegerMethodRef;
extern struct Constant* printFloatMethodRef;

extern struct Constant* openSquareParenthesis;
extern struct Constant* closeSquareParenthesis;
extern struct Constant* space;


extern struct Constant* constantClassMixed;
extern struct Constant* mixedInitMethodRef;
extern struct Constant* mixedAddMethodRef;
extern struct Constant* mixedEquallyMethodRef;
extern struct Constant* mixedPrintMethodRef;
extern struct Constant* mixedSetIntMethodRef;
extern struct Constant* mixedSetBoolMethodRef;
extern struct Constant* mixedSetFloatMethodRef;
extern struct Constant* mixedSetStringMethodRef;
extern struct Constant* mixedSetTypeMethodRef;
extern struct Constant* mixedGetIntMethodRef;
extern struct Constant* mixedGetBoolMethodRef;
extern struct Constant* mixedGetFloatMethodRef;
extern struct Constant* mixedGetStringMethodRef;
extern struct Constant* mixedGetTypeMethodRef;

extern struct Constant* mixedAddMethodRef;
extern struct Constant* mixedSubMethodRef;
extern struct Constant* mixedMulMethodRef;
extern struct Constant* mixedDivMethodRef;
extern struct Constant* mixedEquallyMethodRef;
extern struct Constant* mixedNotEquallyMethodRef;
extern struct Constant* mixedNotMethodRef;
extern struct Constant* mixedMoreMethodRef;
extern struct Constant* mixedLessMethodRef;

void generateCode(struct Root* root);

void writeConstantsTable();
void writeClassMetadata();
void writeFieldsTable();
void writeMethodsTable();
void writeMethod(struct Method* method);
void writeConstant(struct Constant* constant);
void writeField(struct Field* field);
void generateCodeForDefaultConstructor();
//void loadComparisonResultToStack(char* code, int* offset);

char* generateCodeForMethod(struct Method* method,int* codeLength);
void generateCodeToCallPrint(char* code, int* offset, int printMethodRefId);
void generateCodeForExprList(struct Method* method, struct expression_list* exprList, char* code, int* offset);
void generateCodeForExpr(struct Method* method, struct expression* expr, char* code, int* offset);
void generateCodeForStmtList(struct Method* method, struct statement_list* stmtList, char* code, int* offset);
void generateCodeForStmt(struct Method* method, struct statement* stmt, char* code, int* offset);
void generateCodeForVarDecl(struct Method* method, struct nvar* varDecl, char* code, int* offset);
void generateCodeForConstDecl(struct Method* method, struct nval* constDecl, char* code, int* offset);
void generateCodeForSingleAssignment(struct Method*  method, int localVarId, struct expression* expr, char* code, int* offset);

void generateCodeMixedIntObject(int value, char* code, int* offset);
void generateCodeMixedBoolObject(int value, char* code, int* offset);
void generateCodeMixedFloatObject(int value, char* code, int* offset);
void generateCodeMixedStringObject(int value, char* code, int* offset);
void generateCodeMixedObject(char* code, int* offset);
void generateCodeForIfStmt(struct Method* method, struct nif* ifStmt, char* code, int* offset);

void generateCodeMixedPrint(char* code, int* offset);

void Write(void* data, int count);
void writeU1();
void writeU2();
void writeU4();
void writeS2();
void writeS4();
void writeSf4();
void writeString(char* str);
void writeU1ToArray(char* code, int* offset);
void writeS1ToArray(char* code, int* offset);
void writeU2ToArray(char* code, int* offset);
void writeU4ToArray(char* code, int* offset);
void writeS2ToArray(char* code, int* offset);
void writeS4ToArray(char* code, int* offset);
void writeSf4ToArray(char* code, int* offset);

#endif
