#ifndef CALCULADORA_BMP_H_INCLUDED
#define CALCULADORA_BMP_H_INCLUDED

#include "calculadora.h"

void guardarBMP(const char* nombre, int ancho, int alto, unsigned char* datos);
void graficar_ecuacion(ecuacion *,int ,double ,double ,double ,double );
void graficar_ecuacion_consola(ecuacion* ecuaciones, int indice, double x_min, double x_max, double y_min, double y_max);

#endif // CALCULADORA_BMP_H_INCLUDED