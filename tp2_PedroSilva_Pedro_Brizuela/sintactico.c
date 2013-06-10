

#include "nafe.h"


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
            //printf("t=%s\n",tostring(token_actual));
        }
    }
}


// program = declaration_list | statement_list
void program(void)
{
    if (token_actual == VAR)
        declaration_list();
    else
        statement_list();
}

// declaration_list = VAR id_list ;
void declaration_list(void)
{
    match(VAR);
    id_list();
    match(PUNTO_Y_COMA);
}

// id_list = id { , id }
void id_list(void)
{
    id();
    while (token_actual == COMA) {
        match(COMA);
        id();
        if (PARSE_ERROR)
            return;
    }
}

// id = ID [ [ NUM_ENTERO ] ]
void id(void)
{
    match(ID);
    if (token_actual == OP_COR_AB) {
        match(OP_COR_AB);
        match(NUM_ENTERO);
        match(OP_COR_CE);
    }
}

// statement_list = empty { statement }
void statement_list(void)
{
    while (token_actual == IF ||
       token_actual == FOR ||
       token_actual == PUNTO_Y_COMA ||
       token_actual == WRITE ||
       token_actual == WRITELN ||
       token_actual == ID) {
        statement();
        if (PARSE_ERROR)
            return;
    }
}

// statement = exp_stmt | selection_stmt | iteration_stmt
void statement(void)
{
    if (token_actual == IF)
        selection_stmt();
    else if (token_actual == FOR)
        iteration_stmt();
    else
        exp_stmt();
}

// selection_stmt = IF expression THEN statement_list [ ELSE statement_list] END IF;
void selection_stmt(void)
{
    match(IF);
    expression();
    match(THEN);
    statement_list();
    if (token_actual == ELSE) {
        match(ELSE);
        statement_list();
    }
    match(END);
    match(IF);
    match(PUNTO_Y_COMA);
}

// iteration_stmt = FOR expression TO num_or_id [ STEP num_or_id ] DO statement_list END FOR;
void iteration_stmt(void)
{
    match(FOR);
    expression();
    match(TO);
    num_or_id();
    if (token_actual == STEP) {
        match(STEP);
        num_or_id();
    }
    match(DO);
    statement_list();
    match(END);
    match(FOR);
    match(PUNTO_Y_COMA);
}

// num_or_id = var | num
void num_or_id(void)
{
    if (token_actual == ID)
        var();
    else
        num();
}

// var = ID [ [ expression ] ]
void var(void)
{
    match(ID);
    if (token_actual == OP_COR_AB) {
        match(OP_COR_AB);
        expression();
        match(OP_COR_CE);
    }
}

// num = NUM_REAL | NUM_DECIMAL | NUM_ENTERO
void num(void)
{
    if (token_actual == NUM_REAL)
        match(NUM_REAL);
    else if (token_actual == NUM_DECIMAL)
        match(NUM_DECIMAL);
    else
        match(NUM_ENTERO);
}

// exp_stmt = assign_stmt ; | call ; | ;
void exp_stmt(void)
{
    if (token_actual == PUNTO_Y_COMA)
        match(PUNTO_Y_COMA);
    else if (token_actual == WRITE || token_actual == WRITELN) {
        call();
        match(PUNTO_Y_COMA);
    } else {
        assign_stmt();
        match(PUNTO_Y_COMA);
    }
}

// assign_stmt = var = expresion
void assign_stmt(void)
{
    var();
    match(OP_IGUAL);
    expression();
}

// expression = simple_expression [ relop simple_expression ]
void expression(void)
{
    simple_expression();
    if (token_actual == OP_IGUAL ||
        token_actual == OP_NOIGUAL ||
        token_actual == OP_MAYOR ||
        token_actual == OP_MENOR ||
        token_actual == OP_MAYOR_O_IGUAL ||
        token_actual == OP_MENOR_O_IGUAL) {
            relop();
            simple_expression();
    }    
}

// relop = <= | < | > | >= | = | !=
void relop(void)
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

// simple_expression = term { addop term }
void simple_expression(void)
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

// term = factor { mulop factor }
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

// factor = ( expression ) | var | num
void factor(void)
{
    if (token_actual == OP_PAR_AB) {
        match(OP_PAR_AB);
        expression();
        match(OP_PAR_CE);
    } else if (token_actual == ID)
        var();
    else
        num();
}

// call = WRITE ( expression_w ) | WRITELN ( expression_w )
void call(void)
{
    if (token_actual == WRITE) {
        match(WRITE);
        match(OP_PAR_AB);
        expression_w();
        match(OP_PAR_CE);
    } else {
        match(WRITELN);
        match(OP_PAR_AB);
        expression_w();
        match(OP_PAR_CE);
    }
}

// expression_w = expression | CADENA
void expression_w(void)
{
    if (token_actual == CADENA)
        match(CADENA);
    else
        expression();
}
