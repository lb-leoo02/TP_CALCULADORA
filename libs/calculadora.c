#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "Vector.h"
#include "calculadora.h"
#include <string.h>
#include "Archivos.h"



#define TAM_ECUACION 512
#define MAX_ECUAC 2

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


void borrarecuacion(ecuacion *ecu , size_t *ce){
    destruirecuaciones(ecu,*ce);
    *ce=0;
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

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char pedir_opcion(const char *validas, const char *mensaje) {
    char op;
    printf("%s", mensaje);
    do {
        do {
            op = getchar();
        } while (op == '\n' || op == ' ');  // ignora enter o espacios

        limpiarBuffer();  // limpia el resto de la línea
        op = Amayus(op);

        if (!strchr(validas, op)) {
            printf("Ingrese una opción válida: ");
        }
    } while (!strchr(validas, op));

    return op;
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
    if(p_ecu > ecu && (*(p_ecu-1)=='=' || cont_igual!=1)){
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
void introducirecuacion(char* ecu) {
    EstadoEcuacion estado;
    char confirmacion;
    do {
        estado=1;
        printf("Introduzca una ecuacion: ");
        fgets(ecu, TAM_ECU, stdin);
        quitarenter(ecu);
        printf("\nEcuacion: %s",ecu);
        printf("\nDesea introducir esta ecuacion? S/N: ");
        do{
            scanf("%c",&confirmacion);
            limpiarBuffer();
            confirmacion=Amayus(confirmacion);
            if(confirmacion!='S' && confirmacion!='N')
                printf("\nEliga una opcion correcta: ");
        }while(confirmacion!='S' && confirmacion!='N');
        
        if(confirmacion=='S'){
            estado = verificarecuacion(ecu);

            if (estado != ECUACION_OK) {
                ayuda(); // Muestra ayuda si hay error
                printf("Ecuacion invalida. Vuelva a intentar.\n");
            }
        }

    } while (estado != ECUACION_OK);
}

void mostrarecuaciones(ecuacion *ecuac, size_t ce){
    ecuacion *p_ecuac=ecuac;
    for(int i=0; i<ce; i++){
        printf("\necuacion %d: %s",i+1, p_ecuac->ecu);
        p_ecuac++;
    }
    
}

/////////////////////////////////////////////////////////////////////////////////INI: PRINTEO

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
/////////////////////////////////////////////////////////////////////////////////FIN: PRINTEO

void introducirecuacionarchivo(char* ecuU, void* ecuarch){
    char *p_ecuarch=(char*)ecuarch, *p_ecuU=ecuU;
    while(*p_ecuarch){
        if(*p_ecuarch!='\n')
            *p_ecuU=*p_ecuarch;
        p_ecuU++;
        p_ecuarch++;
    }
    p_ecuU++;
    p_ecuU='\0';
}

int ingresarecu(ecuacion *ecuac, size_t *cecu, int ingreso, void*ecuarch){
    //INGRESO ==1 :CARGAR POR USUARIO
    //INGRESO ==0: CARGAR POR ARCHIVO
    //INGRESO ==2
    int num=-1;
    ecuacion aux_ecuacion={0}, *p_ecuac;
    char ecuA[TAM_ECUACION]={0};

    if(ingreso==1 || ingreso==2){
        if(ingreso==2)
            printf("\nReemplazar Ecuacion\n");
        introducirecuacion(ecuA);
    }
    else if(ingreso==0){
        introducirecuacionarchivo(ecuA,ecuarch);
    }

    aux_ecuacion.arbol_ecu=NULL;
    p_ecuac=ecuac;

    if(ingreso==1 && *cecu<MAX_ECUAC){
        p_ecuac+=*cecu;
        (*cecu)++;
    }
    else{
        if(ingreso!=2)
            printf("Memoria llena. Que ecuacion desea reemplazar?: ");
        else
            printf("Que ecuacion desea reemplazar?: ");
        
        printf("\necuacion 0: %s","CANCELAR");
        mostrarecuaciones(ecuac,*cecu);
        do{
            if(num<0 || num>10)
                printf("\nIngrese un numero valido:");
            scanf("%d", &num);
        }while(num<0 || num>10);

        aux_ecuacion.arbol_ecu=NULL;
        p_ecuac=ecuac;
        p_ecuac+=(num-1);
    }

    if(num==0)
        return 0;

    strcpy(aux_ecuacion.ecu, ecuA);
    if(num>0)
        EliminarArbol((p_ecuac->arbol_ecu)); //eliminar arbol anterior en caso de reemplazarlo
    *p_ecuac=aux_ecuacion;


    Vector ecuac_token;
    tokenizar(&ecuac_token,ecuA); //TOKENIZAMOS ecuA (la ecuacion actual)

    puts("");
    printtoken(&ecuac_token);
    puts("");

    insertarmultiplicaciones(&ecuac_token); //Ingresamos las multiplicaciones implicitas


    puts("");
    printtoken(&ecuac_token);
    puts("");

    postfija(&ecuac_token); //PASAMOS EL TOKEN A POSTFIJA
    CrearArbol(&ecuac_token,&(p_ecuac->arbol_ecu)); //CREAMOS EL ARBOL CON LOS DATOS DE LA ECUACION
    

    inorder(p_ecuac->arbol_ecu, mostrar_char, mostrar_double); //printea el arbol inorder



    vectorEliminar(&ecuac_token);

return 0;
}

double evaluarArbol(struct nodo* arbol, double x, double y) {
    if (arbol == NULL) return 0; // seguridad

    switch (arbol->tipo) {
        case NUMERO:
            return arbol->numero;

        case VARIABLE:
            return (arbol->variable == 'x') ? x : y;

        case OPERADOR: {
            double izq = evaluarArbol(arbol->op.izq, x, y);
            double der = evaluarArbol(arbol->op.der, x, y);

            switch (arbol->op.operador) {
                case '+': return izq + der;
                case '-': return izq - der;
                case '*': return izq * der;
                case '/': return der != 0 ? izq / der : 0; // proteger división por cero
                case '^': return pow(izq, der);
                default: return 0; // operador desconocido
            }
        }
    }

    return 0; // caso no reconocido
}

void EvaluarXeY(ecuacion *ecu,size_t cecu){
    int i=0;
    char op='a';
    double x,y, resultado;
    ecuacion *p_ecu=ecu;
    mostrarecuaciones(ecu,cecu);
    printf("\nIngrese el numero que quiera evaluar: ");
    do{
        scanf("%d",&i);
        if(i<0 || i>cecu)
            printf("\nIngrese una ecuacion valida: ");
    }while(i<0 || i>cecu);

    p_ecu+=(i-1);
    printf("\nusted selecciono: %s", p_ecu->ecu);

    while(op!='N'){
        printf("\nIngrese el valor de x: ");
        scanf("%lf",&x);
        printf("\ningrese valor de y: ");
        scanf("%lf",&y);
        resultado = evaluarArbol(p_ecu->arbol_ecu, x, y);
        if (fabs(resultado) < 0.001) {
            printf("El x=%.2f e y=%.2f pertenecen a la ecuación\n", x, y);
        } else {
            printf("El x=%.2f e y=%.2f NO pertenecen a la ecuación\n", x, y);
        }
        op=pedir_opcion("SN","Quiere ingresar otros valores para X e Y? S/N: ");
    }
}

#include <math.h>
#include <stdio.h>

#define MAX_ITER 100
#define EPSILON 1e-6

// Busca la raíz f(x, y) = 0, donde se fija una variable y se calcula la otra
// Retorna 1 si se encuentra una solución, 0 si no.
// 'rango' define el intervalo: [-rango, rango]
int encontrar_variable_dependiente(nodo *arbol,char variable_fija,double valor_fijo,double *resultado,double rango){
    
    double min = -rango;
    double max = rango;

    double f_min = evaluarArbol(arbol,
        (variable_fija == 'x') ? valor_fijo : min,
        (variable_fija == 'y') ? valor_fijo : min);

    double f_max = evaluarArbol(arbol,
        (variable_fija == 'x') ? valor_fijo : max,
        (variable_fija == 'y') ? valor_fijo : max);

    if (f_min * f_max > 0) {
        // No hay cambio de signo en el intervalo: no hay raíz
        return 0;
    }

    // Método de bisección
    for (int i = 0; i < MAX_ITER; i++) {
        double medio = (min + max) / 2.0;
        double f_medio = evaluarArbol(arbol,
            (variable_fija == 'x') ? valor_fijo : medio,
            (variable_fija == 'y') ? valor_fijo : medio);

        if (fabs(f_medio) < EPSILON) {
            *resultado = medio;
            return 1;
        }

        double f_izq = evaluarArbol(arbol,
            (variable_fija == 'x') ? valor_fijo : min,
            (variable_fija == 'y') ? valor_fijo : min);

        if (f_izq * f_medio < 0)
            max = medio;
        else
            min = medio;
    }

    return 0; // no se encontró dentro de la precisión
}

/*
void GenerarTabla(ecuacion* ecu,size_t cecu){
    int i=0;
    char op='a', incognita;
    double x,y, resultado, paso, num;;
    ecuacion *p_ecu=ecu;
    mostrarecuaciones(ecu,cecu);
    printf("\nIngrese el numero que quiera evaluar: ");
    do{
        scanf("%d",&i);
        if(i<0 || i>cecu)
            printf("\nIngrese una ecuacion valida: ");
    }while(i<0 || i>cecu);

    p_ecu+=(i-1);

    printf("\nusted selecciono: %s", p_ecu->ecu);
    incognita=pedir_opcion("XY","\nQue incognita va a usar (X o Y): ");

    printf("\nFije el valor para %c: ", incognita);
    if(incognita=='X'){
        scanf("%lf",&x);
    }
    else{
        scanf("%lf",&y);
    }

    printf("\nQue paso va a tener la tabla: ");
    do{
        printf("%lf", &paso);
        if(paso<0);
            printf("\nIngrese un paso valido: ");
    }while(paso<0);

    
    
    if(incognita=='X'){
        num=x-5*paso;
    }
    else{
        num=y-5*paso;
    }
    for(i=0;i<10;i++){
        
        if(incognita=='X')
            resultado = evaluarArbol(p_ecu->arbol_ecu, x, num);
        else
            resultado = evaluarArbol(p_ecu->arbol_ecu, num, y);
        
        printf;



        num+=paso;
    }
}
*/
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
