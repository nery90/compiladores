/*
 *  Evaluacion Postfija
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Practica de Programacion Nro. 1
 *
 *	Descripcion:
 *	Evalua expresiones postfija
 *
 */

#include <math.h>


struct expre   {
	char op;
	float valor;
};


void evaluar (struct expre posfijo[],float *x)
{
	struct LIFO pila;
	void init_pila (struct LIFO *p);
	int pila_vacia (struct LIFO *p);
	//void ins_pila (struct LIFO *p,float s);
	//void retira_pila (struct LIFO *p,float *s);
	void F(float a,float b,float *r,char op);
	float aux,a,b,r;
	int i = 0;
	char c;

	init_pila (&pila);
	while (posfijo [i].op == 'v'  || posfijo [i].op == 'r') {
		if (posfijo [i].op == 'v') {
			aux = posfijo [i].valor;
			ins_pila (&pila,aux);
		}
		else if (posfijo [i].valor != '#') {
				retira_pila (&pila,&b);
				retira_pila (&pila,&a);
				c = (char)posfijo [i].valor;
				F (a,b,&r,c);
				ins_pila (&pila,r);
		}
		i++;
	}
	*x = pila.a[0];
}

void F(float a,float b,float *r,char op)
{
	switch (op) {
		case '+'  : *r = (a + b); break;
		case '-'  : *r = (a - b); break;
		case '*'  : *r = a * b; break;
		case '/'  : *r = (a / b); break;
		//case '^'  : *r = exp (b * log(a)); break;
		default   : printf ("situacion de error\n"); break;
	}
}
