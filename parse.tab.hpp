/* A Bison parser, made by GNU Bison 2.5.  */

/* Skeleton interface for Bison GLR parsers in C
   
      Copyright (C) 2002-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CLASS = 258,
     PROG = 259,
     VOID = 260,
     INT = 261,
     BOOL = 262,
     IF = 263,
     FOR = 264,
     BREAK = 265,
     CONT = 266,
     CALLOUT = 267,
     LC = 268,
     RC = 269,
     LP = 270,
     RP = 271,
     MULOP = 272,
     DIVOP = 273,
     ADDOP = 274,
     SUBOP = 275,
     MODOP = 276,
     NOTOP = 277,
     GT = 278,
     GE = 279,
     LT = 280,
     LE = 281,
     EQ = 282,
     NE = 283,
     TRUE = 284,
     FALSE = 285,
     DEC_LIT = 286,
     IDF = 287,
     HEX_LIT = 288,
     CHAR_LIT = 289,
     STR_LIT = 290,
     ANDOP = 291,
     OROP = 292,
     COMMA = 293,
     STOP = 294,
     RETURN = 295,
     ELSE = 296,
     EQUALS = 297,
     LS = 298,
     RS = 299
   };
#endif


#ifndef YYSTYPE
typedef union YYSTYPE
{

/* Line 2663 of glr.c  */
#line 17 "parse.ypp"

    ASTBoolLiteral *boo;
    ASTLiteral *lit;
    ASTIntLiteral *lint;
    ASTLocation *loc;
    ASTExpr *ex;
    ASTNormalMethodCall *nmc;
    ASTMethodCall *mc;
    ASTCalloutSub *acs;
    string *str;
    vector<ASTExpr>* fop;
    vector<ASTCalloutSub>* vacs;
    ASTAssignmentStmt *as;
    ASTStatement *st;
    ASTAssign *asdn;    
    vector<ASTStatement>*vstmt;
    vector<string>*vstr;
    ASTVarsSub * dbs;
    vector<ASTVarsSub> *vdbs;
    ASTBlock *blck;
    ASTIFStmt *ifelst;
    ASTMetDecl *mdc;
    vector<ASTMetDecl> *vmdc;
    ASTFieldDecl *fdl;
    vector<ASTFieldDecl> *vfdl;
    ASTVariable *vbl;
    vector<ASTVariable> *vvbl;





/* Line 2663 of glr.c  */
#line 126 "parse.tab.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{

  char yydummy;

} YYLTYPE;
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



extern YYSTYPE yylval;



