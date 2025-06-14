#include <stdio.h>
#include <stdlib.h>
#include "libs/calculadora.h"

int main()
{
    ecuacion ecuaciones[10]; // ecuaciones={{izq,der}{izq,der}{izq,der}{}{}}
    size_t cecu=0;
    char op;
    //calculadora(ecuaciones);


    while((op=menu())!='X'){
        switch(op){
            case 'A':
                if(ingresarecu(ecuaciones, &cecu))
                    puts("No se pudo aniadir ecuacion");
                break;
            case 'B':
                puts("opcion B");
                break;
            case 'C':
                puts("opcion C");
                break;
            case 'D':
                puts("opcion D");
                break;
            case 'E':
                puts("opcion E");
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
    /*
    for(int i=0; i<cecu; i++){
        printf("ecuacion %d: %s\n",i+1, ecuaciones[i].ecu);
    }*/
    destruirecuaciones(ecuaciones, cecu);
    return 0;
}


/*
calculadora(ecuacion ecuaciones){
    while()
    puts("[A]fjeifjs");
    puts("[B]fjeifjs");
    puts("[C]fjeifjs");
    puts("[D]fjeifjs");
    puts("[E]fjeifjs");
    puts("[F]fjeifjs");
    puts("[G]fjeifjs");
    puts("[H]fjeifjs");
    puts("[X]fjeifjs");
    printf("ingrese opcion");
    scanf();

    switch
        CASE A:
            meterecuacion(ecuacion);
        case B:
            verecuacion();
        case C:
            guardarrevisar();
        case X:
            return;
    }       */



