#ifndef ARBOL_MATH_H_INCLUDED
#define ARBOL_MATH_H_INCLUDED

#define esvariable(x) (((x)=='x') || ((x)=='y') || ((x)=='X') || ((x)== 'Y'))
#define esnumero(x) (((x)>='0') && ((x)<='9'))
#define esoperador(x) (((x)=='+') || ((x)=='-') || ((x)=='*') || ((x)=='/') || ((x)=='^') || ((x)=='='))
#define esparentesis(x) (((x)=='(') || ((x)== ')'))

#include "Vector.h"



typedef enum {
    OPERADOR,
    NUMERO,
    VARIABLE
} TipoNodo;

typedef struct nodo {
    TipoNodo tipo;

    union {
        struct {
            char operador;// '+', '-', '*', '/', '^'
            struct nodo* izq;
            struct nodo* der;
        } op;

        double numero;

        char variable;// x o y
    };
} nodo;

typedef enum {
    TOKEN_NUM,
    TOKEN_VAR,
    TOKEN_OP,
    TOKEN_PARENTESIS
} TipoToken;

typedef struct {
    TipoToken tipo;
    char valor[16]; //
} Token; //  321x^2+73y  [321][x][^]

/*
    VERIFICAR ECUACION
    Ingresar la expresion infija || X 321x^2+73y
    TOKENIZAR ECUACION xx
    Convertir a notacion postfija (RPN)  ||321 x 2 ^  73 y +
        Usas el algoritmo de Shunting Yard.
    Recorrer la postfija y construir el arbol usando una pila X
        Si es numero o variable: creas un nodo y lo apilas.
        Si es operador: desapilas dos nodos, creas uno nuevo con el operador y los
        dos hijos, y lo apilas.
    El ultimo nodo en la pila es la raiz del arbol

*/  //321 x 2 ^ *  73 y * +
//pila: N(*) N(*) N(+)
//salida:

//Separa la ecuacion en tokens independientes para mejorar su manejo
int tokenizar(Vector*,char[]);
void print_token(void* elem);
void printtoken(Vector*);
//Orden de precedencia de los operadores
int precedencia(Token t);
//Operadores cuyo valor se asocian con su variable o numero de la izquierda
bool esAsociativoIzq(Token t);
//vectortope para verificar en pilas de tda
Token* vectorTope(Vector* v);
//verifica si es '('
int esParentesisApertura(Token* t);
//verifica si es '('
int esParentesisCierre(Token* t);
//Pone las multiplicaciones implicitas de cada ecuacion
void insertarmultiplicaciones(Vector* tokens);
/*
Leer el siguiente token (numero, variable, operador o parentesis).

Si es un numero o variable:
  Agregarlo directamente a la salida.

Si es un operador (+, *, etc.):
  Mientras haya operadores en la pila con mayor o igual precedencia, sacarlos de la pila y
  ponerlos en la salida.
  Luego, apilar el operador actual.

Si es un parentesis izquierdo (:
  Apilarlo.
  pila=(+,(,+,)
Si es un parentesis derecho ):
  Sacar operadores de la pila y agregarlos a la salida
hasta encontrar un parentesis izquierdo (.
  Eliminar el parentesis izquierdo de la pila (no se agrega a la salida).

Al final de la expresion:
  Sacar y agregar a la salida todos los operadores restantes en la pila.
*/
int postfija(Vector*);

/*
Recorrer la postfija y construir el arbol usando una pila X
    Si es numero o variable: creas un nodo y lo apilas.
    Si es operador: desapilas dos nodos, creas uno nuevo con el operador y los
    dos hijos, y lo apilas.
*/

//Se crea nodo y se inicializa en NULL y tipo de dato
int crearnodo(nodo**, int,Token*);
//Se linkean nodos para crear arbol el primer dato es la raiz el siguiente es la hoja der y el tercer dato es la hoja izq
void linkearnodos(nodo*,nodo*,nodo*);
//toma la ecuacion ya TOKENIZADA y la convierte en arbol recorriendolo en postfija
int CrearArbol(Vector*,nodo**);
//libera el espacio de memoria creado  por el arbol entero
void EliminarArbol(nodo*);



#endif // ARBOL_MATH_H_INCLUDED
