%{
int a = 0, b = 0;
%}

%option noyywrap

%%
public                  { printf("PUBLIC "); }
private                 { printf("PRIVATE "); }
protected               { printf("PROTECTED "); }
def                     { printf("DEF "); }
Int                     { printf("INT "); }
Float                   { printf("FLOAT "); }
Boolean                 { printf("BOOLEAN "); }
val                     { printf("VAL "); }
var                     { printf("VAR "); }
new                     { printf("NEW "); }
object                  { printf("OBJECT "); }
while                   { printf("WHILE "); }
class                   { printf("CLASS "); }
override                { printf("OVERRIDE "); }
if                      { printf("IF "); }
else                    { printf("ELSE "); }
match                   { printf("MATCH "); }
case                    { printf("CASE "); }
extends                 { printf("EXTENDS "); }
this                    { printf("THIS "); }
Array                   { printf("ARRAY "); }
lazy                    { printf("LAZY "); }
Long                    { printf("LONG "); }
Short                   { printf("SHORT "); }
final                   { printf("FINAL "); }
super                   { printf("FINAL "); }

"("                     { printf("( "); }
")"                     { printf(") "); }
"["                     { printf("[ "); }
"]"                     { printf("] "); }
"{"                     { printf("{ "); }
"}"                     { printf("} "); }
":"                     { printf(": "); }
";"                     { printf("; "); }
"="                     { printf("= "); }
"+"                     { printf("+ "); }
"-"                     { printf("- "); }
"*"                     { printf("* "); }
"/"                     { printf("/ "); }
"%"                     { printf("% "); }
"&"                     { printf("& "); }
"&&"                    { printf("&& "); }
"|"                     { printf("| "); }
"||"                    { printf("|| "); }
">"                     { printf("> "); }
">>"                    { printf(">> "); }
">>>"                   { printf(">>> "); }
"<"                     { printf("< "); }
"<<"                    { printf("<< "); }
"<<<"                   { printf("<<< "); }
"^"                     { printf("^ "); }
","                     { printf(", "); }
"."                     { printf(". "); }
">="                    { printf("MORE_EQ "); }
"<="                    { printf("LESS_EQ "); }
"+="                    { printf("PLUS_EQ "); }
"-="                    { printf("LESS_EQ "); }
"*="                    { printf("MUL_EQ "); }
"/="                    { printf("DIV_EQ "); }
"++"                    { printf("INCREASE "); }
"--"                    { printf("REDUCE "); }

([1-9][0-9]*)|0         { printf("NUMBER_INT "); }
[1-9][0-9]*\.[0-9]*     { printf("NUMBER_FLOAT "); }

[A-Za-z_][A-Za-z_0-9]*  { printf("ID "); }

.                   {}
%%
