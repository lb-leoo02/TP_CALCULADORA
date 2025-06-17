#ifndef CALCULADORA_H_INCLUDED
#define CALCULADORA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"
#include "arbol_math.h"


#define TAM_ECU 512

typedef struct{
    nodo *arbol_ecu;    //arbol de ecuacion
    char ecu[TAM_ECU];  //char de ecuacion
}ecuacion;

void limpiarBuffer();
char menu();
void quitarespacios(char* ecu);
int ingresarecu(ecuacion*, size_t*,int,void*);
void mostrarecuaciones(ecuacion*, size_t);
//int AniadirEcuacion(ecuacion*, size_t*);
void ayuda();
void borrarecuacion(ecuacion*, size_t*);
void destruirecuaciones(ecuacion*, size_t);

void evaluarecuacion(nodo*arbol, double valor); 



#endif // CALCULADORA_H_INCLUDED
