#include "calculadora.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t tipo;
    uint32_t tamano;
    uint16_t reservado1;
    uint16_t reservado2;
    uint32_t offset;
} CabeceraBMP;

typedef struct {
    uint32_t tamano;
    int32_t  ancho;
    int32_t  alto;
    uint16_t planos;
    uint16_t bits_por_pixel;
    uint32_t compresion;
    uint32_t tamano_imagen;
    int32_t  x_por_metro;
    int32_t  y_por_metro;
    uint32_t colores_usados;
    uint32_t colores_importantes;
} InfoBMP;
#pragma pack(pop)

void guardarBMP(const char* nombre, int ancho, int alto, unsigned char* datos) {
    CabeceraBMP cabecera = {0};
    InfoBMP info = {0};
    FILE* archivo = fopen(nombre, "wb");
    if (!archivo) return;

    int fila_padded = (ancho*3 + 3) & (~3);
    int tamano_datos = fila_padded * alto;

    cabecera.tipo = 0x4D42;
    cabecera.tamano = sizeof(CabeceraBMP) + sizeof(InfoBMP) + tamano_datos;
    cabecera.offset = sizeof(CabeceraBMP) + sizeof(InfoBMP);

    info.tamano = sizeof(InfoBMP);
    info.ancho = ancho;
    info.alto = alto;
    info.planos = 1;
    info.bits_por_pixel = 24;
    info.tamano_imagen = tamano_datos;

    fwrite(&cabecera, sizeof(cabecera), 1, archivo);
    fwrite(&info, sizeof(info), 1, archivo);

    // BMP se guarda de abajo hacia arriba
    for (int fila = alto-1; fila >= 0; fila--) {
        unsigned char* p_fila = datos + fila*ancho*3;
        fwrite(p_fila, 3, ancho, archivo);
        for (int p = 0; p < fila_padded - ancho*3; p++)
            fputc(0, archivo);
    }
    fclose(archivo);
}

void graficar_ecuacion(ecuacion* ecuaciones, int indice, 
                       double x_min, double x_max, double y_min, double y_max) {
    int ancho = 500, alto = 500;
    unsigned char* datos = (unsigned char*)calloc(ancho*alto*3, 1);

    // Fondo blanco
    for (unsigned char* p = datos; p < datos + ancho*alto*3; p++)
        *p = 255;

    // Calcular posicion de los ejes
    int eje_x = (int)((0 - y_min) * (alto - 1) / (y_max - y_min)); // fila para y=0
    int eje_y = (int)((0 - x_min) * (ancho - 1) / (x_max - x_min)); // columna para x=0

    // Dibujar ejes en gris claro (200,200,200)
    if(eje_x >= 0 && eje_x < alto) {
        for (int px = 0; px < ancho; px++) {
            unsigned char* p_pixel = datos + ((alto - 1 - eje_x) * ancho + px) * 3;
            *(p_pixel + 0) = 200;
            *(p_pixel + 1) = 200;
            *(p_pixel + 2) = 200;
        }
    }
    if(eje_y >= 0 && eje_y < ancho) {
        for (int py = 0; py < alto; py++) {
            unsigned char* p_pixel = datos + (py * ancho + eje_y) * 3;
            *(p_pixel + 0) = 200;
            *(p_pixel + 1) = 200;
            *(p_pixel + 2) = 200;
        }
    }

    // Graficar la curva en azul
    for (int px = 0; px < ancho; px++) {
        double x = x_min + (x_max - x_min) * px / (ancho - 1);
        for (int py = 0; py < alto; py++) {
            double y = y_min + (y_max - y_min) * (alto - 1 - py) / (alto - 1);
            double resultado = evaluarArbol((ecuaciones + indice)->arbol_ecu, x, y);
            if (fabs(resultado) < 0.1) {
                unsigned char* p_pixel = datos + (py * ancho + px) * 3;
                *(p_pixel + 0) = 255; // Azul
                *(p_pixel + 1) = 0;
                *(p_pixel + 2) = 0;
            }
        }
    }
    guardarBMP("grafico.bmp", ancho, alto, datos);
    free(datos);
}

void graficar_ecuacion_consola(ecuacion* ecuaciones, int indice, double x_min, double x_max, double y_min, double y_max) {
    int ancho = 80;   // columnas de la consola
    int alto = 30;    // filas de la consola
    char matriz[30][81];

    // Inicializar fondo
    for(int i=0; i<alto; i++) {
        for(int j=0; j<ancho; j++)
            matriz[i][j] = ' ';
        matriz[i][ancho] = '\0';
    }

    // Calcular posicion de los ejes
    int eje_x = (int)((0 - y_min) * (alto - 1) / (y_max - y_min));
    int eje_y = (int)((0 - x_min) * (ancho - 1) / (x_max - x_min));

    // Dibujar ejes
    if(eje_x >= 0 && eje_x < alto)
        for(int j=0; j<ancho; j++)
            matriz[alto-1-eje_x][j] = '-';
    if(eje_y >= 0 && eje_y < ancho)
        for(int i=0; i<alto; i++)
            matriz[i][eje_y] = '|';

    // Graficar la curva
    for(int j=0; j<ancho; j++) {
        double x = x_min + (x_max - x_min) * j / (ancho - 1);
        for(int i=0; i<alto; i++) {
            double y = y_min + (y_max - y_min) * (alto - 1 - i) / (alto - 1);
            double resultado = evaluarArbol((ecuaciones + indice)->arbol_ecu, x, y);
            if (fabs(resultado) < 0.42) {
                matriz[i][j] = '*';
            }
        }
    }

    // Mostrar la matriz
    for(int i=0; i<alto; i++)
        printf("%s\n", matriz[i]);
}

