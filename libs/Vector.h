#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define TODO_OK 0
#define VEC_LLENO 1
#define DUPLICADO 2
#define FUERA_DE_RANGO 5

#define BURBUJEO 1
#define SELECCION 2
#define INSERCION 3

#define ERR_MEMORIA 4


typedef struct
{
    void* vec;
    size_t ce;
    size_t cap;
    size_t tamElem;
}
Vector;


typedef int (*Cmp)(const void* e1, const void* e2);
typedef void (*Actualizar)(void* actualizado, const void* actualizador);
typedef void (*Accion)(int pos, void* e, void* datos);


bool vectorCrear(Vector* v, size_t tamElem); // malloc()
int vectorOrdInsertar(Vector* v, const void* elem, Cmp cmp, Actualizar actualizar); // realloc()
int vectorInsertarEnPos(Vector* v, void* elem, int pos); // realloc()
int vectorInsertarAlInicio(Vector* v, int elem); // realloc()
int vectorInsertarAlFinal(Vector* v, const void* elem); // realloc()
int vectorOrdBuscar(const Vector* v, void* elem, Cmp cmp); // Búsqueda binaria / dicotómica
int vectorBuscar(const Vector* v, int* elem);
bool vectorOrdEliminarElem(Vector* v, int* elem); // realloc()
bool vectorEliminarElem(Vector* v, int* elem); // realloc()
bool vectorEliminarElemDePos(Vector* v, int pos, int* elemElim); // realloc()
bool vectorEliminarElemPrimero(Vector* v, int* elemElim); // realloc()

void vectorOrdenar(Vector* v, Cmp cmp, int metodo);
void vectorVaciar(Vector* v); // realloc()
void vectorEliminar(Vector* v); // free()
void vectorIntMostrar(const Vector* v);
void vectorRecorrer(Vector* v, Accion accion, void* datos);

void vector_map(Vector*,void accion(void*));
int vectorObtener(Vector*, int, void*);
bool vectorEliminarAlFinal(Vector* v);
int vectorObtenerUltimo(Vector* v, void* destino);
bool vectorVacio(Vector* v);



//////////////////////////////////////////////////////////////////////////////// Iterador


typedef struct
{
    void* pri;
    void* ult;
    void* act;
    bool finIter;
    size_t tamElem;
}
VectorIterador;


void vectorIteradorCrear(VectorIterador* it, Vector* v);
void* vectorIteradorPrimero(VectorIterador* it);
void* vectorIteradorUltimo(VectorIterador* it);
void* vectorIteradorSiguiente(VectorIterador* it);
void* vectorIteradorAnterior(VectorIterador* it);
void* vectorIteradorActual(VectorIterador* it);
bool vectorIteradorFinIter(VectorIterador* it);



#endif // VECTOR_H
