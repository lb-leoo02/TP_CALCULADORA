#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Vector.h"


#define CAP_INI 10
#define FACTOR_INCR 1.5
#define FACTOR_DECR 0.5


bool redimensionarVector(Vector* v, size_t nuevaCap);
void intercambiar(void* a, void* b, size_t tamElem);
void ordenarBurbujeo(Vector* v);
void ordenarSeleccion(Vector* v, Cmp cmp);
int ordenarInsercion(Vector* v, Cmp cmp);
void* buscarMenor(void* ini, void* fin, size_t tamElem, Cmp cmp);


bool vectorCrear(Vector* v, size_t tamElem)
{
    v->ce = 0;

    v->vec = malloc(CAP_INI * tamElem);

    if(v->vec == NULL) // !v->vec
    {
        v->cap = 0;
        return false;
    }

    v->cap = CAP_INI;
    v->tamElem = tamElem;

    return true;
}


int vectorOrdInsertar(Vector* v, const void* elem, Cmp cmp, Actualizar actualizar)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v, v->cap * FACTOR_INCR))
        {
            return VEC_LLENO;
        }
    }

    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* posIns = v->vec;
    while(posIns <= ult && cmp(elem, posIns) > 0)
    {
        posIns += v->tamElem;
    }

    // 1 - Elem del vec mayor. Sí inserto
    // 2 - Elem del vec igual (Duplicado). No inserto
    // 3 - Elem mayor a todos los del vector. Sí inserto

    if(posIns <= ult && cmp(elem, posIns) == 0)
    {
        if(actualizar)
        {
            actualizar(posIns, elem);
        }

        return DUPLICADO;
    }

    for(void* i = ult; i >= posIns; i -= v->tamElem)
    {
        memcpy(i + v->tamElem, i, v->tamElem);
    }

    memcpy(posIns, elem, v->tamElem);
    v->ce++;

    return TODO_OK;
}

int vectorInsertarEnPos(Vector* v, void* elem, int pos) {
    if (pos < 0 || pos > (int)v->ce)
        return FUERA_DE_RANGO;

    if (v->ce == v->cap) {
        if (!redimensionarVector(v, v->cap * FACTOR_INCR))
            return ERR_MEMORIA;
    }
    void* destino = (char*)v->vec + (pos + 1) * v->tamElem;
    void* origen  = (char*)v->vec + pos * v->tamElem;
    size_t cant_mover = v->ce - pos;

    memmove(destino, origen, cant_mover * v->tamElem);

    void* donde_insertar = (char*)v->vec + pos * v->tamElem;
    memcpy(donde_insertar, elem, v->tamElem);

    v->ce++;
    return TODO_OK;
}

int vectorInsertarAlFinal(Vector* v, const void* elem)
{
    if(v->ce == v->cap)
    {
        if(!redimensionarVector(v, v->cap * FACTOR_INCR))
        {
            return VEC_LLENO;
        }
    }

	void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* posIns = ult + v->tamElem;

	memcpy(posIns, elem, v->tamElem);
	v->ce++;

	return TODO_OK;
}

int vectorObtenerUltimo(Vector* v, void* destino) {
    if (v->ce == 0) {
        return 0;
    }
    size_t offset = (v->ce - 1) * v->tamElem;
    memcpy(destino, (char*)v->vec + offset, v->tamElem);
    return 1;
}

bool vectorOrdEliminarElem(Vector* v, int* elem)
{
    if(v->ce == 0)
    {
        return false;
    }

    int* ult = v->vec + v->ce - 1;
    int* posElim = v->vec;
    while(posElim <= ult && *elem > *posElim)
    {
        posElim++;
    }

    // 1 - elem = posElim. Lo elimino.
    // 2 - elem < posElim. No existe.
    // 3 - elem > todos. No existe.

    if(posElim > ult || *elem < *posElim)
    {
        return false;
    }

    *elem = *posElim; // Sin sentido para ints. Sí para estructuras.

    for(int* i = posElim; i < ult; i++)
    {
        *i = *(i + 1);
    }

    v->ce--;

    if(v->cap * FACTOR_DECR >= CAP_INI && v->ce * 4 <= v->cap)
    {
        redimensionarVector(v, v->cap * FACTOR_DECR);
    }

    return true;
}


void vectorVaciar(Vector* v)
{
    v->ce = 0;
    redimensionarVector(v, CAP_INI);
}


void vectorEliminar(Vector* v)
{
    free(v->vec);
    v->vec = NULL;
    v->ce = 0;
    v->cap = 0;
}


void vectorIntMostrar(const Vector* v)
{
    int* ult = ((int*)v->vec) + v->ce - 1;

    for(int* i = v->vec; i <= ult; i++)
    {
        printf("[%d]: %d\n", (int)(i - (int*)v->vec), *i);
    }
}


void vectorOrdenar(Vector* v, Cmp cmp, int metodo)
{
    switch(metodo)
    {
		case SELECCION:
			ordenarSeleccion(v, cmp);
			break;

		case INSERCION:
		    ordenarInsercion(v,cmp);
		    break;
	}
}

void ordenarSeleccion(Vector* v, Cmp cmp)
{
	void* ult = v->vec + (v->ce - 1) * v->tamElem;
	void* m;

	for(void* i = v->vec; i < ult; i += v->tamElem)
	{
		m = buscarMenor(i, ult, v->tamElem, cmp);
		intercambiar(m, i, v->tamElem);
	}
}


void* buscarMenor(void* ini, void* fin, size_t tamElem, Cmp cmp)
{
	void* m = ini;

	for(void* j = ini + tamElem; j <= fin; j += tamElem)
	{
		if(cmp(j, m) < 0)
		{
			m = j;
		}
	}

	return m;
}

int ordenarInsercion(Vector* v, Cmp cmp) {
    size_t i;
    void* elemAIns = malloc(v->tamElem);
    if (!elemAIns) {
        return ERR_MEMORIA; // Manejo de error en caso de falta de memoria
    }

    for (i = 1; i < v->ce; i++) {
        // Copiar el elemento a insertar
        memcpy(elemAIns, v->vec + i * v->tamElem, v->tamElem);

        // Establecer j en la posición anterior a i
        void* j = v->vec + (i - 1) * v->tamElem;

        // Mover los elementos que son mayores que elemAIns
        while (j >= v->vec && cmp(j, elemAIns) > 0) {
            memcpy(v->vec + (i * v->tamElem), j, v->tamElem);
            i--;
            j = v->vec + (i - 1) * v->tamElem;
        }

        // Insertar elemAIns en su posición correcta
        memcpy(j + v->tamElem, elemAIns, v->tamElem);
    }
    free(elemAIns);
    return TODO_OK;

}

void intercambiar(void* a, void* b, size_t tamElem)
{
	void* aTemp = malloc(tamElem);
    memcpy(aTemp, a, tamElem);
	memcpy(a, b, tamElem);
	memcpy(b, aTemp, tamElem);
    free(aTemp);
}


bool redimensionarVector(Vector* v, size_t nuevaCap)
{
    void* nVec = realloc(v->vec, nuevaCap * v->tamElem);

    if(!nVec)
    {
       return false;
    }

    v->vec = nVec;

    v->cap = nuevaCap;
    return true;
}


int vectorOrdBuscar(const Vector* v, void* elem, Cmp cmp)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* posAct = v->vec;
    while(posAct <= ult && cmp(elem, posAct) > 0)
    {
        posAct += v->tamElem;
    }

    if(posAct > ult || cmp(elem, posAct) != 0)
    {
        return -1;
    }

    memcpy(elem, posAct, v->tamElem);

    return (posAct - v->vec) / v->tamElem;
}


void vectorRecorrer(Vector* v, Accion accion, void* datos)
{
    void* ult = v->vec + (v->ce - 1) * v->tamElem;
    void* posAct = v->vec;
    while(posAct <= ult)
    {
        accion((posAct - v->vec) / v->tamElem, posAct, datos);
        posAct += v->tamElem;
    }
}

void vector_map(Vector* vec, void accion(void*)){
    size_t i;
    if (!vec || !vec->vec || !accion)
        return;

    char *p=vec->vec;
    for(i=0;i<vec->ce;i++){
        accion(p);
        p+=vec->tamElem;
    }
}

int vectorObtener(Vector* v, int i, void* dest) {
    if (i < 0 || i >= (int)v->ce) return FUERA_DE_RANGO;

    void* origen = (char*)v->vec + i * v->tamElem;
    memcpy(dest, origen, v->tamElem);
    return TODO_OK;
}

bool vectorEliminarAlFinal(Vector* v) {
    if (v->ce == 0)
        return false;

    v->ce--;
    return true;
}


bool vectorVacio(Vector* v) {
    return v->ce == 0;
}


//////////////////////////////////////////////////////////////////////////////// Iterador

void vectorIteradorCrear(VectorIterador* it, Vector* v)
{
    it->pri = v->vec;
    it->ult = v->vec + (v->ce - 1) * v->tamElem;
    it->act = NULL;
    it->tamElem = v->tamElem;
    it->finIter = true;
}


void* vectorIteradorPrimero(VectorIterador* it)
{
	if(it->pri > it->ult) // Vector vacío
	{
		return NULL;
	}

	it->act = it->pri;
	it->finIter = false;

	return it->act;
}


void* vectorIteradorUltimo(VectorIterador* it)
{
	if(it->pri > it->ult) // Vector vacío
	{
		return NULL;
	}

	it->act = it->ult;
	it->finIter = false;

	return it->act;
}


void* vectorIteradorSiguiente(VectorIterador* it)
{
	if(it->act == it->ult) // Fin iteración.
	{
		it->finIter = true;
		return NULL;
	}

	it->act += it->tamElem;

	return it->act;
}


void* vectorIteradorAnterior(VectorIterador* it)
{
	if(it->act == it->pri) // Fin iteración.
	{
		it->finIter = true;
		return NULL;
	}

	it->act -= it->tamElem;

	return it->act;
}

void* vectorIteradorActual(VectorIterador* it)
{
	return it->act;
}


bool vectorIteradorFinIter(VectorIterador* it)
{
	return it->finIter;
}
