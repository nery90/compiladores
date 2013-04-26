/*
 *  Estructura Pila
 *	Curso: Compiladores y Lenguajes de Bajo de Nivel
 *	Practica de Programacion Nro. 1
 *
 *	Descripcion:
 *	Implementa una estructura LIFO
 *
 */

#define MAXIMO 30
#define MAX  20

struct LIFO {
	int t;
	char a[MAXIMO];
};


char tope (struct LIFO p)
{
	return ( p.a [p.t -1] );
}

void init_pila (struct LIFO *p)
{
	p->t = 0;
}

int pila_vacia (struct LIFO *p)
{
	return (!p->t);
}

void ins_pila (struct LIFO *p,char s)
{
	if (p->t == MAXIMO)
		printf ("la pila no soporta mas elementos\n");
	else  {
		  p->t++;
		  p->a [p->t - 1] = s;
	}
}

void retira_pila (struct LIFO *p,char *s)
{
	if (pila_vacia (p) )  {
		printf ("la pila esta vacia\n");
		*s = '#';
	}
	else  {
		  *s = p->a [p->t - 1];
		  p->t--;
	}
}
