/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015 Free Software Foundation, Inc.

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

#ifndef YY_YY_TOKEN_H_INCLUDED
# define YY_YY_TOKEN_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "expression.y" /* glr.c:197  */

        #include "ast.h"

#line 48 "token.h" /* glr.c:197  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    OP_ADD = 258,
    OP_SUB = 259,
    OP_MULT = 260,
    OP_DIV = 261,
    TK_L_PAR = 262,
    TK_R_PAR = 263,
    TK_NUMBER = 264,
    TK_HEX = 265,
    TK_BIN = 266,
    TK_DEC = 267,
    TK_EOL = 268,
    TK_EOF = 269,
    TK_ERROR = 270,
    TK_EQUAL = 271,
    TK_RW_PRINT = 272,
    TK_COMMA = 273,
    TK_RW_IF = 274,
    TK_RW_ELSE = 275,
    TK_R_BRACE = 276,
    TK_L_BRACE = 277,
    TK_COMP_GREATER = 278,
    TK_COMP_GREATER_EQUAL = 279,
    TK_COMP_LESS = 280,
    TK_COMP_NEQUAL = 281,
    TK_COMP_LESS_EQUAL = 282,
    TK_COMP_EQUAL = 283,
    TK_VARIABLE = 284
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 21 "expression.y" /* glr.c:197  */

        Statement *statement_t;
        Expr *expr_t;
        int int_t;
        string* string_t;

#line 97 "token.h" /* glr.c:197  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TOKEN_H_INCLUDED  */
