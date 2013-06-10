

#include "libgral.h"


char *tostring(int i)
{
    switch (i) {
        case ID: return "IDENTIFICADOR";
        case COMENTARIO_TIPO1: return "COMENTARIO_TIPO1";
        case COMENTARIO_TIPO2: return "COMENTARIO_TIPO2";
        case NUM_REAL: return "NUMERO_REAL";
        case NUM_DECIMAL: return "NUMERO_DECIMAL";
        case NUM_ENTERO: return "NUMERO_ENTERO";
        case CADENA: return "CADENA";
        case OP_SUMA: return "+";
        case OP_RESTA: return "-";
        case OP_MULT: return "*";
        case OP_DIV: return "/";
        case OP_PAR_AB: return "(";
        case OP_PAR_CE: return ")";
        case OP_COR_AB: return "[";
        case OP_COR_CE: return "]";
        case OP_IGUAL: return "=";
        case OP_NOIGUAL: return "!=";
        case OP_MAYOR: return ">";
        case OP_MENOR: return "<";
        case OP_MAYOR_O_IGUAL: return ">=";
        case OP_MENOR_O_IGUAL: return "<=";
        case PUNTO_Y_COMA: return ";";
        case COMA: return ",";
        case IF: return "IF";
        case THEN: return "THEN";
        case ELSE: return "ELSE";
        case END: return "END";
        case VAR: return "VAR";
        case WRITE: return "WRITE";
        case WRITELN: return "WRITELN";
        case FOR: return "FOR";
        case TO: return "TO";
        case STEP: return "STEP";
        case DO: return "DO";
        default: return "EOF";
    }
}


void iniciar_analizador_sintactico(void)
{
    PARSE_ERROR=0;
}


void match(int expected_token)
{
    if (PARSE_ERROR==1||FDA)
        return;
            
    if (token_actual==expected_token) {
        token_actual=getToken();
        if (token_actual == -1) {
            if (fgetc(archivo_fuente)==EOF)
                FDA=1;
            else {
                printf("Error [linea: %d], '%s' no es un terminal valido\n",num_linea,lex_buffer);
                PARSE_ERROR=1;
            }
        }
    } else {
        printf("Error [linea: %d], se esperaba '%s' y se encontro '%s':%s\n",num_linea,tostring(expected_token),tostring(token_actual),lex_buffer);        
        
        /* sincronizar */
        while (token_actual!=PUNTO_Y_COMA) {
            token_actual=getToken();
            if (token_actual == -1) {
                PARSE_ERROR=1;
                if (fgetc(archivo_fuente)==EOF)
                    FDA=1;
                else
                    printf("Error [linea: %d], '%s' no es un terminal valido\n",num_linea,lex_buffer);
                return;
            }

        }
    }
}

//aplicacion de la gramatica

void program(void)
{
    if (token_actual == VAR)
        lista_declaracion();
    else
        lista_sentencias();
}


void lista_declaracion(void)
{
    match(VAR);
    lista_ids();
    match(PUNTO_Y_COMA);
}


void lista_ids(void)
{
    id();
    while (token_actual == COMA) {
        match(COMA);
        id();
        if (PARSE_ERROR)
            return;
    }
}

void id(void)
{
    match(ID);
    if (token_actual == OP_COR_AB) {
        match(OP_COR_AB);
        match(NUM_ENTERO);
        match(OP_COR_CE);
    }
}

void lista_sentencias(void)
{
    while (token_actual == IF ||
       token_actual == FOR ||
       token_actual == PUNTO_Y_COMA ||
       token_actual == WRITE ||
       token_actual == WRITELN ||
       token_actual == ID) {
        sentencia();
        if (PARSE_ERROR)
            return;
    }
}


void sentencia(void)
{
    if (token_actual == IF)
        sentencia_select();
    else if (token_actual == FOR)
        sentencia_iteracion();
    else
        sentencia_exp();
}


void sentencia_select(void)
{
    match(IF);
    exp();
    match(THEN);
    lista_sentencias();
    if (token_actual == ELSE) {
        match(ELSE);
        lista_sentencias();
    }
    match(END);
    match(IF);
    match(PUNTO_Y_COMA);
}


void sentencia_iteracion(void)
{
    match(FOR);
    exp();
    match(TO);
    num_or_id();
    if (token_actual == STEP) {
        match(STEP);
        num_or_id();
    }
    match(DO);
    lista_sentencias();
    match(END);
    match(FOR);
    match(PUNTO_Y_COMA);
}


void num_or_id(void)
{
    if (token_actual == ID)
        var();
    else
        num();
}


void var(void)
{
    match(ID);
    if (token_actual == OP_COR_AB) {
        match(OP_COR_AB);
        exp();
        match(OP_COR_CE);
    }
}


void num(void)
{
    if (token_actual == NUM_REAL)
        match(NUM_REAL);
    else if (token_actual == NUM_DECIMAL)
        match(NUM_DECIMAL);
    else
        match(NUM_ENTERO);
}


void sentencia_exp(void)
{
    if (token_actual == PUNTO_Y_COMA)
        match(PUNTO_Y_COMA);
    else if (token_actual == WRITE || token_actual == WRITELN) {
        llamada_sub();
        match(PUNTO_Y_COMA);
    } else {
        asignacion();
        match(PUNTO_Y_COMA);
    }
}


void asignacion(void)
{
    var();
    match(OP_IGUAL);
    exp();
}


void exp(void)
{
   exp_simple();
    if (token_actual == OP_IGUAL ||
        token_actual == OP_NOIGUAL ||
        token_actual == OP_MAYOR ||
        token_actual == OP_MENOR ||
        token_actual == OP_MAYOR_O_IGUAL ||
        token_actual == OP_MENOR_O_IGUAL) {
            operador_relacion();
            exp_simple();
    }    
}


void operador_relacion(void)
{
    if (token_actual == OP_MENOR_O_IGUAL)
        match(OP_MENOR_O_IGUAL);
    else if (token_actual == OP_MENOR)
        match(OP_MENOR);
    else if (token_actual == OP_MAYOR)
        match(OP_MAYOR);
    else if (token_actual == OP_MAYOR_O_IGUAL)
        match(OP_MAYOR_O_IGUAL);
    else if (token_actual == OP_NOIGUAL)
        match(OP_NOIGUAL);
    else if (token_actual == OP_IGUAL)
        match(OP_IGUAL);
}


void exp_simple(void)
{
    term();
    while (token_actual == OP_SUMA || token_actual == OP_RESTA) {
        if (token_actual == OP_SUMA)
            match(OP_SUMA);
        else
            match(OP_RESTA);
        term();
        if (PARSE_ERROR)
            return;
    }
}


void term(void)
{
    factor();
    while (token_actual == OP_MULT || token_actual == OP_DIV) {
        if (token_actual == OP_MULT)
            match(OP_MULT);
        else
            match(OP_DIV);
        factor();
        if (PARSE_ERROR)
            return;
    }
}


void factor(void)
{
    if (token_actual == OP_PAR_AB) {
        match(OP_PAR_AB);
        exp();
        match(OP_PAR_CE);
    } else if (token_actual == ID)
        var();
    else
        num();
}


void llamada_sub(void)
{
    if (token_actual == WRITE) {
        match(WRITE);
        match(OP_PAR_AB);
        exp_w();
        match(OP_PAR_CE);
    } else {
        match(WRITELN);
        match(OP_PAR_AB);
        exp_w();
        match(OP_PAR_CE);
    }
}


void exp_w(void)
{
    if (token_actual == CADENA)
        match(CADENA);
    else
        exp();
}
