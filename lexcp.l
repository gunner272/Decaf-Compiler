%{
#include <bits/stdc++.h>    
#include "node.cpp"
#include "parse.tab.hpp"
using namespace std;
%}

%option noyywrap

DIGIT [0-9]
ID [a-z][a-z0-9]*
HEX_DIG DIGIT|a|b|c|d|e|f|A|B|C|D|E|F

%%
'.' {return CHAR_LIT;}
\".*\" {yylval.str=new string(yytext,yyleng);return STR_LIT;}
; {return STOP;}
class {return CLASS;}
Program {return PROG;}
void {return VOID;}
int { return INT;}
return {return RETURN;}
boolean {return BOOL;}
if {return IF;}
else {return ELSE;}
for {return FOR;}
break {return BREAK;}
continue {return CONT;}
callout {return CALLOUT;}
\{ {return LC;}
\} {return RC;}
\( {return LP;}
\) {return RP;}
\[ {return LS;}
\] {return RS;}
, {return COMMA;}
\* {return MULOP;}
\/ {return DIVOP;}
\+ {return ADDOP;}
\- {return SUBOP;}
% {return MODOP;}
! {return NOTOP;}
>  {return GT;}
>= {return GE;}
\<  {return LT;}
\<= {return LE;}
== {return EQ;}
!= {return NE;}
= {return EQUALS;}
&& {return ANDOP;}
\|\| {return OROP;}
true {return TRUE;}
false {return FALSE;}
{DIGIT}{DIGIT}* {yylval.str = new string (yytext,yyleng);return DEC_LIT;}
{ID} {yylval.str = new string(yytext, yyleng);return IDF;}
0x{HEX_DIG}{HEX_DIG}* {return HEX_LIT;}
<<EOF>>  { return 0; }
[ \t\n]+ { }
.        { cerr << "Unrecognized token!" << endl; exit(1); }
%%

