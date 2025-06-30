#include <stdio.h>
#include <stdlib.h>
#include "libs/calculadora.h"
#include "libs/Archivos.h"
int main()
{
    ecuacion ecuaciones[10]; 
    size_t cecu=0;
    char op, subop;
    int band=0, salir=1;
    contarArchivos(&band);

    while(salir){
        op=menu();
        switch(op){
            case 'A':
                if(cecu!=0){
                    subop = pedir_opcion("ABX",
                    "Ingresar Ecuación:\n"
                    "[A] - Agregar ecuación nueva.\n"
                    "[B] - Cambiar ecuación existente.\n"
                    "[X] - Volver.\n"
                    "Ingrese una opción: "
                    );

                    if(subop=='B'){
                        ingresarecu(ecuaciones, &cecu,2,0);
                        break;
                    }
                    if(subop=='X'){
                        break;
                    }
                }
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
                band=0;
                break;
            case 'F':
                subop = pedir_opcion("ABFX",
                    "Evaluar Ecuación:\n"
                    "[A] - Ingresar una tabla de valores.\n"
                    "[B] - Ingresar un solo valor para una incógnita.\n"
                    "[C] - Buscar raices.\n"
                    "[X] - Volver.\n"
                    "Ingrese una opción: "
                );
                if(subop=='A'){
                    EvaluarXeY(ecuaciones, cecu);
                }
                else if(subop=='B'){
                    GenerarTabla(ecuaciones,cecu);
                }
                else if(subop=='F'){
                    //hacer algo
                }
                break;
            case 'G':
                puts("opcion G");
                break;
            case 'H':
                ayuda();
                break;
            case 'X':
                subop = pedir_opcion("SN",
                    "Seguro quieres salir? S/N: "
                );
                if(subop=='S'){
                    printf("\nSaliendo del programa...");
                    salir=0;
                }
                break;
        }
    }

    borrarecuacion(ecuaciones,&cecu);
    return 0;
}
