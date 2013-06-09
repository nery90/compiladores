/****************************************************/
/* File: symtab.h                                   */
/* Interface de la tabla de símbolos                */
/* (sólo permite una tabla de símbolos)             */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

/* Procedimiento st_insert inserta numeros de línea y
 * localizaciones de memoria en la tabla de símbolos
 * loc = localización de memoria es insertada sólo la
 * primera vez, después es ignorada
 */
void st_insert( char * name, int lineno, int loc );

/* Funcion st_lookup retorna la localización 
 * de memoria de una variable o -1 si no se encontró
 */
int st_lookup ( char * name );

/* Procedimiento printSymTab imprime una lista 
 * formateada del contenido de la tabla de símbolos 
 * al archivo listado
 */
void printSymTab(FILE * listing);

#endif
