#include "arbol_math.h"
#include "Vector.h"
#include <stdio.h>
#include <string.h>

void print_token(void* elem){
    Token* aux = (Token*)elem;
    printf("[%s]", aux->valor);
}

void printtoken(Vector* tok){
    vector_map(tok,print_token);
}

int tokenizar(Vector *token, char ecu[]){
    char* p_ecu=ecu; // 3x=*2 
    if(!vectorCrear(token,sizeof(Token))){
        return 0;
    }
    Token aux; //{3,{}}}
    char *p_aux;
    int band_igual=0, band_parentesis=0;

    while(*p_ecu){
        memset(&aux, 0, sizeof(Token));
        p_aux=aux.valor;

        if(p_ecu > ecu && *(p_ecu-1) == '=' && !esoperador(*p_ecu)) {
            *p_aux='-';
            p_aux++;
            *p_aux='\0';
            aux.tipo=TOKEN_OP;
            vectorInsertarAlFinal(token,&aux);
            p_aux=aux.valor;
        }

        if(esvariable(*p_ecu)){
            aux.tipo=TOKEN_VAR;
            *p_aux=*p_ecu;
            p_ecu++;
            p_aux++;
        }
        else if(esoperador(*p_ecu)){
            aux.tipo=TOKEN_OP;
            if(*p_ecu == '='){
                band_igual=1;
            }
            else if(band_igual && !band_parentesis){
                if(*p_ecu=='-'){
                    *p_aux='+';
                }
                else if(*p_ecu=='+'){
                    *p_aux='-';
                }
                else {
                    *p_aux=*p_ecu;
                }
            }
            else{//=(2+3)
                *p_aux=*p_ecu;
            }
            
            p_ecu++;
            p_aux++;
        }
        else if(esparentesis(*p_ecu)){ //=(2y-1/2)+(3x+2)
            if(*(p_ecu)=='(')
                band_parentesis++;
            else{
                band_parentesis--;
            }
            aux.tipo=TOKEN_PARENTESIS;//2x+3-2y+1
            *p_aux=*p_ecu;
            p_ecu++;
            p_aux++;    
        }
        else if(esnumero(*p_ecu) || *p_ecu == '.'){//321x^(2)+7y^(3)
            aux.tipo=TOKEN_NUM;
            while(*p_ecu && (esnumero(*p_ecu) || *p_ecu == '.')){
                *p_aux=*p_ecu;
                p_ecu++;
                p_aux++;
            }
        }
        else{
            p_ecu++;
        }
        
        *p_aux='\0';
        if(p_ecu > ecu && *(p_ecu-1)!='=' && vectorInsertarAlFinal(token,&aux) != TODO_OK){
            return 0;
        }//token={{{0},{3,2,1,\0}},{x,\0},{^,\0},{(,\0)},{2,\0},{}}}
    }

    if(band_parentesis){
        printf("\nCERRAR PARENTESIS\n");
        return 0;
    }
    return 1;
}


int esParentesisApertura(Token* t) {
    return t->tipo == TOKEN_PARENTESIS && t->valor[0] == '(';
}

int esParentesisCierre(Token* t) {
    return t->tipo == TOKEN_PARENTESIS && t->valor[0] == ')';
}

void insertarmultiplicaciones(Vector* tokens) {
    Token actual, siguiente;   //token={{{0},{3,2,1,\0}},{x,\0},{^,\0},{(,\0)},{2,\0},{}}}
    int insertar_asterisco;
    size_t i;
    Token op;
    char *p_op;
    for (i = 0; i < tokens->ce - 1; i++){
        vectorObtener(tokens, i, &actual);  //actual={{0},{3,2,1\0}}
        vectorObtener(tokens, i + 1, &siguiente); //siguiente={{1},{x,\0}}}
        insertar_asterisco = 0;

        // 2x  || x*2  ||  2(  || xy  || x( || )2
        if ((actual.tipo == TOKEN_NUM && siguiente.tipo == TOKEN_VAR) ||
            (actual.tipo == TOKEN_VAR && siguiente.tipo == TOKEN_VAR) ||
            (actual.tipo == TOKEN_NUM && esParentesisApertura(&siguiente)) ||
            (actual.tipo == TOKEN_VAR && esParentesisApertura(&siguiente)) ||
            (esParentesisCierre(&actual) && (siguiente.tipo == TOKEN_NUM ||
            siguiente.tipo == TOKEN_VAR || esParentesisApertura(&siguiente)))
        ){
            insertar_asterisco = 1;
        }

        if(insertar_asterisco) {
            op.tipo = TOKEN_OP;
            p_op=op.valor;
            *p_op = '*';
            p_op++;
            *p_op = '\0';

            vectorInsertarEnPos(tokens, &op, i + 1);
            i++;
        }
    }
}

int precedencia(Token t) {
    char op = *t.valor;

    switch(op) {
        case '^': return 3;  // exponente
        case '*':
        case '/': return 2;  // multiplicacion y division
        case '+':
        case '-': return 1;  // suma y resta
        default: return 0;   // otros: parentesis, numeros, variables
    }
}
bool esAsociativoIzq(Token t) {
    char op = *t.valor;
    switch(op) {
        case '^': return false; // exponente es derecha
        case '*':
        case '/':
        case '+':
        case '-': return true; // los demas son izquierda
        default: return true;  // por defecto, tratamos como izquierda
    }
}

Token* vectorTope(Vector* v) {
    if (v->ce == 0) // ce = cantidad de elementos
        return NULL;
    // Obtener puntero al ultimo elemento
    return (Token*) ((char*)v->vec + (v->ce - 1) * v->tamElem);
}

int postfija(Vector* t){
    Vector pila, salida;
    vectorCrear(&pila, sizeof(Token));
    vectorCrear(&salida, sizeof(Token));

    VectorIterador it_token;
    vectorIteradorCrear(&it_token, t);
    vectorIteradorPrimero(&it_token);

    while (!vectorIteradorFinIter(&it_token)) {
        Token* tok_act = vectorIteradorActual(&it_token);

        if (tok_act->tipo == TOKEN_NUM || tok_act->tipo == TOKEN_VAR) {
            vectorInsertarAlFinal(&salida, tok_act);
        }
        else if (tok_act->tipo == TOKEN_OP) {
            Token* tope;
            while (!vectorVacio(&pila) &&
                   (tope = vectorTope(&pila)) &&
                   esoperador(*tope->valor) &&
                   (precedencia(*tope) > precedencia(*tok_act) ||
                    (precedencia(*tope) == precedencia(*tok_act) && esAsociativoIzq(*tok_act)))) {

                Token opTope;
                vectorObtenerUltimo(&pila, &opTope);
                vectorEliminarAlFinal(&pila);
                vectorInsertarAlFinal(&salida, &opTope);
            }
            vectorInsertarAlFinal(&pila, tok_act);
        }
        else if (tok_act->tipo == TOKEN_PARENTESIS) {
            if (esParentesisApertura(tok_act)) {
                vectorInsertarAlFinal(&pila, tok_act);
            }
            else if (esParentesisCierre(tok_act)) {
                Token opTope;
                bool encontrarParentesisIzq = false;
                while (!vectorVacio(&pila)) {
                    vectorObtenerUltimo(&pila, &opTope);
                    vectorEliminarAlFinal(&pila);
                    if (esParentesisApertura(&opTope)) {
                        encontrarParentesisIzq = true;
                        break;
                    } else {
                        vectorInsertarAlFinal(&salida, &opTope);
                    }
                }
                if (!encontrarParentesisIzq) {
                    printf("Error: Parentesis izquierdo no encontrado\n");
                    return 1;
                }
            }
        }

        vectorIteradorSiguiente(&it_token);
    }

    // Vaciar pila restante
    Token opTope;
    while (!vectorVacio(&pila)) {
        vectorObtenerUltimo(&pila, &opTope);
        vectorEliminarAlFinal(&pila);
        if (esParentesisCierre(&opTope) || esParentesisApertura(&opTope)) {
            printf("Error: Parentesis sin pareja en la expresion\n");
            return 1;
        }
        vectorInsertarAlFinal(&salida, &opTope);
    }

    // Reemplazar contenido original por la postfija
    vectorVaciar(t);
    VectorIterador it_salida;
    vectorIteradorCrear(&it_salida, &salida);
    vectorIteradorPrimero(&it_salida);
    while (!vectorIteradorFinIter(&it_salida)) {
        Token* tok = vectorIteradorActual(&it_salida);
        vectorInsertarAlFinal(t, tok);
        vectorIteradorSiguiente(&it_salida);
    }

    vectorEliminar(&salida);
    vectorEliminar(&pila);

    return 0;
}

int crearnodo(nodo** a, int tipo, Token* elem) {
    if (!(*a = malloc(sizeof(nodo))))
        return 1;

    (*a)->tipo = tipo;

    switch ((*a)->tipo) {
        case OPERADOR:
            (*a)->op.operador = *(elem->valor);
            (*a)->op.izq = NULL;
            (*a)->op.der = NULL;
            break;
        case NUMERO:
            (*a)->numero = atof(elem->valor);
            break;
        case VARIABLE:
            (*a)->variable = *(elem->valor);
            break;
    }

    return 0;
}



void EliminarArbol(nodo* root) {
    if (!root) return;

    if (root->tipo == OPERADOR) {
        EliminarArbol(root->op.izq);
        EliminarArbol(root->op.der);
    }

    free(root);
}

void linkearnodos(nodo* aux_arbol,nodo *der,nodo *izq){ //aux2=3 aux3=x || x^2
    aux_arbol->op.izq=izq;
    aux_arbol->op.der=der;
}
/*

Recorrer la postfija y construir el arbol usando una pila X
    Si es numero o variable: creas un nodo y lo apilas.
    Si es operador: desapilas dos nodos, creas uno nuevo con el operador y los
    dos hijos, y lo apilas.
*/
int CrearArbol(Vector *tok, nodo **Arbol){ //vector=[2] [x] [2] [^] [*] [2] [+]
    Vector pila;
    vectorCrear(&pila, sizeof(nodo*));
    VectorIterador i_token;
    vectorIteradorCrear(&i_token,tok);
    vectorIteradorPrimero(&i_token);

    Token *aux;
    nodo* aux2;
    nodo* aux3;
    nodo* aux_arbol;
    int tipo;
    //aux_arbol         aux2        aux3

    while(!vectorIteradorFinIter(&i_token)){
        aux=vectorIteradorActual(&i_token);
        if(aux->tipo == TOKEN_VAR || aux->tipo == TOKEN_NUM){
            if(aux->tipo == TOKEN_VAR)
                tipo=VARIABLE;
            else
                tipo=NUMERO;

            crearnodo(&aux_arbol,tipo,aux);
            vectorInsertarAlFinal(&pila,&aux_arbol);
        }
        else if(aux->tipo == TOKEN_OP){ //pila(aux_arbol, aux_arbol, )
                                                // [3] [x] [2] [^] [*] [2] [+]
            vectorObtenerUltimo(&pila,&aux2);//3    //pila= 2 x 3 ^
            vectorEliminarAlFinal(&pila);
            vectorObtenerUltimo(&pila,&aux3);//x
            vectorEliminarAlFinal(&pila);
            tipo=OPERADOR;
            crearnodo(&aux_arbol,OPERADOR,aux);
            linkearnodos(aux_arbol,aux2,aux3);
            vectorInsertarAlFinal(&pila,&aux_arbol);
        }
        vectorIteradorSiguiente(&i_token);
    }
    vectorObtenerUltimo(&pila,Arbol);
    vectorEliminar(&pila);

    return 0;
}
