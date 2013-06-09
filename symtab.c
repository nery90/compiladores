/****************************************************/
/* File: symtab.c                                   */
/* Implementación de la tabla de símbolos           */
/* (permite una sola tabla de símbolos)             */
/* La tabla de simbolos está implementada como      */
/* una tabla hash encadenada                        */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* SIZE es el tamaño de la tabla hash */
#define SIZE 211

/* SHIFT es el poder de 2 usado como multiplicador en la función hash */
#define SHIFT 4

/* la función hash */
static int hash ( char * key )
{ int temp = 0;
  int i = 0;
  while (key[i] != '\0')
  { temp = ((temp << SHIFT) + key[i]) % SIZE;
    ++i;
  }
  return temp;
}

/* la lista de numeros de linea en el codigo fuente 
 * en las cuales una variable es referenciada
 */
typedef struct LineListRec
   { int lineno;
     struct LineListRec * next;
   } * LineList;

/* El registro en la lista bucket para
 * cada variable, incluyendo nombre, 
 * localizacion de memoria, y
 * la lista de numeros de linea donde 
 * aparecen en el codigo fuente
 */
typedef struct BucketListRec
   { char * name;
     LineList lines;
     int memloc ; /* localización de memoria por cada variable */
     struct BucketListRec * next;
   } * BucketList;

/* tabla hash */
static BucketList hashTable[SIZE];

/* Procedimiento st_insert inserta numeros de linea y
 * localizaciones de memoria en la tabla de simbolos
 * loc = localización de memoria es insertada sólo la 
 * primera vez, después es ignorada
 */
void st_insert( char * name, int lineno, int loc )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) /* la variable todavía no esta en la tabla */
  { l = (BucketList) malloc(sizeof(struct BucketListRec));
    l->name = name;
    l->lines = (LineList) malloc(sizeof(struct LineListRec));
    l->lines->lineno = lineno;
    l->memloc = loc;
    l->lines->next = NULL;
    l->next = hashTable[h];
    hashTable[h] = l; }
  else /* encontrada en la tabla, asi que sólo se añade número de línea */
  { LineList t = l->lines;
    while (t->next != NULL) t = t->next;
    t->next = (LineList) malloc(sizeof(struct LineListRec));
    t->next->lineno = lineno;
    t->next->next = NULL;
  }
} /* st_insert */

/* Función st_lookup retorna la localización
 * de memoria de una variable o -1 si no se encontró
 */
int st_lookup ( char * name )
{ int h = hash(name);
  BucketList l =  hashTable[h];
  while ((l != NULL) && (strcmp(name,l->name) != 0))
    l = l->next;
  if (l == NULL) return -1;
  else return l->memloc;
}

/* Procedimiento printSymTab imprime una lista 
 * formateada del contenido de la tabla de símbolos 
 * al archivo listado
 */
void printSymTab(FILE * listing)
{ int i;
  fprintf(listing,"Variable Name  Location   Line Numbers\n");
  fprintf(listing,"-------------  --------   ------------\n");
  for (i=0;i<SIZE;++i)
  { if (hashTable[i] != NULL)
    { BucketList l = hashTable[i];
      while (l != NULL)
      { LineList t = l->lines;
        fprintf(listing,"%-14s ",l->name);
        fprintf(listing,"%-8d  ",l->memloc);
        while (t != NULL)
        { fprintf(listing,"%4d ",t->lineno);
          t = t->next;
        }
        fprintf(listing,"\n");
        l = l->next;
      }
    }
  }
} /* printSymTab */
