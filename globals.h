/****************************************************/
/* File: globals.h                                  */
/* Tipos y variables globales                       */
/* Debe venir antes de los otros                    */
/* archivos en los include                          */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRESERVED = numero de palabras reservadas */
#define MAXRESERVED 14

typedef enum 
    /* book-keeping tokens */
   {ENDFILE,ERROR,
    /* palabras reservadas */
    FOR,TO,STEP,DO,ENDFOR,IF,THEN,ELSE,END,REPEAT,UNTIL,READ,WRITE,WRITELN,
    /* tokens de caracteres multiples */
    ID,NUM,
    /* simbolos especiales */
    ASSIGN,EQ,LT,PLUS,MINUS,TIMES,OVER,LPAREN,RPAREN,SEMI
   } TokenType;

extern FILE* source; /* archivo de texto fuente */
extern FILE* listing; /* listado del texto de salida */
extern FILE* code; /* archivo de texto para el simulador TM */

extern int lineno; /* numero de linea fuente para el listing */

/**************************************************/
/*******   Arbol Sintáctico para el parseo ********/
/**************************************************/

typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfK,ForsK,RepeatK,AssignK,ReadK,WriteK,WritelnK} StmtKind;
typedef enum {OpK,ConstK,IdK} ExpKind;

/* ExpType es usado para chequeo de tipos */
typedef enum {Void,Integer,Boolean} ExpType;

#define MAXCHILDREN 5

typedef struct treeNode
   { struct treeNode * child[MAXCHILDREN];
     struct treeNode * sibling;
     int lineno;
     NodeKind nodekind;
     union { StmtKind stmt; ExpKind exp;} kind;
     union { TokenType op;
             int val;
             char * name; } attr;
     ExpType type; /* para el chequeo de tipos de expresiones */
   } TreeNode;

/**************************************************/
/**********   Banderas para el rastreo  ***********/
/**************************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;

/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
 * and lookups to be reported to the listing file
 */
extern int TraceAnalyze;

/* TraceCode = TRUE causes comments to be written
 * to the TM code file as code is generated
 */
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error; 
#endif
