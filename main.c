#include <stdio.h>
#include <stdlib.h>
#include "libs/calculadora.h"
#include "libs/Archivos.h"
#include "libs/calculadora_bmp.h"
int main()
{
    ecuacion ecuaciones[10]; 
    size_t cecu=0;
    char op, subop;
    int arch_guardados=0, salir=1;
    contarArchivos(&arch_guardados);
    int indice;

    while(salir){
        op=menu();
        switch(op){
            case 'A':
                if(cecu!=0){
                    limpiarPantalla();
                    subop = pedir_opcion("ABX",
                    "--- INGRESAR ECUACION ---\n"
                    "[A] - Agregar ecuacion nueva.\n"
                    "[B] - Cambiar ecuacion existente.\n"
                    "[X] - Volver.\n"
                    "Ingrese una opcion: "
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
                mostrarecuaciones(ecuaciones, cecu,1);
                EnterParaSalir();
                break;
            case 'C':
                if(arch_guardados<10){
                    if(!GuardaArchivos(ecuaciones, cecu, &arch_guardados)){
                        borrarecuacion(ecuaciones,&cecu);
                    }
                }
                else
                    puts("No podes guardar mas ecuaciones. Utilice la opcion [E] para borrar las ecuaciones guardadas");

                EnterParaSalir();
                break;
            case 'D':
                CargarArchivo(ecuaciones,&cecu);
                break;
            case 'E':
                borrarArchivos();
                arch_guardados=0;
                EnterParaSalir();
                break;
            case 'F':
                if(cecu != 0){
                    limpiarPantalla();
                    printf("--- EVALUAR ECUACION ---\n\n");
                    subop = pedir_opcion("ABCX",
                        "[A] - Ingresar una tabla de valores.\n"
                        "[B] - Ingresar un solo valor para una incognita.\n"
                        "[C] - Buscar raices.\n"
                        "[X] - Volver.\n"
                        "Ingrese una opcion: "
                    );
                    if(subop=='A'){
                        EvaluarXeY(ecuaciones, cecu);
                    }
                    else if(subop=='B'){
                        TablaAlrededorDeValor(ecuaciones,cecu);
                        EnterParaSalir();
                    }
                    else if(subop=='C'){
                        BuscarRaices(ecuaciones, cecu);
                    }
                }
                else {
                    puts("No hay ecuaciones cargadas para evaluar.");
                }
                break;
            case 'G':
                if(cecu == 0) {
                    puts("No hay ecuaciones cargadas para graficar.");
                    break;
                }
                graficar(ecuaciones,cecu,&indice);
                break;
            case 'H':
                menu_ayuda_interactivo();
                EnterParaSalir();
                break;
            case 'X':
                subop = pedir_opcion("SN",
                    "Seguro quieres salir? S/N: "
                );
                if(subop=='S'){
                    printf("\nSaliendo del programa...\n");
                    salir=0;
                }
                break;
        }
    }

    borrarecuacion(ecuaciones,&cecu);
    return 0;
}
