#ifndef CALCULADORA_H_INCLUDED
#define CALCULADORA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"
#include "arbol_math.h"

#define TAM_ECU 512

typedef struct{
    nodo *arbol_ecu;    //ecuacion
    char ecu[TAM_ECU];
}ecuacion;

char menu();
void quitarespacios(char* ecu);
int ingresarecu(ecuacion*, size_t*);
void mostrarecuaciones(ecuacion* );
//int AniadirEcuacion(ecuacion*, size_t*);
void ayuda();
void destruirecuaciones(ecuacion*, size_t);



#endif // CALCULADORA_H_INCLUDED
