#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "Vector.h"
#include "calculadora.h"
#include <string.h>

#define EsOperacion(x) (((x)=='+') || ((x)=='-') || ((x)=='/') || ((x)=='*') ||  ((x)=='^'))
#define EsVariable(x) (((x)=='x') || ((x)=='y') || ((x)=='X') || ((x)=='Y'))
#define Amayus(x) (((x) >= 'a' && (x) <= 'z') ? ((x) - ('a' - 'A')) : (x))

#define TAM_ECUACION 512
#define MAX_ECUAC 3

typedef enum{
    ECUACION_OK,
    SYNTAX_ERROR,
    DOBLE_OPERADOR_ERROR,
    POTENCIA_PARENTESIS_ERROR,
    PARENTESIS_NO_CERRADO,
    DIVISION_ENTRE_CERO,
    NO_ES_ECUACION,
    ERROR_IGUAL,
    FALTA_OPERANDO,
    ERROR_IGUAL_OPERADOR,
    PARENTESIS_ERROR
}EstadoEcuacion;

void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");     // Windows
    #else
        system("clear");   // Linux / macOS
    #endif
}


void destruirecuaciones(ecuacion *ecu, size_t n) {
    if (ecu == NULL) return;

    ecuacion *p_ecu = ecu;
    for (size_t i = 0; i < n; ++i) {
        if (p_ecu->arbol_ecu != NULL) {
            EliminarArbol(p_ecu->arbol_ecu);
            p_ecu->arbol_ecu = NULL;
        }
        p_ecu++;
    }
}



/*
char menu(){
    char linea[100];  // buffer suficientemente grande
    char letra;

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("[A] - Escribir ecuacion\n");
        printf("[B] - Ver ecuaciones\n");
        printf("[C] - Guardar y reiniciar\n");
        printf("[D] - Leer ecuaciones\n");
        printf("[E] - Borrar ecuaciones guardadas\n");
        printf("[F] - Resolver ecuacion\n");
        printf("[G] - Graficar ecuacion\n");
        printf("[H] - Ayuda\n");
        printf("[X] - Salir\n");
        printf("Seleccione una opcion: ");

        fgets(linea, sizeof(linea), stdin); // lee toda la línea
        letra = toupper(linea[0]);          // toma el primer carácter

        if(letra != 'X' && (letra < 'A' || letra > 'H')){
            printf("Ingrese letra valida.\n");
        }

    } while(letra != 'X' && (letra < 'A' || letra > 'H'));

    return letra;
}*/
/*
char menu(){
    char letra[2];
    char *p_letra=letra;
        limpiarPantalla();
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("[A] - Escribir ecuacion\n");
        printf("[B] - Ver ecuaciones\n");
        printf("[C] - Guardar y reiniciar\n");
        printf("[D] - Leer ecuaciones\n");
        printf("[E] - Borrar ecuaciones guardadas\n");
        printf("[F] - Resolver ecuacion\n");
        printf("[G] - Graficar ecuacion\n");
        printf("[H] - Ayuda\n");
        printf("[X] - Salir\n");
        printf("Seleccione una opcion: ");
    do{
        fgets(letra, 2, stdin);
        *p_letra=Amayus(*p_letra);
        getchar();
        if(*p_letra != 'X' && (*p_letra<'A' || *p_letra>'H')){
            printf("Ingrese letra valida:\n");
        }
    }while(*p_letra != 'X' && (*p_letra<'A' || *p_letra>'H'));

return *p_letra;
}*/

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char menu(){
    char letra;

    //limpiarPantalla();
    printf("\n--- MENU PRINCIPAL ---\n");
    printf("[A] - Escribir ecuacion\n");
    printf("[B] - Ver ecuaciones\n");
    printf("[C] - Guardar y reiniciar\n");
    printf("[D] - Leer ecuaciones\n");
    printf("[E] - Borrar ecuaciones guardadas\n");
    printf("[F] - Resolver ecuacion\n");
    printf("[G] - Graficar ecuacion\n");
    printf("[H] - Ayuda\n");
    printf("[X] - Salir\n");
    printf("Seleccione una opcion: ");
    do {
        letra = getchar();            // Lee un solo carácter
        limpiarBuffer();             // Descarta el resto de la línea
        letra = Amayus(letra);       // Convertir a mayúscula si hace falta

        if (letra != 'X' && (letra < 'A' || letra > 'H')) {
            printf("Ingrese una opcion valida: ");
        }
    } while (letra != 'X' && (letra < 'A' || letra > 'H'));

    return letra;
}

void quitarespacios(char* ecu) {
    char* origen = ecu;
    char* destino = ecu;

    while (*origen) {
        if (!isspace(*origen)) {
            *destino = *origen;
            destino++;
        }
        origen++;
    }
    *destino = '\0';
}

int verificarecuacion(char* ecu){
    char *p_ecu=ecu; // -2^(1/2) || 1/(x-2) cuando x=2 
    int band_error=ECUACION_OK, cont_par_abi=0, cont_par_cerr=0, cont_variables=0, cont_igual=0;
    quitarespacios(ecu); 
    while(*p_ecu && !band_error && (esoperador(*p_ecu) || esnumero(*p_ecu) || esvariable(*p_ecu) || esparentesis(*p_ecu))){

        
        if(p_ecu > ecu && *(p_ecu-1)=='=' && !(*p_ecu=='+' ||*p_ecu=='-'  || *p_ecu=='(' || esnumero(*p_ecu) || esvariable(*p_ecu))){
            band_error=ERROR_IGUAL_OPERADOR;
        }
        if(*ecu=='='){ 
            band_error=ERROR_IGUAL;
        }
        if(*p_ecu=='='){
            cont_igual++;
        }
        if(EsOperacion(*p_ecu) && EsOperacion(*(p_ecu+1)))
            band_error=DOBLE_OPERADOR_ERROR;

        if(EsOperacion(*p_ecu) && !(esnumero(*(p_ecu+1)) || esvariable(*(p_ecu+1)) || esparentesis(*(p_ecu+1))))
            band_error=FALTA_OPERANDO;

        if(*p_ecu=='(' && !(esnumero(*(p_ecu+1)) || esvariable(*(p_ecu+1)) || esparentesis(*(p_ecu+1)) || *(p_ecu+1)=='-'))
            band_error=PARENTESIS_ERROR;

        if(*p_ecu == '^' && *(p_ecu+1)!= '('){ //
            band_error=POTENCIA_PARENTESIS_ERROR;
        }


        if(*p_ecu == '('){
            cont_par_abi++;
        }else if(*p_ecu==')')
            cont_par_cerr++;

        if(*p_ecu == '/' && *(p_ecu+1)== '0'){ // x/(0)
            band_error=DIVISION_ENTRE_CERO;
        }

        if(esvariable(*p_ecu)){
            cont_variables++;
        }
        p_ecu++;
    }

    if(!*p_ecu=='\0' && !esoperador(*p_ecu) && !esnumero(*p_ecu) && !esvariable(*p_ecu) && !esparentesis(*p_ecu)){
        band_error=SYNTAX_ERROR;
        return band_error;
    }
    if(p_ecu > ecu && *(p_ecu-1)=='=' || cont_igual!=1){
        band_error=ERROR_IGUAL;
    }
    if(!cont_variables)
        band_error=NO_ES_ECUACION;

    if(cont_par_abi != cont_par_cerr)
        band_error=PARENTESIS_NO_CERRADO;

    if(band_error){
        return band_error;
    }

return ECUACION_OK;
}


/*
void introducirecuacion(char* ecu) {
    EstadoEcuacion estado;

    do {
        printf("Introduzca una ecuacion: ");
        fgets(ecu, TAM_ECU, stdin);

        ecu[strcspn(ecu, "\n")] = '\0';  // Eliminar salto de línea si quedo

        estado = verificarecuacion(ecu);

        if (estado != ECUACION_OK) {
            ayuda(); // Muestra ayuda si hay error
            printf("Ecuacion invalida. Vuelva a intentar.\n");
        }

    } while (estado != ECUACION_OK);
}*/
void introducirecuacion(char* ecu) {
    char buffer[TAM_ECU];
    memset(buffer, 0, sizeof(buffer));
    memset(ecu, 0, TAM_ECUACION);
    char* p;

    printf("Introduzca una ecuacion: ");

    if (fgets(buffer, sizeof(buffer), stdin)) {
        // Recorremos el buffer para quitar '\n' y espacios, usando punteros
        p = buffer;
        while (*p && *p != '\n') p++;
        if (*p == '\n') *p = '\0';

        // Limpiar espacios con quitarespacios (ya usa punteros)
        quitarespacios(buffer);

        // Copiar a ecu solo si entra en el tamaño permitido
        p = buffer;
        char* dest = ecu;
        size_t count = 0;
        while (*p && count < TAM_ECUACION - 1) {
            *dest++ = *p++;
            count++;
        }
        *dest = '\0';
    } else {
        // Si fgets falla, dejar la ecuación vacía
        *ecu = '\0';
    }
}


void mostrarecuaciones(ecuacion *ecuac){
    ecuacion *p_ecuac=ecuac;
    for(int i=0; i<MAX_ECUAC; i++){
        printf("\necuacion %d: %s\n",i+1, p_ecuac->ecu);
        p_ecuac++;
    }
}

void mostrar_char(char* p) {
    printf("%c ", *p);
}

void mostrar_double(double* p) {
    printf("%.2f ", *p);
}

void inorder(nodo* root, void (*mostrar1)(char*), void (*mostrar2)(double*)) {
    if (!root)
        return;

    if (root->tipo == OPERADOR) {
        inorder(root->op.izq, mostrar1, mostrar2);
        mostrar1(&root->op.operador);
        inorder(root->op.der, mostrar1, mostrar2);
    }
    else if (root->tipo == VARIABLE) {
        mostrar1(&root->variable);
    }
    else if (root->tipo == NUMERO) {
        mostrar2(&root->numero);
    }
}


int ingresarecu(ecuacion *ecuac, size_t *cecu){
    int num=0;
    ecuacion aux_ecuacion={0}, *p_ecuac;
    char ecuU[TAM_ECUACION]={0};
    introducirecuacion(ecuU);

    aux_ecuacion.arbol_ecu=NULL;
    p_ecuac=ecuac;

    if(*cecu<MAX_ECUAC){
        p_ecuac+=*cecu;
        (*cecu)++;
    }else{
        printf("Memoria llena ¿Que ecuacion desea reemplazar?: "); 
        mostrarecuaciones(ecuac);                                 
        do{
            if(num<1 || num>10)
                printf("\nIngrese un numero valido:");
            scanf("%d", &num);
        }while(num<1 || num>10);

        aux_ecuacion.arbol_ecu=NULL;
        p_ecuac=ecuac;
        p_ecuac+=(num-1);
    }

    strcpy(aux_ecuacion.ecu, ecuU);
    *p_ecuac=aux_ecuacion;


    Vector ecuac_token;
    tokenizar(&ecuac_token,ecuU); //TOKENIZAMOS ecuU (la ecuacion actual)

    puts("");
    printtoken(&ecuac_token);
    puts("");
    
    insertarmultiplicaciones(&ecuac_token); //Ingresamos las multiplicaciones implicitas


    puts("");
    printtoken(&ecuac_token);
    puts("");

    postfija(&ecuac_token); //PASAMOS EL TOKEN A POSTFIJA
    CrearArbol(&ecuac_token,&(p_ecuac->arbol_ecu)); //CREAMOS EL ARBOL CON LOS DATOS DE LA ECUACION
    inorder(p_ecuac->arbol_ecu, mostrar_char, mostrar_double);



    vectorEliminar(&ecuac_token);

return 0;
}

void ayuda(){
    puts("INTRODUCIR ECUACIONES SIGUIENDO LAS SIGUIENTES REGLAS PARA EL CORRECTO FUNCIONAMIENTO DEL PROGRAMA:");
    puts("VARIABLES: Solo se acepta X e Y.");
    puts("OPERADORES: Se acepta tanto suma (+) resta (-) multiplicacion (*), division (/), Potencias y Raices.");
    puts("POTENCIA: Para ingresar potencias se debe colocar el caracter ^ seguido de parentesis y el numero de la potencia.");
    puts("\tEJ: X al cuadrado se escribiria como x^(2).");
    puts("RAIZ: Para ingresar las raices se debe colocar en su forma de potencia m/n.");
    puts("\tEJ: para la raiz cuadrada de Y se escribiria como y^(1/2).");
    puts("\tEJ2: Si dentro de la raiz hay mas de un elemento tambien se debe poner en parentesis.");
    puts("\t\tEJ2.1: Raiz cuadrada de x mas uno se escribiria como (x+1)^(1/2).");

    puts("PULSE ENTER PARA CERRAR");
    getchar();
    limpiarBuffer();
}


