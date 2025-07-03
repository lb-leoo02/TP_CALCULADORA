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

void EnterParaSalir();
void limpiarPantalla();
void limpiarBuffer();
char pedir_opcion(const char *, const char*);
char menu();
void quitarespacios(char* ecu);
int ingresarecu(ecuacion*, size_t*,int,void*);
void mostrarecuaciones(ecuacion*, size_t,int);
void ayuda_ecuacion();
void menu_ayuda_interactivo();
void borrarecuacion(ecuacion*, size_t*);
void destruirecuaciones(ecuacion*, size_t);
void graficar(ecuacion* ecuaciones, size_t cecu,int* indice);
double evaluarArbol(nodo*, double, double);
void EvaluarListaDeValores(ecuacion *ecu, size_t cecu);
void BuscarRaices(ecuacion*, size_t);

int buscar_raiz_multi(nodo* arbol, double fijo, double min, double max, double tolerancia, int max_iter, double* soluciones, int max_sol, int buscar_y);

void TablaAlrededorDeValor(ecuacion *ecu, size_t cecu);

#endif // CALCULADORA_H_INCLUDED
