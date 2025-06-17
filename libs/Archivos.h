#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include "calculadora.h"

int contarArchivos(int*);
int GuardaArchivos(ecuacion*, size_t, int*);
int CargarArchivo(ecuacion*,size_t*);
void quitarenter(char*);
int borrarArchivos();

#endif // ARCHIVOS_H_INCLUDED
