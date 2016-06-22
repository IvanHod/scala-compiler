#include "generationCode.h"

unsigned char
ICONST_M1 = 0x2, // REMOVE THIS (SIGNED)
ICONST_0 = 0x3,
ICONST_1 = 0x4,
ICONST_2 = 0x5,
ICONST_3 = 0x6,
ICONST_4 = 0x7,
ICONST_5 = 0x8,
BIPUSH = 0x10,
SIPUSH = 0x11,
LDC = 0x12,
LDC_W = 0x13,
ILOAD = 0x15,
FLOAD = 0x17,
ALOAD = 0x19,
ALOAD_0 = 0x2a,
ISTORE = 0x36,
FSTORE = 0x38,
ASTORE = 0x3A,
POP = 0x57,
DUP = 0x58,
DUP2 = 0x5C,
IADD = 0x60,
FADD = 0x62,
IMUL = 0x68,
FMUL = 0x6A,
ISUB = 0x64,
FSUB = 0x66,
IDIV = 0x6C,
FDIV = 0x6e,
IINC = 0x84,
FNEG = 0x76,
INEG = 0x74,
IREM = 0x70,
IF_ICMPEQ = 0x9F,
IF_ICMPNE = 0xA0,
IF_ICMPLT = 0xA1,
IF_ICMPLE = 0xA4,
IF_ICMPGT = 0xA3,
IF_ICMPGE = 0xA2,
FCMPG = 0x96,
FCMPL = 0x95,
IFEQ = 0x99,
IFNE = 0x9A,
IFLT = 0x55,
IFLE = 0x9E,
IFGT = 0x9D,
IFGE = 0x9C,
IF_ACMPEQ = 0xA5,
IF_ACMPNE = 0xA6,
GOTO = 0xA7,
TABLESWITCH = 0xAA,
LOOKUPSWITCH = 0xAB,
NEWARRAY = 0xBC,
T_BOOLEAN = 4,
T_CHAR = 5,
T_FLOAT = 6,
T_DOUBLE = 7,
T_BYTE = 8,
T_SHORT = 9,
T_INT = 10,
T_LONG = 11,
ANEWARRAY = 0xBD,
ARRAY_LENGTH = 0xBE,
IALOAD = 0x2E,
AALOAD = 0x32,
FALOAD = 0x30,
IASTORE = 0x4F,
AASTORE = 0x53,
FASTORE = 0x51,
NEW = 0xBB, // create new object
GETFIELD = 0xB4,
PUTFIELD = 0xB5,
INSTANCE = 0xC1,
INVOKEVIRTUAL = 0xB6,
INVOKESPECIAL = 0xB7,
INVOKESTATIC = 0xB8,
IRETURN = 0xAC,
FRETURN = 0xAE,
ARETURN = 0xB0,
RETURN = 0xB1
;

static unsigned short
ACC_SUPER = 0x0020,
ACC_PUBLIC = 0x0001,
ACC_STATIC = 0x0008;

extern int i;

void generateCode(struct Root* root){
    program = root;
    // 0200 user can only write file
    char *filePath = "/home/ivan/qt-creator-projects/scala-compiler/tests/HelloWorld.class";
    if ((fh = open(filePath, O_BINARY | O_WRONLY | O_TRUNC | O_CREAT, 0200)) == -1) {
        freopen("CON", "w", stdout);
        printf("Failed to open class file\n");
        return;
    }
    else {
        printf("Generating code...\n");
        //magic number
        u4 = htonl(0xCAFEBABE);
        writeU4();

        //minor version
        u2 = htons(3);
        writeU2();

        //major version of jdk 8
        u2 = htons(0x2D);
        writeU2();

        writeConstantsTable();

        writeClassMetadata();

        writeFieldsTable();

        writeMethodsTable();

        //write number of class's attributes(usally 0)
        u2 = htons(1);
        writeU2();
        //write class's attributes
        //index of attribute name
        u2 = htons(constantSourceFile->id);
        writeU2();
        //attribute length
        u4 = htonl(2);
        writeU4();
        u2 = htons(constantSourceFileName->id);
        writeU2();

    }
    close(fh);
}



void writeClassMetadata() {
    //access flags
    int accessFlags = ACC_SUPER | ACC_PUBLIC;
    u2 = htons(accessFlags);
    writeU2();

    //write constants class
    u2 = htons(constantClass->id);
    writeU2();

    //write parent class(Object);
    u2 = htons(constantObjectClass->id);
    writeU2();

    //number of interface
    u2 = 0;
    writeU2();

    //write interfaces (nothing to write =)) )
}

void writeFieldsTable() {
    //write number of field
    int fieldCount = hashtable_size(fieldsTable);
    u2 = htons(fieldCount);
    writeU2();
    if( fieldCount > 0) {
        //write field table
        HashTableIter i;
        hashtable_iter_init(&i, fieldsTable);
        while (hashtable_iter_has_next(&i)) {
            TableEntry *e;
            hashtable_iter_next(&i, &e);
            struct  Field* field = (struct Field*)e;
            writeField(field);
        }
    }
}


//not very sure. this function need to be tested
void writeField(struct Field* field) {
    //write access flag
    u2 = htons(ACC_PUBLIC);
    writeU2();

    //write number of CONSTANT_utf8, which contains field's name
    char* fieldName = field->constFieldref->const2->const1->utf8;
    struct Constant* constant = getConstantUtf8(fieldName);
    u2 = htons(constant->id);
    writeU2();

    //write numbef of CONSTANT_utf8, which contains field's description (aka field's type)
    char* fieldDescription = field->constFieldref->const2->const2->utf8;
    constant = getConstantUtf8(fieldDescription);
    u2 = htons(constant->id);
    writeU2();

    //write number of field's attribue
    u2 = 0;
    writeU2();

    //write attribute's description(nothing to write, as usually)

}

void writeMethodsTable() {

    int methodCount = hashtable_size(methodsTable) + 1;
    u2 = htons(methodCount);
    writeU2();

    generateCodeForDefaultConstructor();

    //write methods
    HashTableIter i;
    hashtable_iter_init(&i, methodsTable);
    int size = hashtable_size(methodsTable);
    while (hashtable_iter_has_next(&i)) {
        TableEntry *e;
        hashtable_iter_next(&i, &e);
        struct Method* method = (struct Method*) e->value;
        writeMethod(method);
    }
}

void writeMethod(struct Method* method) {
    char* methodName = method->constMethodref->const2->const1->utf8;
    u2 = htons(ACC_PUBLIC);
    //if (strcmp("main", method->functionDecl->name) == 0) {
        u2 = htons(ACC_PUBLIC | ACC_STATIC);
    //}
    //else {
    //    u2 = htons(ACC_PUBLIC);
    //}
    writeU2();

    //write id of  constant utf8, which contains method's name
    u2 = htons(method->constMethodref->const2->const1->id);
    writeU2();

    //write id of constant utf8, which contains method's description
    u2 = htons(method->constMethodref->const2->const2->id);
    writeU2();

    //method's attribute count
    u2 = htons(1);
    writeU2();

    //write method's attribute table
    //constant utf8's id
    u2 = htons(constantCode->id);
    writeU2();

    //generate code for method
    int codeLength;
    char* code = generateCodeForMethod(method, &codeLength);

    //define length of attribute code
    int attributeCodeLength = 12 + codeLength;
    u4 = htonl(attributeCodeLength);
    writeU4();
    //stack's size
    u2 = htons(100);
    writeU2();
    //local variable count
    int localVarsCount;
    localVarsCount = list_size(method->localVariablesTable); // +1 if dinamic method

    u2 = htons(localVarsCount);
    writeU2();

    //write method's bytecode size
    u4 = htonl(codeLength);
    writeU4();
    //write bytecode of method
    for (i = 0; i < codeLength; ++i) {
        u1 = code[i];
        writeU1();
    }

    //write number of exception
    u2 = htons(0);
    writeU2();
    //write exception table(no exception)

    //write number of attribute's attribute
    u2 = htons(0);
    writeU2();
}

char* generateCodeForMethod(struct Method* method, int* codeLength) {
    char* code = (char*)malloc(2000 * sizeof(char));
    int* offset = (int*)malloc(sizeof(int));
    *offset = 0;
    struct statement* stmt = method->functionDecl->body->first;
    while (stmt != NULL) {
        generateCodeForStmt(method, stmt, code, offset);
        stmt = stmt->next;
    }
    if (method->returnType->typeName == VOID_TYPE_NAME) {
        u1 = RETURN;
    } else {
        // !Ass
        struct expression *_expr = method->functionDecl->body->last->expr_list->first->left;
        u1 = ILOAD;
        writeU1ToArray(code, offset);
        //write the id number of id_expression from local var table
        u1 = _expr->semanticType->idNum;
        writeU1ToArray(code, offset);
        u1 = IRETURN;
    }
    writeU1ToArray(code, offset);

    *codeLength = *offset;
    return code;
}

void generateCodeToCallPrint(char* code, int* offset, int printMethodRefId) {
    u1 = INVOKESTATIC;
    writeU1ToArray(code, offset);
    //write id of constant method ref print integer
    u2 = htons(printMethodRefId);
    writeU2ToArray(code, offset);
}

void generateCodeForExprList(struct Method* method, struct expression_list* exprList, char* code, int* offset) {
    struct expression* expr = exprList->first;
    while (expr != NULL) {
        generateCodeForExpr(method, expr, code, offset);
        expr = expr->next;
    }
}

void generateCodeForExpr(struct Method* method, struct expression* expr, char* code, int* offset) {
    switch(expr->type) {
    case id:
        switch (expr->semanticType->typeName) {
        case BOOL_TYPE_NAME:
        case INT_TYPE_NAME: u1 = ILOAD; break;
        case FLOAT32_TYPE_NAME: u1 = FLOAD; break;
        case STRING_TYPE_NAME: u1 = ALOAD; break;
        default: break;
        }
        //write load instruction
        writeU1ToArray(code, offset);
        //write the id number of id_expression from local var table
        u1 = expr->semanticType->idNum;
        writeU1ToArray(code, offset);

        u1 = INVOKESTATIC;
        writeU1ToArray(code, offset);
        //write id of constant method ref print integer

        switch (expr->semanticType->typeName) {
        case BOOL_TYPE_NAME: u2 = htons(mixedSetBoolMethodRef->id); break;
        case INT_TYPE_NAME: u2 = htons(mixedSetIntMethodRef->id); break;
        case FLOAT32_TYPE_NAME: u2 = htons(mixedSetFloatMethodRef->id); break;
        case STRING_TYPE_NAME: u2 = htons(mixedSetStringMethodRef->id); break;
        default: break;
        }
        writeU2ToArray(code, offset);
        break;
    case Int:
        generateCodeMixedIntObject(expr->semanticType->constantExpressionNum, code, offset);
        break;
    case Bool:
        generateCodeMixedBoolObject(expr->semanticType->constantExpressionNum, code, offset);
        break;
    case Float:
        generateCodeMixedFloatObject(expr->semanticType->constantExpressionNum, code, offset);
    break;
    case String:
        generateCodeMixedStringObject(expr->semanticType->constantExpressionNum, code, offset);
        break;
    case assigment:
        generateCodeForSingleAssignment(method, expr->left->semanticType->idNum, expr->rigth, code, offset);
            break;
    case equal:
    generateCodeForExpr(method, expr->left, code, offset);
    generateCodeForExpr(method, expr->rigth, code, offset);

    u1 = INVOKESTATIC;
    writeU1ToArray(code, offset);

    //id of local variable
    u2 = htons(mixedEquallyMethodRef->id);
    writeU2ToArray(code, offset);
        break;
    case no_equal:
        generateCodeForExpr(method, expr->left, code, offset);
        generateCodeForExpr(method, expr->rigth, code, offset);

        u1 = INVOKESTATIC;
        writeU1ToArray(code, offset);

        //id of local variable
        u2 = htons(mixedNotEquallyMethodRef->id);
        writeU2ToArray(code, offset);
        break;
    case no:
    generateCodeForExpr(method, expr->left, code, offset);
        break;
    case more:
        generateCodeForExpr(method, expr->left, code, offset);
        generateCodeForExpr(method, expr->rigth, code, offset);

        u1 = INVOKESTATIC;
        writeU1ToArray(code, offset);

        //id of local variable
        u2 = htons(mixedMoreMethodRef->id);
        writeU2ToArray(code, offset);
        break;
    case more_eq:
        break;
    case less:
        generateCodeForExpr(method, expr->left, code, offset);
        generateCodeForExpr(method, expr->rigth, code, offset);

        u1 = INVOKESTATIC;
        writeU1ToArray(code, offset);

        //id of local variable
        u2 = htons(mixedLessMethodRef->id);
        writeU2ToArray(code, offset);
        break;
    case less_eq:
        break;
    case plus:
        generateCodeForExpr(method, expr->left, code, offset);
        generateCodeForExpr(method, expr->rigth, code, offset);

        u1 = INVOKESTATIC;
        writeU1ToArray(code, offset);

        //id of local variable
        u2 = htons(mixedAddMethodRef->id);
        writeU2ToArray(code, offset);
        break;
    case sub:
        generateCodeForExpr(method, expr->left, code, offset);
        generateCodeForExpr(method, expr->rigth, code, offset);

        u1 = INVOKESTATIC;
        writeU1ToArray(code, offset);

        //id of local variable
        u2 = htons(mixedSubMethodRef->id);
        writeU2ToArray(code, offset);
        break;
    case mul:
        generateCodeForExpr(method, expr->left, code, offset);
        generateCodeForExpr(method, expr->rigth, code, offset);

        u1 = INVOKESTATIC;
        writeU1ToArray(code, offset);

        //id of local variable
        u2 = htons(mixedMulMethodRef->id);
        writeU2ToArray(code, offset);
        break;
    case DIV:
        break;
    case div_residue:
        break;
    case unarOR:
        break;
    case _or:
        break;
    case unarXOR:
        break;
    case _xor:
        break;
    case _and:
        break;
    case call_func: {
        char* calleeName = expr->String;
        struct Method* calleeMethod = getMethod(calleeName);
        //surely, every methods in this class is static
        //load referece to object of called method
        //fill stack with arguments
        if (expr->expr_list != NULL) {
            struct expression* arg = expr->expr_list->first;
            while (arg != NULL) {
                generateCodeForExpr(method, arg, code, offset);
                u1 = INVOKEVIRTUAL;
                writeU1ToArray(code, offset);

                //store value on top of stack to local variable
                switch (expr->semanticType->typeName) {
                case INT_TYPE_NAME: {
                    u2 = htons(mixedGetIntMethodRef->id);
                    break;
                }
                case BOOL_TYPE_NAME: {
                    u2 = htons(mixedGetBoolMethodRef->id);
                    break;
                }
                case FLOAT32_TYPE_NAME: {
                    u2 = htons(mixedGetFloatMethodRef->id);
                    break;
                }
                case STRING_TYPE_NAME: {
                    u2 = htons(mixedGetStringMethodRef->id);
                    break;
                }
                default: break;
                }
                writeU2ToArray(code, offset);
                arg = arg->next;
            }
            //
        }
        u1 = INVOKESTATIC;
        writeU1ToArray(code, offset);
        //write id of constant method ref
        u2 = htons(calleeMethod->constMethodref->id);
        writeU2ToArray(code, offset);
        break;
    }
    case println: {
        struct expression *simpleExpr = expr->expr_list->first;
        while(simpleExpr != NULL) {
            generateCodeForExpr(method, simpleExpr, code, offset);
            if(simpleExpr->type == call_func) {
                u1 = INVOKESTATIC;
                writeU1ToArray(code, offset);
                //write id of constant method ref print integer
                u2 = htons(mixedSetIntMethodRef->id);
                writeU2ToArray(code, offset);
            }
            generateCodeMixedPrint(code, offset);
            simpleExpr = simpleExpr->next;
        }
        break;
    }
    default: break;
    }
}

void generateCodeForStmtList(struct Method* method, struct statement_list* stmtList, char* code, int* offset) {
    struct statement* stmt = stmtList->first;
    while (stmt != NULL) {
        generateCodeForStmt(method, stmt, code, offset);
        stmt = stmt->next;
    }
}

void generateCodeForStmt(struct Method* method, struct statement* stmt, char* code, int* offset) {
    switch (stmt->type) {
        case EXPR_LIST: {
            generateCodeForExprList(method, stmt->expr_list, code, offset);
            break;
        }
        case STMT_LIST: {
            generateCodeForStmtList(method, stmt->stmt_list, code, offset);
            break;
        }
        case NVAR: {
            generateCodeForVarDecl(method, stmt->to_print_var, code, offset);
            break;
        }
        case NVAL: {
            //generateCodeForConstDecl(method, stmt->to_print_val, code, offset);
            break;
        }
        case NIF: {
            generateCodeForIfStmt(method, stmt->_if, code, offset);
            break;
        }
        case NMATCH: {
            //generateCodeForSwitchStmt(method, stmt->switchStmt, code, offset);
            break;
        }
    default: break;
    }
}

void generateCodeForVarDecl(struct Method* method, struct nvar* varSpec, char* code, int* offset) {
    // var a : return_value = result;
    struct id_in_list* id = varSpec->idList->first;
    struct Expression* expr = varSpec->result;
    while (id != NULL) {
        generateCodeForSingleAssignment(method, id->idNum, expr, code, offset);
        id = id->next;
    }
}

void generateCodeForConstDecl(struct Method* method, struct nval* val, char* code, int* offset) {
    //one line constant declaration

}

void generateCodeForIfStmt(struct Method* method, struct nif* ifStmt, char* code, int* offset) {
    //generate code for condition-expression of if stmt
    //if type (if statement; expression - not supported if type) is eliminated in semantic checking
    generateCodeForExpr(method, ifStmt->expr, code, offset);


    u1 = ISTORE;
    writeU1ToArray(code, offset);
    u2 = htons(mixedGetBoolMethodRef->id);
    writeU2ToArray(code, offset);

    //write command ifeq
    int ifeqPos = *offset;
    int gotoPos;
    u1 = IFEQ;
    writeU1ToArray(code, offset);

    //temporary 2 bytes for displacement
    s2 = htons(0);
    writeS2ToArray(code,offset);
    //generate code for block if condition is correct
    bool isContainReturn = false;
    generateCodeForStmtList(method, ifStmt->to_then, code, offset);
    if (ifStmt->to_else != NULL) {
        if (ifStmt->to_else != NULL) {
            //block of else
            //isContainReturn = isContainStatementType(ifStmt->block->stmtList, RETURN_STMT);
        }
        if (!isContainReturn) {
            gotoPos = *offset;
            u1 = GOTO;
            writeU1ToArray(code, offset);
            // write temporary displacement value to offset
            s2 = htons(0);
            writeS2ToArray(code, offset);
        }
    }
    //reserve value of offset
    /*int tmp = *offset;
    s2 = htons(*offset - ifeqPos);
    //fix the value of ifeq's operand
    *offset = ifeqPos +1 ;
    writeS2ToArray(code, offset);
    //restore value of offset for future instructions
    *offset = tmp;
    if (ifStmt->elseBlock != NULL) {
        if (ifStmt->elseBlock->ifStmt != NULL) {
            generateCodeForIfStmt(method, ifStmt, code, offset);
        }
        else if (ifStmt->elseBlock->block != NULL) {
            generateCodeForStmtList(method, ifStmt->elseBlock->block->stmtList, code, offset);
        }
        if (!isContainReturn) {
            tmp = *offset;
            s2 = htons(*offset - gotoPos);
            //fix value of goto's operand
            *offset = gotoPos + 1;
            writeS2ToArray(code, offset);
            *offset = tmp;
        }
    }*/
}

void generateCodeForSingleAssignment(struct Method*  method, int localVarId, struct expression* expr, char* code, int* offset) {
    //generate code for right expression
    // expr - rigth expression
    generateCodeForExpr(method, expr, code, offset);

    // получить необходимое значение
    u1 = INVOKEVIRTUAL;
    writeU1ToArray(code, offset);

    //store value on top of stack to local variable
    switch (expr->semanticType->typeName) {
    case INT_TYPE_NAME: {
        //write id of constant method ref print integer
        u2 = htons(mixedGetIntMethodRef->id);
        u1 = ISTORE;
        break;
    }
    case BOOL_TYPE_NAME: {
        u2 = htons(mixedGetBoolMethodRef->id);
        u1 = ISTORE;
        break;
    }
    case FLOAT32_TYPE_NAME: {
        u2 = htons(mixedGetFloatMethodRef->id);
        u1 = FSTORE;
        break;
    }
    case STRING_TYPE_NAME: {
        u2 = htons(mixedGetStringMethodRef->id);
        u1 = ASTORE;
        break;
    }
    default: break;
    }
    writeU2ToArray(code, offset);

    writeU1ToArray(code, offset);
    //id of local variable
    u1 = localVarId;
    writeU1ToArray(code, offset);
}

void generateCodeForDefaultConstructor() {
    u2 = htons(ACC_PUBLIC);
    writeU2();

    //write id of  constant utf8, which contains method's name
    struct Constant* constantInit = getConstantUtf8("<init>");
    u2 = htons(constantInit->id);
    writeU2();

    //write id of constant utf8, which contains method's description
    struct Constant* constantConstructorDescriptor = getConstantUtf8("()V");
    u2 = htons(constantConstructorDescriptor->id);
    writeU2();

    //method's attribute count
    u2 = htons(1);
    writeU2();

    //write method's attribute table
    //constant utf8's id
    u2 = htons(constantCode->id);
    writeU2();

    int codeLength = 5;

    //define length of attribute code
    int attributeCodeLength = 12 + codeLength;
    u4 = htonl(attributeCodeLength);
    writeU4();
    //stack's size
    u2 = htons(10);
    writeU2();
    //local variable count
    int localVarsCount = 1; //include variable this(id = 0)
    u2 = htons(localVarsCount);
    writeU2();

    //write method's bytecode size
    u4 = htonl(codeLength);
    writeU4();

    /*-----------------------------------*/
    //write bytecode of method
    u1 = ALOAD_0;
    writeU1();
    //call to object's contructor
    u1 = INVOKESPECIAL;
    writeU1();
    u2 = htons(objectConstructorMethodRef->id);
    writeU2();
    u1 = RETURN;
    writeU1();
    /*-----------------------------------*/

    //write number of exception
    u2 = htons(0);
    writeU2();
    //write exception table(no exception)

    //write number of attribute's attribute
    u2 = htons(0);
    writeU2();
}

void writeConstantsTable() {
    //constant's size +1
    int constantCount = list_size(constantsTable);
    u2 = htons(constantCount + 1);
    writeU2();

    //constants
    struct Constant* constant = NULL;
    for (i = 0; i < constantCount; ++i) {
        list_get_at(constantsTable, i, &constant);
        writeConstant(constant);
    }
}

void writeString(char* str) {
    Write((void*)str, strlen(str));
}

void writeU1() {
    Write((void*)&u1, 1);
}

void writeU2() {
    Write((void*)&u2, 2);
}

void writeU4() {
    Write((void*)&u4, 4);
}

void writeS2() {
    Write((void*)&s2, 2);
}

void writeS4() {
    Write((void*)&s4, 4);
}

void writeSf4() {
    int*tmp = (int*)&sf4;
    int value = htonl(*tmp);
    Write((void*)&value, 4);
}

void writeU1ToArray(char* code, int* offset) {
    code[*offset] = u1;
    *offset += 1;
}

void writeS1ToArray(char*code, int* offset) {
    code[*offset] = s1;
    *offset += 1;
}

void writeU2ToArray(char* code, int* offset) {
    code[*offset] = u2 & 0xFF;
    code[*offset + 1] = (u2 >> 8) & 0xFF;
    *offset += 2;
}

void writeU4ToArray(char* code, int* offset) {
    code[*offset] = u4 & 0xFF;
    code[*offset + 1] = (u4 >> 8) & 0xFF;
    code[*offset + 2] = (u4 >> 16) & 0xFF;
    code[*offset + 3] = (u4 >> 24) & 0xFF;
    *offset += 4;
}

void writeS2ToArray(char* code, int* offset) {
    code[*offset] = s2 & 0xFF;
    code[*offset + 1] = (s2 >> 8) & 0xFF;
    *offset += 2;
}

void writeS4ToArray(char* code, int* offset) {
    int*tmp = (int*)&s4;
    int value = *tmp;
    code[*offset] = value & 0xFF;
    code[*offset + 1] = (value >> 8) & 0xFF;
    code[*offset + 2] = (value >> 16) & 0xFF;
    code[*offset + 3] = (value >> 24) & 0xFF;
    *offset += 4;
}

void writeSf4ToArray(char* code, int* offset) {
    int* floatToIntPointer = (int*)&sf4;
    int intSf4 = *floatToIntPointer;
    code[*offset] = intSf4 & 0xFF;
    code[*offset + 1] = (intSf4 >> 8) & 0xFF;
    code[*offset + 2] = (intSf4 >> 16) & 0xFF;
    code[*offset + 3] = (intSf4 >> 24) & 0xFF;
    *offset += 4;
}

void writeConstant(struct Constant* constant) {
    u1 = constant->type;
    Write((void*)&u1, 1);
    int length;
    switch (constant->type) {
    case CONSTANT_Utf8:
        length = strlen(constant->utf8);
        u2 = htons(length);
        //length
        Write((void*)&u2, 2);
        //data
        Write((void*)constant->utf8, length);
        break;
    case CONSTANT_Integer:
        s4 = htonl(constant->intValue);
        Write((void*)&s4, 4);
        break;
    case CONSTANT_Boolean:
        s4 = htonl(constant->boolValue);
        Write((void*)&s4, 4);
        break;
    case CONSTANT_Float:
        sf4 = constant->floatValue;
        writeSf4();
        break;
    case CONSTANT_String:
    case CONSTANT_Class:
        u2 = htons(constant->const1->id);
        Write((void*)&u2, 2);
        break;
    case CONSTANT_Fieldref:
    case CONSTANT_Methodref:
    case CONSTANT_NameAndType:
        //first constant's id
        u2 = htons(constant->const1->id);
        Write((void*)&u2, 2);
        //second constant's id
        u2 = htons(constant->const2->id);
        Write((void*)&u2, 2);
        break;
    }
}

void Write(void* data, int count) {
    int result = write(fh, data, count);
    if (result < 0) {
        printf("Failed to write data to file\n");
    }
}

/* -------------------------------------------------- Mixed class ---------------------------------------*/

void generateCodeMixedPrint(char* code, int* offset) {
    u1 = INVOKESTATIC;
    writeU1ToArray(code, offset);
    //write id of constant method ref print integer
    u2 = htons(mixedPrintMethodRef->id);
    writeU2ToArray(code, offset);
}

void generateCodeMixedIntObject(int value, char* code, int* offset) {
    // загружают целое число
    u1 = LDC;
    //write load instruction
    writeU1ToArray(code, offset);
    //write the id number of id_expression from local var table
    u1 = value;
    writeU1ToArray(code, offset);

    u1 = INVOKESTATIC;
    writeU1ToArray(code, offset);
    //write id of constant method ref print integer
    u2 = htons(mixedSetIntMethodRef->id);
    writeU2ToArray(code, offset);
}

void generateCodeMixedBoolObject(int value, char* code, int* offset) {
    // загружают целое число
    u1 = LDC;
    //write load instruction
    writeU1ToArray(code, offset);
    //write the id number of id_expression from local var table
    u1 = value;
    writeU1ToArray(code, offset);

    u1 = INVOKESTATIC;
    writeU1ToArray(code, offset);
    //write id of constant method ref print integer
    u2 = htons(mixedSetBoolMethodRef->id);
    writeU2ToArray(code, offset);
}

void generateCodeMixedFloatObject(int value, char* code, int* offset) {
    // загружают целое число
    u1 = LDC;
    //write load instruction
    writeU1ToArray(code, offset);
    //write the id number of id_expression from local var table
    u1 = value;
    writeU1ToArray(code, offset);

    u1 = INVOKESTATIC;
    writeU1ToArray(code, offset);
    //write id of constant method ref print integer
    u2 = htons(mixedSetFloatMethodRef->id);
    writeU2ToArray(code, offset);
}

void generateCodeMixedStringObject(int value, char* code, int* offset) {
    // загружают целое число
    u1 = LDC;
    //write load instruction
    writeU1ToArray(code, offset);
    //write the id number of id_expression from local var table
    u1 = value;
    writeU1ToArray(code, offset);

    u1 = INVOKESTATIC;
    writeU1ToArray(code, offset);
    //write id of constant method ref print integer
    u2 = htons(mixedSetStringMethodRef->id);
    writeU2ToArray(code, offset);
}
