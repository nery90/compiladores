/*
*	Analizador Léxico	
*	Curso: Compiladores y Lenguajes de Bajo de Nivel
*	Lexer de Calculadora
*	
*	Descripcion:
*	Implementa un analizador léxico que reconoce números y signos de operación aritmética
	para una calculadora.
*	
*/

/*********** LIbrerias utilizadas **************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/***************** MACROS **********************/

//Codigos
#define NUM			284
#define OPSUMA		291
#define OPMULT		292
// Fin Codigos
#define TAMBUFF 	5
#define TAMLEX 		50
#define TAMHASH 	101

/************* Definiciones ********************/

typedef struct entrada{
    //definir los campos de 1 entrada de la tabla de simbolos
    int compLex;
    char lexema[TAMLEX];	
    struct entrada *tipoDato; // null puede representar variable no declarada	
    // aqui irian mas atributos para funciones y procedimientos...

} entrada;

typedef struct {
    int numLinea;
    int compLex;
    entrada *pe;
} token;

/************* Variables globales **************/

int consumir;			/* 1 indica al analizador lexico que debe devolver
                        el sgte componente lexico, 0 debe devolver el actual */

char cad[5*TAMLEX];		// string utilizado para cargar mensajes de error
token t;				// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico

FILE *archivo;			// Fuente de la calculadora
char buff[2*TAMBUFF];	// Buffer para lectura de archivo fuente
char id[TAMLEX];		// Utilizado por el analizador lexico
int delantero=-1;		// Utilizado por el analizador lexico
int fin=0;				// Utilizado por el analizador lexico
int numLinea=1;			// Numero de Linea

/************** Prototipos *********************/


void sigLex();		// Del analizador Lexico

/**************** Funciones **********************/
int stricmp(const char* cad1,const char* cad2) 
{
    int cmp;
    size_t i;

    char* c1 = strdup(cad1);
    char* c2 = strdup(cad2);

    const size_t s1 = strlen(c1);
    const size_t s2 = strlen(c2);

    for(i = 0; i < s1; ++i)
        c1[i] = toupper(c1[i]);

    for(i = 0; i < s2; ++i)
        c2[i] = toupper(c2[i]);

    cmp = strcmp(c1, c2);

    free(c2);
    free(c1);

    return cmp;
}

/*********************HASH************************/
entrada *tabla;				//declarar la tabla de simbolos
int tamTabla=TAMHASH;		//utilizado para cuando se debe hacer rehash
int elems=0;				//utilizado para cuando se debe hacer rehash

int h(const char* k, int m)
{
    unsigned h=0,g;
    int i;
    for (i=0;i<strlen(k);i++)
    {
        h=(h << 4) + k[i];
        if (g=h&0xf0000000){
            h=h^(g>>24);
            h=h^g;
        }
    }
    return h%m;
}
void insertar(entrada e);

void initTabla()
{	
    int i=0;

    tabla=(entrada*)malloc(tamTabla*sizeof(entrada));
    for(i=0;i<tamTabla;i++)
    {
        tabla[i].compLex=-1;
    }
}

int esprimo(int n)
{
    int i;
    for(i=3;i*i<=n;i+=2)
        if (n%i==0)
            return 0;
    return 1;
}

int siguiente_primo(int n)
{
    if (n%2==0)
        n++;
    for (;!esprimo(n);n+=2);

    return n;
}

//en caso de que la tabla llegue al limite, duplicar el tamaño
void rehash()
{
    entrada *vieja;
    int i;
    vieja=tabla;
    tamTabla=siguiente_primo(2*tamTabla);
    initTabla();
    for (i=0;i<tamTabla/2;i++)
    {
        if(vieja[i].compLex!=-1)
            insertar(vieja[i]);
    }		
    free(vieja);
}

//insertar una entrada en la tabla
void insertar(entrada e)
{
    int pos;
    if (++elems>=tamTabla/2)
        rehash();
    pos=h(e.lexema,tamTabla);
    while (tabla[pos].compLex!=-1)
    {
        pos++;
        if (pos==tamTabla)
            pos=0;
    }
    tabla[pos]=e;

}
//busca una clave en la tabla, si no existe devuelve NULL, posicion en caso contrario
entrada* buscar(const char *clave)
{
    int pos;
    entrada *e;
    pos=h(clave,tamTabla);
    while(tabla[pos].compLex!=-1 && stricmp(tabla[pos].lexema,clave)!=0 )
    {
        pos++;
        if (pos==tamTabla)
            pos=0;
    }
    return &tabla[pos];
}

void insertTablaSimbolos(const char *s, int n)
{
    entrada e;
    sprintf(e.lexema,s);
    e.compLex=n;
    insertar(e);
}

void initTablaSimbolos()
{
    int i;
    entrada pr,*e;
    insertTablaSimbolos("+",OPSUMA);
    insertTablaSimbolos("-",OPSUMA);
    insertTablaSimbolos("*",OPMULT);
    insertTablaSimbolos("/",OPMULT);
}

// Rutinas del analizador lexico

void error(const char* mensaje)
{
    printf("Lin %d: Error Lexico. %s.\n",numLinea,mensaje);	
}

void sigLex()
{
    int i=0, longid=0;
    char c=0;
    int acepto=0;
    int estado=0;
    char msg[41];
    entrada e;

    t.numLinea = numLinea;
    while((c=fgetc(archivo))!=EOF)
    {

        if (c==' ' || c=='\t')
            continue;	//eliminar espacios en blanco
        else if(c=='\n')
        {
            //incrementar el numero de linea
            numLinea++;
            t.compLex='\n';
            strcpy(e.lexema,"EOL");
            t.pe=&e;
            break;
        }
        else if (isdigit(c))
        {
            //es un numero
            i=0;
            estado=0;
            acepto=0;
            id[i]=c;

            while(!acepto)
            {
                switch(estado){
                case 0: //una secuencia netamente de digitos, puede ocurrir . o e
                    c=fgetc(archivo);
                    if (isdigit(c))
                    {
                        id[++i]=c;
                        estado=0;
                    }
                    else if(c=='.'){
                        id[++i]=c;
                        estado=1;
                    }
                    else if(tolower(c)=='e'){
                        id[++i]=c;
                        estado=3;
                    }
                    else{
                        estado=6;
                    }
                    break;

                case 1://un punto, debe seguir un digito (caso especial de array, puede venir otro punto)
                    c=fgetc(archivo);						
                    if (isdigit(c))
                    {
                        id[++i]=c;
                        estado=2;
                    }
                    else{
                        estado=6;
                    }
                    break;
                case 2://la fraccion decimal, pueden seguir los digitos o e
                    c=fgetc(archivo);
                    if (isdigit(c))
                    {
                        id[++i]=c;
                        estado=2;
                    }
                    else if(tolower(c)=='e')
                    {
                        id[++i]=c;
                        estado=3;
                    }
                    else
                        estado=6;
                    break;
                case 3://una e, puede seguir +, - o una secuencia de digitos
                    c=fgetc(archivo);
                    if (c=='+' || c=='-')
                    {
                        id[++i]=c;
                        estado=4;
                    }
                    else if(isdigit(c))
                    {
                        id[++i]=c;
                        estado=5;
                    }
                    else{
                        sprintf(msg,"No se esperaba '%c'",c);
                        estado=-1;
                    }
                    break;
                case 4://necesariamente debe venir por lo menos un digito
                    c=fgetc(archivo);
                    if (isdigit(c))
                    {
                        id[++i]=c;
                        estado=5;
                    }
                    else{
                        sprintf(msg,"No se esperaba '%c'",c);
                        estado=-1;
                    }
                    break;
                case 5://una secuencia de digitos correspondiente al exponente
                    c=fgetc(archivo);
                    if (isdigit(c))
                    {
                        id[++i]=c;
                        estado=5;
                    }
                    else{
                        estado=6;
                    }break;
                case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
                    if (c!=EOF)
                        ungetc(c,archivo);
                    else
                        c=0;
                    id[++i]='\0';
                    acepto=1;
                    t.pe=buscar(id);
                    if (t.pe->compLex==-1)
                    {
                        sprintf(e.lexema,id);
                        e.compLex=NUM;
                        insertar(e);
                        t.pe=buscar(id);
                    }
                    t.compLex=NUM;
                    break;
                case -1:
                    while(c!=EOF)
                    {
                        c=fgetc(archivo);
                        if (c=='\n')
                        {
                            ungetc(c,archivo);
                            break;
                        }
                    }
                    if (c==EOF)
                        error("No se esperaba el fin de archivo");
                    else
                        error(msg);
                    estado=-2;
                }
                if (estado==-2)
                    break;
            }
            if (estado==-2)
                continue;
            break;
        }
        else if (c=='+')
        {
            t.compLex=OPSUMA;
            t.pe=buscar("+");
            break;
        }
        else if (c=='-')
        {
            t.compLex=OPSUMA;
            t.pe=buscar("-");
            break;
        }
        else if (c=='*')
        {
            t.compLex=OPMULT;
            t.pe=buscar("*");
            break;
        }
        else if (c=='/')
        {
            c=fgetc(archivo);
            if (c=='/')
                while(c!=EOF)
                {
                    c=fgetc(archivo);
                    if (c=='\n')
                    {
                        ungetc(c,archivo);
                        break;
                    }
                }
            else if (c!=EOF)
            {
                ungetc(c,archivo);
                t.compLex=OPMULT;
                t.pe=buscar("/");
            }
            else
            {
                t.compLex=OPMULT;
                t.pe=buscar("/");
            }
            if (c=='\n')
                continue;
            break;
        }
        else if (c!=EOF)
        {
            sprintf(msg,"%c no esperado",c);
            error(msg);
            while(c!=EOF)
            {
                c=fgetc(archivo);
                if (c=='\n')
                {
                    ungetc(c,archivo);
                    break;
                }
            }
            if (c==EOF)
                error("No se esperaba el fin de archivo");
        }
    }
    if (c==EOF)
    {
        t.compLex=EOF;
        sprintf(e.lexema,"EOF");
        t.pe=&e;
    }
}

int main(int argc,char* args[])
{
    // inicializar analizador lexico
    int complex=0;

    initTabla();
    initTablaSimbolos();

    if(argc > 1)
    {
        if (!(archivo=fopen(args[1],"rt")))
        {
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        while (t.compLex!=EOF){
            sigLex();
            printf("Lin %d: %s -> %d\n",t.numLinea,t.pe->lexema,t.compLex);
        }
        fclose(archivo);
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}
