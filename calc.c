/*
 *  Calculadora de expresiones aritmeticas	
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Practica de Programacion Nro. 1
 *	
 *	Descripcion:
 *	Implementa una calculadora de expresiones aritmeticas simples.
 *	
 */

#include "anlex.h"
#include "pila.h"
#include "infijaToPostfija.h"
#include "evaluacionPostfija.h"

int main(int argc,char* args[])
{
	initTabla();
	initTablaSimbolos();

	int controlFila=1;

	char strposfijo [100],infijo[100];
	void in_a_pos (char *infijo,char *posfijo);
	int i=0;

	struct expre posfijo [MAX];
	void evaluar (struct expre posfijo[], float *x);
	void lea_expresion (struct expre a[]);
	float x = 0;

	if(argc > 1)
	{
		if (!(archivo=fopen(args[1],"rt")))
		{
			printf("Archivo no encontrado.");
			exit(1);
		}
		while (t.compLex!=EOF){
			sigLex();
			if (numLinea==controlFila)
			{
				strcat( infijo, t.pe->lexema );
			}
			else
			{
				infijo [i] = '#';
				infijo [i+1] = '\0';
				in_a_pos (infijo,strposfijo);
				//printf ("%s  %s  ",infijo,strposfijo);
				evaluar (posfijo,&x);
				printf ("resul=   %f\n",x);
				*infijo='\0';
				controlFila++;
				i=0;
			}
		}
		fclose(archivo);
	}else{
		printf("Debe pasar como parametro el path al archivo fuente.");
		exit(1);
	}

	return 0;
}
