#include <stdio.h>
#include <stdlib.h>
#include "libs/calculadora.h"
#include "libs/Archivos.h"
int main()
{
    ecuacion ecuaciones[10]; 
    size_t cecu=0;
    char op;
    int band=0;
    contarArchivos(&band);

    while((op=menu())!='X'){
        switch(op){
            case 'A':
                if(ingresarecu(ecuaciones, &cecu,1,0))
                    puts("No se pudo aniadir ecuacion");
                break;
            case 'B':
                mostrarecuaciones(ecuaciones, cecu);
                break;
            case 'C':
                if(band<3){
                    if(!GuardaArchivos(ecuaciones, cecu, &band)){
                        borrarecuacion(ecuaciones,&cecu);
                    }
                }
                else
                    puts("No podes guardar mas ecuaciones. Utilice la opcion [E] para borrar las ecuaciones guardadas");
                break;
            case 'D':
                CargarArchivo(ecuaciones,&cecu);
                break;
            case 'E':
                borrarArchivos();
                break;
            case 'F':
                puts("opcion F");
                break;
            case 'G':
                puts("opcion G");
                break;
            case 'H':
                ayuda();
                break;
        }
    }

    borrarecuacion(ecuaciones,&cecu);
    return 0;
}
