/*
 *	Analizador Lexico
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Practica de Programacion Nro. 1
 *
 *	Descripcion:
 *	Implementa un analizador lexico que reconoce numeros, comentarios y
 *  operadores.
 *
 */

/*********** LIbrerias utilizadas **************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

/***************** MACROS **********************/

//Codigos
#define NUM			256
#define OPREL		257
#define OPSUMA		258
#define OPMULT		259
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
	int compLex;
	entrada *pe;
} token;

/************* Variables globales **************/

int consumir;			/* 1 indica al analizador lexico que debe devolver
						el sgte componente lexico, 0 debe devolver el actual */

char cad[5*TAMLEX];		// string utilizado para cargar mensajes de error
token t;				// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico

FILE *archivo;			// Fuente con expresiones aritmeticas simples
char buff[2*TAMBUFF];	// Buffer para lectura de archivo fuente
char id[TAMLEX];		// Utilizado por el analizador lexico
int delantero=-1;		// Utilizado por el analizador lexico
int fin=0;				// Utilizado por el analizador lexico
int numLinea=1;			// Numero de Linea

/************** Prototipos *********************/


void sigLex();		// Del analizador Lexico
int stricmp(const char *cad1, const char *cad2);  // No existe en Ansi C

/**************** Funciones **********************/

int stricmp(const char *cad1, const char *cad2)
{
	int i;
	int tam1=strlen(cad1);
	int tam2=strlen(cad2);
	char s1[tam1];
	char s2[tam2];
	for(i = 0; i<tam1; i++)
	{
		s1[i] = tolower(cad1[i]);
	}
	for(i = 0; i<tam2; i++)
	{
		s2[i] = tolower(cad2[i]);
	}
	return strcmp(s1,s2);
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

//en caso de que la tabla llegue al limite, duplicar el tamanho
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
	insertTablaSimbolos("(",'(');
	insertTablaSimbolos(")",')');
	insertTablaSimbolos("=",OPREL);
	insertTablaSimbolos("+",OPSUMA);
	insertTablaSimbolos("-",OPSUMA);;
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
	int i=0;
	char c=0;
	int acepto=0;
	int estado=0;
	char msg[41];
	entrada e;

	while((c=fgetc(archivo))!=EOF)
	{

		if (c==' ' || c=='\t')
			continue;	//eliminar espacios en blanco
		else if(c=='\n')
		{
			//incrementar el numero de linea
			numLinea++;
			continue;
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
						else if(c=='.')
						{
							i--;
							fseek(archivo,-1,SEEK_CUR);
							estado=6;
						}
						else{
							sprintf(msg,"No se esperaba '%c'",c);
							estado=-1;
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
						if (c==EOF)
							error("No se esperaba el fin de archivo");
						else
							error(msg);
						exit(1);
					}
				}
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
			if ((c=fgetc(archivo))=='/')
			{// Es un comentario
				while(c!='\n')
				{
					c=fgetc(archivo);
				}
				fseek(archivo,-1,SEEK_CUR);
			}
			else
			{
				fseek(archivo,-1,SEEK_CUR);
				t.compLex=OPMULT;
				t.pe=buscar("/");
				break;
			}
		}
		else if (c=='=')
		{
			t.compLex=OPREL;
			t.pe=buscar("=");
			break;
		}
		else if (c=='(')
		{
			t.compLex='(';
			t.pe=buscar("(");
			break;
		}
		else if (c==')')
		{
			t.compLex=')';
			t.pe=buscar(")");
			break;
		}
		else if (c!=EOF)
		{
			sprintf(msg,"%c no esperado",c);
			error(msg);
		}
	}
	if (c==EOF)
	{
		t.compLex=EOF;
		sprintf(e.lexema,"EOF");
		t.pe=&e;
	}

}

