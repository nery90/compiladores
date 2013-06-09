/****************************************************/
/* File: scan.h                                     */
/* Interface del scanner                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SCAN_H_
#define _SCAN_H_

/* MAXTOKENLEN es el tamaño máximo de un token */
#define MAXTOKENLEN 40

/* tokenString array guarda el lexema de cada token */
extern char tokenString[MAXTOKENLEN+1];

/* la función getToken retorna el
 * siguiente token en el archivo fuente
 */
TokenType getToken(void);

#endif
