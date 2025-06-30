#ifndef CALCULADORA_H_INCLUDED
#define CALCULADORA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Vector.h"
#include "arbol_math.h"

#define EsOperacion(x) (((x)=='+') || ((x)=='-') || ((x)=='/') || ((x)=='*') ||  ((x)=='^'))
#define EsVariable(x) (((x)=='x') || ((x)=='y') || ((x)=='X') || ((x)=='Y'))
#define Amayus(x) (((x) >= 'a' && (x) <= 'z') ? ((x) - ('a' - 'A')) : (x))


#define TAM_ECU 512

typedef struct{
    nodo *arbol_ecu;    //arbol de ecuacion
    char ecu[TAM_ECU];  //char de ecuacion
}ecuacion;

void limpiarBuffer();
char pedir_opcion(const char *, const char*);
char menu();
void quitarespacios(char* ecu);
int ingresarecu(ecuacion*, size_t*,int,void*);
void mostrarecuaciones(ecuacion*, size_t);
//int AniadirEcuacion(ecuacion*, size_t*);
void ayuda();
void borrarecuacion(ecuacion*, size_t*);
void destruirecuaciones(ecuacion*, size_t);

double evaluarArbol(nodo*, double, double);
void EvaluarXeY(ecuacion*,size_t);
void GenerarTabla(ecuacion*,size_t);


#endif // CALCULADORA_H_INCLUDED
