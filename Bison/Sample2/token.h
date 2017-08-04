/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#line 2 "expression.y" /* yacc.c:1909  */

        #include "ast.h"

#line 48 "token.h" /* yacc.c:1909  */

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
    TK_VARIABLE = 265,
    TK_HEX = 266,
    TK_BIN = 267,
    TK_DEC = 268,
    TK_EOL = 269,
    TK_EOF = 270,
    TK_ERROR = 271,
    TK_EQUAL = 272,
    TK_RW_PRINT = 273,
    TK_COMMA = 274,
    TK_RW_IF = 275,
    TK_RW_ELSE = 276,
    TK_R_BRACE = 277,
    TK_L_BRACE = 278,
    TK_COMP_GREATER = 279,
    TK_COMP_GREATER_EQUAL = 280,
    TK_COMP_LESS = 281,
    TK_COMP_NEQUAL = 282,
    TK_COMP_LESS_EQUAL = 283,
    TK_COMP_EQUAL = 284
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 17 "expression.y" /* yacc.c:1909  */

        Statement *statement_t;
        Expr *expr_t;
        int int_t;

#line 96 "token.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TOKEN_H_INCLUDED  */
