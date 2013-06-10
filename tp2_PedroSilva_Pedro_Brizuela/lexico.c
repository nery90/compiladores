

#include "nafe.h"


int iniciar_analizador_lexico(void)
{
    FDA=0;
    lex_buffer[0]='\0';
    num_linea=1;
}


void iniciar_lex_table(ANLEXTABLE *table)
{
    int x,y;
    for  (x=0;x<MAX_ESTADOS;++x) {
        table->Aceptar[x]=0;
        for (y=0;y<MAX_CARACTERES;++y) {
            table->T[x][y]=ERROR;
            table->Avanzar[x][y]=1;
            table->Caracteres[y]=0;
        }
    }
    table->TodoAscii=0;     // especial para las cadenas
}


void crear_lex_tables(void)
{
    /**** COMENTARIO TIPO 1 ****/
        iniciar_lex_table(&lex_table[COMENTARIO_TIPO1]);
    
        lex_table[COMENTARIO_TIPO1].T[1]['/']=2;
        
        lex_table[COMENTARIO_TIPO1].T[2]['*']=3;
        
        lex_table[COMENTARIO_TIPO1].T[3]['/']=3;
        lex_table[COMENTARIO_TIPO1].T[3]['*']=4;
        lex_table[COMENTARIO_TIPO1].T[3][OTRO]=3;
     
        lex_table[COMENTARIO_TIPO1].T[4]['/']=5;
        lex_table[COMENTARIO_TIPO1].T[4]['*']=4;
        lex_table[COMENTARIO_TIPO1].T[4][OTRO]=3;
        
        lex_table[COMENTARIO_TIPO1].Aceptar[5]=1;
        
        sprintf(lex_table[COMENTARIO_TIPO1].Caracteres,"/*");       // los caracteres que no forman parte del "otro"

    /**** COMENTARIO TIPO 2 ****/
        iniciar_lex_table(&lex_table[COMENTARIO_TIPO2]);
    
        lex_table[COMENTARIO_TIPO2].T[1]['/']=2;
        
        lex_table[COMENTARIO_TIPO2].T[2]['/']=3;
        
        lex_table[COMENTARIO_TIPO2].T[3][OTRO]=3;
        lex_table[COMENTARIO_TIPO2].T[3]['\n']=4;
            
        lex_table[COMENTARIO_TIPO2].Aceptar[4]=1;
        
        lex_table[COMENTARIO_TIPO2].Caracteres[0]='\n';

    /**** WRITE ****/
        iniciar_lex_table(&lex_table[WRITE]);

        lex_table[WRITE].T[1]['W']=2;        
        lex_table[WRITE].T[2]['R']=3;
        lex_table[WRITE].T[3]['I']=4;
        lex_table[WRITE].T[4]['T']=5;
        lex_table[WRITE].T[5]['E']=6;

        lex_table[WRITE].T[6][OTRO]=7;
            lex_table[WRITE].Avanzar[6][OTRO]=0;
    
        lex_table[WRITE].Aceptar[7]=1;
    
        sprintf(lex_table[WRITE].Caracteres,"WRITE");

    /**** WRITELN ****/
        iniciar_lex_table(&lex_table[WRITELN]);

        lex_table[WRITELN].T[1]['W']=2;        
        lex_table[WRITELN].T[2]['R']=3;
        lex_table[WRITELN].T[3]['I']=4;
        lex_table[WRITELN].T[4]['T']=5;
        lex_table[WRITELN].T[5]['E']=6;
        lex_table[WRITELN].T[6]['L']=7;
        lex_table[WRITELN].T[7]['N']=8;
        
        lex_table[WRITELN].T[8][OTRO]=9;
            lex_table[WRITELN].Avanzar[8][OTRO]=0;
    
        lex_table[WRITELN].Aceptar[9]=1;
    
        sprintf(lex_table[WRITELN].Caracteres,"WRITELN");

    /**** IF ****/
        iniciar_lex_table(&lex_table[IF]);

        lex_table[IF].T[1]['I']=2;
        lex_table[IF].T[2]['F']=3;

        lex_table[IF].T[3][OTRO]=4;
            lex_table[IF].Avanzar[3][OTRO]=0;
    
        lex_table[IF].Aceptar[4]=1;
    
        sprintf(lex_table[IF].Caracteres,"IF");
        
    /**** DO ****/
        iniciar_lex_table(&lex_table[DO]);

        lex_table[DO].T[1]['D']=2;
        lex_table[DO].T[2]['O']=3;

        lex_table[DO].T[3][OTRO]=4;
            lex_table[DO].Avanzar[3][OTRO]=0;
    
        lex_table[DO].Aceptar[4]=1;
    
        sprintf(lex_table[DO].Caracteres,"DO");
        
     /**** TO ****/
        iniciar_lex_table(&lex_table[TO]);

        lex_table[TO].T[1]['T']=2;
        lex_table[TO].T[2]['O']=3;

        lex_table[TO].T[3][OTRO]=4;
            lex_table[TO].Avanzar[3][OTRO]=0;
    
        lex_table[TO].Aceptar[4]=1;
    
        sprintf(lex_table[TO].Caracteres,"TO");   
           
    /**** THEN ****/
        iniciar_lex_table(&lex_table[THEN]);

        lex_table[THEN].T[1]['T']=2;        
        lex_table[THEN].T[2]['H']=3;
        lex_table[THEN].T[3]['E']=4;
        lex_table[THEN].T[4]['N']=5;

        lex_table[THEN].T[5][OTRO]=6;
            lex_table[THEN].Avanzar[5][OTRO]=0;
    
        lex_table[THEN].Aceptar[6]=1;
    
        sprintf(lex_table[THEN].Caracteres,"THEN");
        
    /**** ELSE ****/
        iniciar_lex_table(&lex_table[ELSE]);

        lex_table[ELSE].T[1]['E']=2;        
        lex_table[ELSE].T[2]['L']=3;
        lex_table[ELSE].T[3]['S']=4;
        lex_table[ELSE].T[4]['E']=5;

        lex_table[ELSE].T[5][OTRO]=6;
            lex_table[ELSE].Avanzar[5][OTRO]=0;
    
        lex_table[ELSE].Aceptar[6]=1;
    
        sprintf(lex_table[ELSE].Caracteres,"ELS");
        
    /**** STEP ****/
        iniciar_lex_table(&lex_table[STEP]);

        lex_table[STEP].T[1]['S']=2;        
        lex_table[STEP].T[2]['T']=3;
        lex_table[STEP].T[3]['E']=4;
        lex_table[STEP].T[4]['P']=5;

        lex_table[STEP].T[5][OTRO]=6;
            lex_table[STEP].Avanzar[5][OTRO]=0;
    
        lex_table[STEP].Aceptar[6]=1;
    
        sprintf(lex_table[STEP].Caracteres,"STEP");
        
    /**** END ****/
        iniciar_lex_table(&lex_table[END]);

        lex_table[END].T[1]['E']=2;        
        lex_table[END].T[2]['N']=3;
        lex_table[END].T[3]['D']=4;

        lex_table[END].T[4][OTRO]=5;
            lex_table[END].Avanzar[4][OTRO]=0;
    
        lex_table[END].Aceptar[5]=1;
    
        sprintf(lex_table[END].Caracteres,"END");

    /**** VAR ****/
        iniciar_lex_table(&lex_table[VAR]);

        lex_table[VAR].T[1]['V']=2;        
        lex_table[VAR].T[2]['A']=3;
        lex_table[VAR].T[3]['R']=4;

        lex_table[VAR].T[4][OTRO]=5;
            lex_table[VAR].Avanzar[4][OTRO]=0;
    
        lex_table[VAR].Aceptar[5]=1;
    
        sprintf(lex_table[VAR].Caracteres,"VAR");
        
    /**** FOR ****/
        iniciar_lex_table(&lex_table[FOR]);

        lex_table[FOR].T[1]['F']=2;        
        lex_table[FOR].T[2]['O']=3;
        lex_table[FOR].T[3]['R']=4;

        lex_table[FOR].T[4][OTRO]=5;
            lex_table[FOR].Avanzar[4][OTRO]=0;
    
        lex_table[FOR].Aceptar[5]=1;
    
        sprintf(lex_table[FOR].Caracteres,"FOR");

    /**** IDENTIFICADOR ****/
        iniciar_lex_table(&lex_table[ID]);

        lex_table[ID].T[1][LETRA]=2;
        
        lex_table[ID].T[2][LETRA]=2;
        lex_table[ID].T[2][DIGITO]=2;
        
        lex_table[ID].T[2][OTRO]=3;
            lex_table[ID].Avanzar[2][OTRO]=0;
    
        lex_table[ID].Aceptar[3]=1;
    
        sprintf(lex_table[ID].Caracteres,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789");
        
    /**** NUM_REAL ****/
        iniciar_lex_table(&lex_table[NUM_REAL]);
        
//        lex_table[NUM_REAL].T[1]['+']=2;          // los signos para numeros se verifican en la BNF
//        lex_table[NUM_REAL].T[1]['-']=2;
        lex_table[NUM_REAL].T[1][DIGITO]=3;
        
        lex_table[NUM_REAL].T[2][DIGITO]=3;

        lex_table[NUM_REAL].T[3][DIGITO]=3;
        lex_table[NUM_REAL].T[3]['E']=6;
        lex_table[NUM_REAL].T[3]['e']=6;
        lex_table[NUM_REAL].T[3]['.']=4;        

        lex_table[NUM_REAL].T[4][DIGITO]=5;

        lex_table[NUM_REAL].T[5]['E']=6;
        lex_table[NUM_REAL].T[5]['e']=6;
        lex_table[NUM_REAL].T[5][DIGITO]=5;

        lex_table[NUM_REAL].T[6]['+']=7;
        lex_table[NUM_REAL].T[6]['-']=7;
        lex_table[NUM_REAL].T[6][DIGITO]=8;
        
        lex_table[NUM_REAL].T[7][DIGITO]=8;

        lex_table[NUM_REAL].T[8][DIGITO]=8;
        
        lex_table[NUM_REAL].T[8][OTRO]=9;
            lex_table[NUM_REAL].Avanzar[8][OTRO]=0;
    
        lex_table[NUM_REAL].Aceptar[9]=1;
    
        sprintf(lex_table[NUM_REAL].Caracteres,"0123456789eE.");     // despues de un numero puede venir + o -, por lo que forman parte del "otro"

    /**** NUM_DECIMAL ****/
        iniciar_lex_table(&lex_table[NUM_DECIMAL]);

//        lex_table[NUM_DECIMAL].T[1]['+']=2;
//        lex_table[NUM_DECIMAL].T[1]['-']=2;
        lex_table[NUM_DECIMAL].T[1][DIGITO]=3;
        
        lex_table[NUM_DECIMAL].T[2][DIGITO]=3;

        lex_table[NUM_DECIMAL].T[3][DIGITO]=3;
        lex_table[NUM_DECIMAL].T[3]['.']=4;        

        lex_table[NUM_DECIMAL].T[4][DIGITO]=5;

        lex_table[NUM_DECIMAL].T[5][DIGITO]=5;
        
        lex_table[NUM_DECIMAL].T[5][OTRO]=6;
            lex_table[NUM_DECIMAL].Avanzar[5][OTRO]=0;
    
        lex_table[NUM_DECIMAL].Aceptar[6]=1;
    
        sprintf(lex_table[NUM_DECIMAL].Caracteres,"0123456789.");

    /**** NUM_ENTERO ****/
        iniciar_lex_table(&lex_table[NUM_ENTERO]);

//        lex_table[NUM_ENTERO].T[1]['+']=2;
//        lex_table[NUM_ENTERO].T[1]['-']=2;
        lex_table[NUM_ENTERO].T[1][DIGITO]=3;
        
        lex_table[NUM_ENTERO].T[2][DIGITO]=3;

        lex_table[NUM_ENTERO].T[3][DIGITO]=3;
        
        lex_table[NUM_ENTERO].T[3][OTRO]=4;  
            lex_table[NUM_ENTERO].Avanzar[3][OTRO]=0;
    
        lex_table[NUM_ENTERO].Aceptar[4]=1;
    
        sprintf(lex_table[NUM_ENTERO].Caracteres,"0123456789");

    /**** CADENA ****/
        iniciar_lex_table(&lex_table[CADENA]);

        lex_table[CADENA].T[1]['"']=2;
        
        lex_table[CADENA].T[2]['"']=3;
        lex_table[CADENA].T[2][OTRO]=2;
        
        lex_table[CADENA].T[3][OTRO]=4;  
            lex_table[CADENA].Avanzar[3][OTRO]=0;
    
        lex_table[CADENA].Aceptar[4]=1;
    
        lex_table[CADENA].TodoAscii=1;      // especial para las cadenas

    /**** OP_SUMA ****/
        iniciar_lex_table(&lex_table[OP_SUMA]);

        lex_table[OP_SUMA].T[1]['+']=2;
        lex_table[OP_SUMA].Aceptar[2]=1;

    /**** OP_RESTA ****/
        iniciar_lex_table(&lex_table[OP_RESTA]);

        lex_table[OP_RESTA].T[1]['-']=2;
        lex_table[OP_RESTA].Aceptar[2]=1;
        
    /**** OP_MULT ****/
        iniciar_lex_table(&lex_table[OP_MULT]);

        lex_table[OP_MULT].T[1]['*']=2;
        lex_table[OP_MULT].Aceptar[2]=1;

    /**** OP_DIV ****/
        iniciar_lex_table(&lex_table[OP_DIV]);

        lex_table[OP_DIV].T[1]['/']=2;
        lex_table[OP_DIV].Aceptar[2]=1;

    /**** OP_PAR_AB ****/
        iniciar_lex_table(&lex_table[OP_PAR_AB]);

        lex_table[OP_PAR_AB].T[1]['(']=2;
        lex_table[OP_PAR_AB].Aceptar[2]=1;

    /**** OP_PAR_CE ****/
        iniciar_lex_table(&lex_table[OP_PAR_CE]);

        lex_table[OP_PAR_CE].T[1][')']=2;
        lex_table[OP_PAR_CE].Aceptar[2]=1;
        
    /**** OP_COR_AB ****/
        iniciar_lex_table(&lex_table[OP_COR_AB]);

        lex_table[OP_COR_AB].T[1]['[']=2;
        lex_table[OP_COR_AB].Aceptar[2]=1;

    /**** OP_COR_CE ****/
        iniciar_lex_table(&lex_table[OP_COR_CE]);

        lex_table[OP_COR_CE].T[1][']']=2;
        lex_table[OP_COR_CE].Aceptar[2]=1;

    /**** OP_IGUAL ****/
        iniciar_lex_table(&lex_table[OP_IGUAL]);

        lex_table[OP_IGUAL].T[1]['=']=2;
        lex_table[OP_IGUAL].Aceptar[2]=1;

    /**** OP_MAYOR ****/
        iniciar_lex_table(&lex_table[OP_MAYOR]);

        lex_table[OP_MAYOR].T[1]['>']=2;
        lex_table[OP_MAYOR].Aceptar[2]=1;
        
    /**** OP_MENOR ****/
        iniciar_lex_table(&lex_table[OP_MENOR]);

        lex_table[OP_MENOR].T[1]['<']=2;
        lex_table[OP_MENOR].Aceptar[2]=1;

    /**** PUNTO_Y_COMA ****/
        iniciar_lex_table(&lex_table[PUNTO_Y_COMA]);

        lex_table[PUNTO_Y_COMA].T[1][';']=2;
        lex_table[PUNTO_Y_COMA].Aceptar[2]=1;

    /**** COMA ****/
        iniciar_lex_table(&lex_table[COMA]);

        lex_table[COMA].T[1][',']=2;
        lex_table[COMA].Aceptar[2]=1;

    /**** OP_NOIGUAL ****/
        iniciar_lex_table(&lex_table[OP_NOIGUAL]);

        lex_table[OP_NOIGUAL].T[1]['!']=2;
        lex_table[OP_NOIGUAL].T[2]['=']=3;
        lex_table[OP_NOIGUAL].Aceptar[3]=1;

    /**** OP_MAYOR_O_IGUAL ****/
        iniciar_lex_table(&lex_table[OP_MAYOR_O_IGUAL]);

        lex_table[OP_MAYOR_O_IGUAL].T[1]['>']=2;
        lex_table[OP_MAYOR_O_IGUAL].T[2]['=']=3;
        lex_table[OP_MAYOR_O_IGUAL].Aceptar[3]=1;

    /**** OP_MENOR_O_IGUAL ****/
        iniciar_lex_table(&lex_table[OP_MENOR_O_IGUAL]);

        lex_table[OP_MENOR_O_IGUAL].T[1]['<']=2;
        lex_table[OP_MENOR_O_IGUAL].T[2]['=']=3;
        lex_table[OP_MENOR_O_IGUAL].Aceptar[3]=1;
}


int otro_ch(char ch, char c[])

{
    int i;
    for (i=0;i<MAX_CARACTERES;++i) {
        if (ch==c[i])
            return 0;
        if (!c[i])
            return 1;
    }
    return 1;
}


char es_otro_digito_letra(ANLEXTABLE table, int estado, char ch, char *ch_a)
{
    if (ch!=OTRO && ch!=DIGITO && ch!=LETRA)
        *ch_a=ch;
    
    if (table.T[estado][OTRO] && table.TodoAscii && ch!='"')
        return OTRO;
    if (table.T[estado][OTRO] && table.TodoAscii && ch=='"')
        return '"';
    
    if (table.T[estado][OTRO] && otro_ch(ch,table.Caracteres))
        return OTRO;
    if (table.T[estado][LETRA] && ((ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || ch=='_'))
        return LETRA;
    if (table.T[estado][DIGITO] && ch>='0' && ch<='9')
        return DIGITO;

    return ch;
}


int _getToken(ANLEXTABLE table)
{
    int estado=1,nuevoestado;
    static char ch,ch_a;
        
    ch=fgetc(archivo_fuente);
    
    int num_linea_anterior=num_linea;
    
    if (ch==EOF) {
        FDA=1;
        return 0;
    } else {
        while (ch==' ' || ch=='\n' || ch=='\t') {
            ch=fgetc(archivo_fuente);
            if (ch == '\n')
                num_linea++;
        }
    }

    int cbuf=0;
    lex_buffer[0]=ch;

    ch=es_otro_digito_letra(table,estado,ch,&ch_a);

    int nl=0;
    
    while (!table.Aceptar[estado] && table.T[estado][ch]) {
        
        nuevoestado = table.T[estado][ch];
        
        if (table.Avanzar[estado][ch]) {
            ch = fgetc(archivo_fuente);
            if (ch=='\n')
                nl++;
            if (ch==EOF) {
                FDA=1;
                return 0;
            }
            lex_buffer[++cbuf]=ch;
        }
        
        estado = nuevoestado;

        ch=es_otro_digito_letra(table,estado,ch,&ch_a);
    }
    
    if (!cbuf)
        cbuf=1;
        
    lex_buffer[cbuf]='\0';
    
    if (ch==OTRO || ch==LETRA || ch==DIGITO)
        ch=ch_a;
        
    if (table.Aceptar[estado]) {
        num_linea+=nl;
        fseek(archivo_fuente,ftell(archivo_fuente)-1,SEEK_SET);
        return 1;
    }
    num_linea=num_linea_anterior;
    return 0;
}


int getToken(void)
{
    int i,posicion_anterior;
reiniciar:
    for (i=0;i<MAX_LEX_TABLE;++i) {
        posicion_anterior=ftell(archivo_fuente);
        if (_getToken(lex_table[i]))
            if (i != COMENTARIO_TIPO1 && i != COMENTARIO_TIPO2)
                return i;
            else
                goto reiniciar;
        if (i<MAX_LEX_TABLE-1)
            fseek(archivo_fuente,posicion_anterior,SEEK_SET);
    }
    return -1;
}
