

#include "libgral.h"


int main(int argc, char *argv[])
{

    iniciar_analizador_lexico();
    iniciar_analizador_sintactico();

    if (argc==1) {
        printf("No hay parametros..");
        return -1;
    }

    archivo_fuente=fopen(argv[1],"r");

    if (!archivo_fuente) {
        printf("Error al abrir %s",argv[1]);
        return -1;
    }
    
    crear_lex_tables();
    
    token_actual=getToken();
    while (!FDA && !PARSE_ERROR)
        program();
  
    if (FDA)
        printf("Sintaxis Correcta\n");
  system("PAUSE");
    return 0;
}
