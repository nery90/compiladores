/****************************************************/
/* File: scan.c                                     */
/* La implementación del scanner                    */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scan.h"

/* Estados en el scanner DFA */
typedef enum
   { START,INASSIGN,INCOMMENT,INNUM,INID,DONE }
   StateType;

/* lexema o identificador o palabra reservada */
char tokenString[MAXTOKENLEN+1];

/* BUFLEN = longitud del buffer para las lineas del codigo fuente */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* Guarda la linea actual */
static int linepos = 0; /* Posición actual en LineBuf */
static int bufsize = 0; /* Tamaño actual del buffer string */
static int EOF_flag = FALSE; /* Soluciona el problema con EOF del ungetNextChar */

/* getNextChar agarra el siguiente caracter que no sea "blank"
   de lineBuf, leyendo en una nueva linea si lineBuf está en el final
*/
static int getNextChar(void)
{ if (!(linepos < bufsize))
  { lineno++;
    if (fgets(lineBuf,BUFLEN-1,source))
    { if (EchoSource) fprintf(listing,"%4d: %s",lineno,lineBuf);
      bufsize = strlen(lineBuf);
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
    { EOF_flag = TRUE;
      return EOF;
    }
  }
  else return lineBuf[linepos++];
}

/* ungetNextChar devuelve un caracter en lineBuf */
static void ungetNextChar(void)
{ if (!EOF_flag) linepos-- ;}

/* Busqueda en tabla de palabras reservadas */
static struct
    { char* str;
      TokenType tok;
    } reservedWords[MAXRESERVED]
   = {{"for",FOR},{"to",TO},{"step",STEP},{"do",DO},{"endfor",ENDFOR},
	  {"if",IF},{"then",THEN},{"else",ELSE},{"end",END},
      {"repeat",REPEAT},{"until",UNTIL},{"read",READ},
      {"write",WRITE},{"writeln",WRITELN}};

/* busca un identificador para ver si es una palabra reservada */
/* usa una busqueda lineal */
static TokenType reservedLookup (char * s)
{ int i;
  for (i=0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))
      return reservedWords[i].tok;
  return ID;
}

/****************************************/
/*** La función primaria del scanner  ***/
/****************************************/
/* La función getToken el siguiente token
 * en el archivo fuente
 */
TokenType getToken(void)
{  /* indice para guardar en tokenString */
   int tokenStringIndex = 0;
   /* guarda el token actual para ser retornado */
   TokenType currentToken;
   /* estado actual - siempre comienza con START */
   StateType state = START;
   /* flag para indicar el guardado en tokenString */
   int save;
   /* flags para indicar el estado de un NUM */ 
   int real = 0;
   int negativo = 0;
   int cientifico = 0;

   while (state != DONE)
   { int c = getNextChar();
     save = TRUE;
     switch (state)
     { case START:
         if (isdigit(c) || (c == '-'))
           state = INNUM;
         else if (isalpha(c))
           state = INID;
         else if (c == '=')
           state = INASSIGN;
         else if ((c == ' ') || (c == '\t') || (c == '\n'))
           save = FALSE;
         else if (c == '/')
         { 
			c = getNextChar();
			if (c == '/')
			{
				save = FALSE;
				state = INCOMMENT;
			}
			else
			{
				ungetNextChar();
				state = START;
			}
         }
         else
         { state = DONE;
           switch (c)
           { case EOF:
               save = FALSE;
               currentToken = ENDFILE;
               break;
             case '<':
               currentToken = LT;
               break;
             case '+':
               currentToken = PLUS;
               break;
             case '-':
               currentToken = MINUS;
               break;
             case '*':
               currentToken = TIMES;
               break;
             case '/':
               currentToken = OVER;
               break;
             case '(':
               currentToken = LPAREN;
               break;
             case ')':
               currentToken = RPAREN;
               break;
             case ';':
               currentToken = SEMI;
               break;
             default:
               currentToken = ERROR;
               break;
           }
         }
         break;
       case INCOMMENT:
         save = FALSE;
         if (c == EOF)
         { state = DONE;
           currentToken = ENDFILE;
         }
         else if (c == '\n') state = START;
         break;
       case INASSIGN:
         state = DONE;
         if (c == '=')
               currentToken = EQ;
         else
         { 
           ungetNextChar();
           currentToken =ASSIGN;
        }
         break;
       case INNUM:
         if (!isdigit(c))
         { 
			if(c=='-' && negativo == 0){
				negativo = 1;
			}else if (c=='.' && real == 0){
				real = 1;
			}else if (c=='e' && cientifico == 0){
				cientifico = 1;
				c = getNextChar();
				real = 0;
				if (c=='-'){
					negativo = 0;
				}else{
					ungetNextChar();
				}	
			}else{
					/* backup en el input */
					ungetNextChar();
					save = FALSE;
					state = DONE;
					currentToken = NUM;
			}
         }
         break;
       case INID:
         if (!isalpha(c))
         { /* backup en el input */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = ID;
         }
         break;
       case DONE:
       default: /* nunca deberia suceder */
         fprintf(listing,"Scanner Bug: state= %d\n",state);
         state = DONE;
         currentToken = ERROR;
         break;
     }
     if ((save) && (tokenStringIndex <= MAXTOKENLEN))
       tokenString[tokenStringIndex++] = (char) c;
     if (state == DONE)
     { tokenString[tokenStringIndex] = '\0';
       if (currentToken == ID)
         currentToken = reservedLookup(tokenString);
     }
   }
   if (TraceScan) {
     fprintf(listing,"\t%d: ",lineno);
     printToken(currentToken,tokenString);
   }
   return currentToken;
} /* fin del getToken */

