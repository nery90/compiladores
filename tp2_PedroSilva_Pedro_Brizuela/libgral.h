
#ifndef _LIBGRAL
#define _LIBGRAL

#include <stdio.h>

/*
 *================ ANALIZADOR LEXICO
 */

/*** TOKENS ***/

    #define COMENTARIO_TIPO1    0
    #define COMENTARIO_TIPO2    1
    
    #define IF                  2
    #define THEN                3
    #define ELSE                4
    #define END                 5
    #define VAR                 6
    #define WRITELN             7
    #define WRITE               8
    #define FOR                 9
    #define TO                 10
    #define STEP               11
    #define DO                 12
    
    #define ID                 13
    
    #define NUM_REAL           14
    #define NUM_DECIMAL        15
    #define NUM_ENTERO         16
    #define CADENA             17
    
    #define OP_SUMA            18
    #define OP_RESTA           20
    
    #define OP_MULT            21
    #define OP_DIV             22
    
    #define OP_PAR_AB           23
    #define OP_PAR_CE           24
    #define OP_COR_AB           25
    #define OP_COR_CE           26
    
    #define OP_IGUAL            27
    #define OP_NOIGUAL          28
    
    #define OP_MAYOR            29
    #define OP_MENOR            30
    
    #define OP_MAYOR_O_IGUAL    31
    #define OP_MENOR_O_IGUAL    32
    
    #define PUNTO_Y_COMA        33
    #define COMA                34

/*** TABLA DE ANALISIS LEXTCO ***/

    #define MAX_LEX_TABLE       35
    
    #define OTRO                'ª'
    #define LETRA               'º'
    #define DIGITO              '€'
    
    #define ERROR               0

    #define MAX_ESTADOS         10
    #define MAX_CARACTERES      255
    
    typedef struct _an_lex_table {
        
        int T[MAX_ESTADOS][MAX_CARACTERES];
        int Aceptar[MAX_ESTADOS];
        int Avanzar[MAX_ESTADOS][MAX_CARACTERES];
        
        char Caracteres[MAX_CARACTERES];
        int TodoAscii;                      // especial para las cadenas
        
    } ANLEXTABLE;
    
    ANLEXTABLE lex_table[MAX_LEX_TABLE];

    char lex_buffer[255];
    int num_linea;

/*** ARCHIVO FUENTE ***/

    FILE *archivo_fuente;
    int FDA;

/*** PROTOTIPO DE FUNCIONES ***/

    int iniciar_analizador_lexico(void);
    void crear_lex_tables(void);
    int getToken(void);

/*
 *================ ANALIZADOR SINTACTICO
 */

    int token_actual;
    int PARSE_ERROR;

/*** PROTOTIPO DE FUNCIONES ***/

    void exp_w(void);
    void llamada_sub(void);
    void factor(void);
    void term(void);
    void exp_simple(void);
    void operador_relacion(void);
    void exp(void);
    void asignacion(void);
    void sentencia_exp(void);
    void num(void);
    void var(void);
    void num_or_id(void);
    void sentencia_iteracion(void);
    void sentencia_select(void);
    void sentencia(void);
    void lista_sentencias(void);
    void id(void);
    void lista_ids(void);
    void lista_declaracion(void);
    void program(void);

#endif
