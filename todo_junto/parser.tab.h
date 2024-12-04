/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_MYPARSER_PARSER_TAB_H_INCLUDED
# define YY_MYPARSER_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef MYPARSERDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define MYPARSERDEBUG 1
#  else
#   define MYPARSERDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define MYPARSERDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined MYPARSERDEBUG */
#if MYPARSERDEBUG
extern int myparserdebug;
#endif

/* Token kinds.  */
#ifndef MYPARSERTOKENTYPE
# define MYPARSERTOKENTYPE
  enum myparsertokentype
  {
    MYPARSEREMPTY = -2,
    MYPARSEREOF = 0,               /* "end of file"  */
    MYPARSERerror = 256,           /* error  */
    MYPARSERUNDEF = 257,           /* "invalid token"  */
    NUMERO = 258,                  /* NUMERO  */
    IDENTIFICADOR = 259,           /* IDENTIFICADOR  */
    IF = 260,                      /* IF  */
    THEN = 261,                    /* THEN  */
    ELSE = 262,                    /* ELSE  */
    END = 263,                     /* END  */
    REPEAT = 264,                  /* REPEAT  */
    UNTIL = 265,                   /* UNTIL  */
    READ = 266,                    /* READ  */
    WRITE = 267,                   /* WRITE  */
    ASSIGN = 268,                  /* ASSIGN  */
    LT = 269,                      /* LT  */
    GT = 270,                      /* GT  */
    EQ = 271,                      /* EQ  */
    NE = 272,                      /* NE  */
    LE = 273,                      /* LE  */
    GE = 274,                      /* GE  */
    PLUS = 275,                    /* PLUS  */
    MINUS = 276,                   /* MINUS  */
    MUL = 277,                     /* MUL  */
    DIV = 278,                     /* DIV  */
    LPAREN = 279,                  /* LPAREN  */
    RPAREN = 280                   /* RPAREN  */
  };
  typedef enum myparsertokentype myparsertoken_kind_t;
#endif

/* Value type.  */
#if ! defined MYPARSERSTYPE && ! defined MYPARSERSTYPE_IS_DECLARED
union MYPARSERSTYPE
{
#line 21 "parser.y"

    int num;                 // Para números
    char* id;                // Para identificadores
    SyntaxTreeNode *node;    // Para nodos del árbol sintáctico

#line 103 "parser.tab.h"

};
typedef union MYPARSERSTYPE MYPARSERSTYPE;
# define MYPARSERSTYPE_IS_TRIVIAL 1
# define MYPARSERSTYPE_IS_DECLARED 1
#endif


extern MYPARSERSTYPE myparserlval;


int myparserparse (void);


#endif /* !YY_MYPARSER_PARSER_TAB_H_INCLUDED  */
