#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "Vector.h"
#include "calculadora.h"
#include <string.h>
#include "Archivos.h"
#include "calculadora_bmp.h"


#define TAM_ECUACION 512
#define MAX_ECUAC 10

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

void EnterParaSalir() {
    printf("\n\nPresione enter para continuar...");
    getchar();
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

        limpiarBuffer();  // limpia el resto de la linea
        op = Amayus(op);

        if (!strchr(validas, op)) {
            printf("Ingrese una opcion valida: ");
        }
    } while (!strchr(validas, op));

    return op;
}

char menu(){
    char letra;

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


    do {
        letra = getchar();            // Lee un solo caracter
        limpiarBuffer();             // Descarta el resto de la linea
        letra = Amayus(letra);       // Convertir a mayuscula si hace falta

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
    int band_error=ECUACION_OK, cont_par_abi=0, cont_par_cerr=0, cont_igual=0,cont_x=0,cont_y=0;
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
            if(*p_ecu=='x')
                cont_x++;
            else if(*p_ecu=='y')
                cont_y++;
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

    if(cont_x ==0 || cont_y==0){
        band_error=NO_ES_ECUACION;
    }


    if(cont_par_abi != cont_par_cerr)
        band_error=PARENTESIS_NO_CERRADO;

    if(band_error){
        return band_error;
    }

return ECUACION_OK;
}

void Amin(char* ecu) {
    char *p_ecu = ecu;
    while (*p_ecu) {
        *p_ecu = tolower(*p_ecu);
        p_ecu++;
    }
}

void introducirecuacion(char* ecu) {
    EstadoEcuacion estado;
    char confirmacion;
    do {
        estado=1;
        printf("Introduzca una ecuacion: ");
        fgets(ecu, TAM_ECU, stdin);
        quitarenter(ecu);
        Amin(ecu);
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
                ayuda_ecuacion(); // Muestra ayuda si hay error
                printf("Ecuacion invalida. Vuelva a intentar.\n");
            }
        }

    } while (estado != ECUACION_OK);
}

void mostrarecuaciones(ecuacion *ecuac, size_t ce, int print){
    if(print){
        limpiarPantalla();
        printf("\n--- Ecuaciones Cargadas ---\n");
    }
    if(ce==0){
        printf("\nNo hay ecuaciones cargadas.\n");
        return;
    }
    ecuacion *p_ecuac=ecuac;
    for(int i=0; i<ce; i++){
        printf("\nEcuacion %d: %s",i+1, p_ecuac->ecu);
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
    //INGRESO ==0 : CARGAR POR ARCHIVO
    //INGRESO ==2 : CAMBIAR ECUACION
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

    if(ingreso!=2 && *cecu<MAX_ECUAC){
        p_ecuac+=*cecu;
        (*cecu)++;
    }
    else{
        if(ingreso!=2)
            printf("Memoria llena. Que ecuacion desea reemplazar?: ");
        else
            printf("Que ecuacion desea reemplazar?: ");

        printf("\necuacion 0: %s","CANCELAR");
        mostrarecuaciones(ecuac,*cecu,0);
        do{
            if(num<0 || num>10)
                printf("\nIngrese un numero valido:");
            scanf("%d", &num);
            limpiarBuffer();
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

    insertarmultiplicaciones(&ecuac_token); //Ingresamos las multiplicaciones implicitas

    postfija(&ecuac_token); //PASAMOS EL TOKEN A POSTFIJA
    CrearArbol(&ecuac_token,&(p_ecuac->arbol_ecu)); //CREAMOS EL ARBOL CON LOS DATOS DE LA ECUACION

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
                case '/':
                    if (fabs(der) < 1e-9) // Si el denominador es muy cercano a cero
                        return izq / 1e-9;
                    else
                        return izq / der;
                case '^': return pow(izq, der);
                default: return 0; // operador desconocido
            }
        }
    }

    return 0; // caso no reconocido
}

void graficar(ecuacion* ecuaciones, size_t cecu, int* indice){
    double x_min, x_max, y_min, y_max;
    limpiarPantalla();
    printf("\n--- GRAFICAR ECUACION ---\n");
    mostrarecuaciones(ecuaciones, cecu,0);
    printf("\nIngrese el numero de ecuacion a graficar: ");
    scanf("%d", indice);
    limpiarBuffer();
    if(*indice < 1 || *indice > cecu) {
        puts("Indice invalido.");
        return;
    }

    printf("Ingrese el limite inferior de X: ");
    scanf("%lf", &x_min);
    printf("Ingrese el limite superior de X: ");
    do{
        scanf("%lf", &x_max);
        if(x_max <=x_min) {
            printf("El limite superior debe ser mayor que el limite inferior. Intente de nuevo: ");
        }
    }while(x_max <= x_min);

    printf("Ingrese el limite inferior de Y: ");
    scanf("%lf", &y_min);
    printf("Ingrese el limite superior de Y: ");
    do{
        scanf("%lf", &y_max);
        if(y_max <= y_min) {
            printf("El limite superior debe ser mayor que el limite inferior. Intente de nuevo: ");
        }
    }while(y_max <= y_min);
    limpiarBuffer();

    graficar_ecuacion(ecuaciones, *indice-1, x_min, x_max, y_min, y_max);
    graficar_ecuacion_consola(ecuaciones, *indice-1, x_min, x_max, y_min, y_max);
    puts("Grafico guardado como 'grafico.bmp'.");
    EnterParaSalir();
}

double buscar_raiz(nodo* arbol, double fijo, double min, double max, double tolerancia, int max_iter, int buscar_y) {
    double a = min, b = max, fa, fb, fm, m;
    int iter = 0;

    fa = buscar_y ? evaluarArbol(arbol, fijo, a) : evaluarArbol(arbol, a, fijo);
    fb = buscar_y ? evaluarArbol(arbol, fijo, b) : evaluarArbol(arbol, b, fijo);

    if (fa * fb > 0) return NAN;

    while ((b - a) > tolerancia && iter < max_iter) {
        m = (a + b) / 2.0;
        fm = buscar_y ? evaluarArbol(arbol, fijo, m) : evaluarArbol(arbol, m, fijo);

        if (fa * fm < 0) {
            b = m; fb = fm;
        } else {
            a = m; fa = fm;
        }
        iter++;
    }
    return (a + b) / 2.0;
}

int buscar_raiz_multi(nodo* arbol, double fijo, double min, double max, double tolerancia, int max_iter, double* soluciones, int max_sol, int buscar_y) {
    double paso = (max - min) / 1000.0;
    int n_sol = 0;
    double a = min, b = a + paso;
    double fa = buscar_y ? evaluarArbol(arbol, fijo, a) : evaluarArbol(arbol, a, fijo);
    double *p_sol = soluciones;

    while (b <= max && n_sol < max_sol) {
        double fb = buscar_y ? evaluarArbol(arbol, fijo, b) : evaluarArbol(arbol, b, fijo);

        // Raiz exacta en a
        if (fabs(fa) < tolerancia) {
            int duplicado = 0;
            for (double *p_check = soluciones; p_check < p_sol; p_check++) {
                if (fabs(*p_check - a) < tolerancia * 2)
                    duplicado = 1;
            }
            if (!duplicado) {
                *p_sol = a;
                p_sol++;
                n_sol++;
            }
        }
        // Raiz exacta en b
        if (fabs(fb) < tolerancia) {
            int duplicado = 0;
            for (double *p_check = soluciones; p_check < p_sol; p_check++) {
                if (fabs(*p_check - b) < tolerancia * 2)
                    duplicado = 1;
            }
            if (!duplicado) {
                *p_sol = b;
                p_sol++;
                n_sol++;
            }
        }
        // Cambio de signo
        if (fa * fb < 0) {
            double raiz = buscar_raiz(arbol, fijo, a, b, tolerancia, max_iter, buscar_y);
            int duplicado = 0;
            for (double *p_check = soluciones; p_check < p_sol; p_check++) {
                if (fabs(*p_check - raiz) < tolerancia * 2)
                    duplicado = 1;
            }
            if (!duplicado) {
                *p_sol = raiz;
                p_sol++;
                n_sol++;
            }
        }
        a += paso;
        b += paso;
        fa = fb;
    }
    return n_sol;
}

double leer_fraccion_o_decimal(const char* prompt) { //1/2=0.5
    char buffer[64];
    double resultado = 0.0;
    int valido = 0;
    while (!valido) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        char* barra = strchr(buffer, '/');
        if (barra) {
            // Es una fracción
            *barra = '\0';
            double num = atof(buffer);
            double den = atof(barra + 1);
            if (den != 0) {
                resultado = num / den;
                valido = 1;
            } else {
                printf("Denominador no puede ser cero.\n");
            }
        } else {
            // Es decimal
            resultado = atof(buffer);
            valido = 1;
        }
    }
    return resultado;
}
void TablaAlrededorDeValor(ecuacion *ecu, size_t cecu) {
    int i = 0;
    double valor, paso = 1.0;
    int variable;
    ecuacion *p_ecu = ecu;

    limpiarPantalla();
    printf("\n--- TABLA DE VALORES ---\n");

    mostrarecuaciones(ecu, cecu,0);
    printf("\nIngrese el numero de ecuacion que quiera evaluar: ");
    do {
        scanf("%d", &i);
        limpiarBuffer();
        if (i < 1 || i > cecu)
            printf("\nIngrese una ecuacion valida: ");
    } while (i < 1 || i > cecu);

    p_ecu += (i - 1);
    printf("\nUsted selecciono: %s\n", p_ecu->ecu);

    variable = pedir_opcion("XY", "¿Sobre que variable desea generar la tabla? (X/Y): ");

    valor = leer_fraccion_o_decimal("Ingrese el valor central para la variable: ");
    paso = leer_fraccion_o_decimal("Ingrese el paso: ");
    if (paso <= 0) paso = 1;

    limpiarPantalla();
    printf("\n--- TABLA DE VALORES ---\n");
    if (variable == 'X') {
        printf("\nTabla de valores para X de la funcion %s\nen el rango [%.2f, %.2f]:\n",p_ecu->ecu, valor - 5 * paso, valor + 5 * paso);
        printf("X\tY (tal que f(x,y)=0)\n");
        int j;
        for (j = -5; j <= 5; j++) {
            double x = valor + j * paso;
            double soluciones[10];
            double *p_sol = soluciones;
            int n_sol = buscar_raiz_multi(p_ecu->arbol_ecu, x, -100, 100, 1e-6, 100, soluciones, 10, 1);
            printf("%.6f\t", x);
            if (n_sol == 0)
                printf("No hay solucion\n");
            else {
                double *p_fin = soluciones + n_sol;
                while (p_sol < p_fin) {
                    printf("%.6f", *p_sol);
                    if (p_sol < p_fin - 1)
                        printf(" | ");
                    p_sol++;
                }
                printf("\n");
            }
        }
    } else {
        printf("\nTabla de valores para Y en el rango [%.2f, %.2f]:\n", valor - 5 * paso, valor + 5 * paso);
        printf("Y\tX (tal que f(x,y)=0)\n");
        int j;
        for (j = -5; j <= 5; j++) {
            double y = valor + j * paso;
            double soluciones[10];
            double *p_sol = soluciones;
            int n_sol = buscar_raiz_multi(p_ecu->arbol_ecu, y, -100, 100, 1e-6, 100, soluciones, 10, 0);
            printf("%.6f\t", y);
            if (n_sol == 0)
                printf("No hay solucion\n");
            else {
                double *p_fin = soluciones + n_sol;
                while (p_sol < p_fin) {
                    printf("%.6f", *p_sol);
                    if (p_sol < p_fin - 1)
                        printf(" | ");
                    p_sol++;
                }
                printf("\n");
            }
        }
    }
}

void BuscarRaices(ecuacion *ecu, size_t cecu) {
    int i = 0;
    int variable;
    ecuacion *p_ecu = ecu;
    double soluciones[20];
    double *p_sol, *p_fin;
    int n_sol;
    limpiarPantalla();
    printf("\n--- BUSCAR RAICES ---\n");

    mostrarecuaciones(ecu, cecu, 0);
    printf("\nIngrese el numero de ecuacion que quiera analizar: ");
    do {
        scanf("%d", &i);
        if (i < 1 || i > cecu)
            printf("\nIngrese una ecuacion valida: ");
    } while (i < 1 || i > cecu);

    p_ecu += (i - 1);
    printf("\nUsted selecciono: %s\n", p_ecu->ecu);

    variable = pedir_opcion("XY", "¿Buscar raices respecto a que variable? (X/Y): ");

    if (variable == 'X') {
        n_sol = buscar_raiz_multi(p_ecu->arbol_ecu, 0, -100, 100, 1e-6, 100, soluciones, 20, 0);
        printf("\nRaices para X (f(x,0)=0):\n");
        if (n_sol == 0)
            printf("No se encontraron raices reales en el rango.\n");
        else {
            p_sol = soluciones;
            p_fin = soluciones + n_sol;
            while (p_sol < p_fin) {
                printf("x = %.6f\n", *p_sol);
                p_sol++;
            }
        }
    } else {
        n_sol = buscar_raiz_multi(p_ecu->arbol_ecu, 0, -100, 100, 1e-6, 100, soluciones, 20, 1);
        printf("\nRaices para Y (f(0,y)=0):\n");
        if (n_sol == 0)
            printf("No se encontraron raices reales en el rango.\n");
        else {
            p_sol = soluciones;
            p_fin = soluciones + n_sol;
            while (p_sol < p_fin) {
                printf("y = %.6f\n", *p_sol);
                p_sol++;
            }
        }
    }
}
void EvaluarListaDeValores(ecuacion *ecu, size_t cecu) {
    int i = 0;
    int variable;
    ecuacion *p_ecu = ecu;
    double valores[100];
    double *p_val = valores;
    char buffer[64], *p_buffer=buffer;

    limpiarPantalla();
    printf("\n--- TABLA DE VALORES (LISTA PERSONALIZADA) ---\n");

    mostrarecuaciones(ecu, cecu, 0);
    printf("\nIngrese el numero de ecuacion que quiera evaluar: ");
    do {
        scanf("%d", &i);
        limpiarBuffer();
        if (i < 1 || i > cecu)
            printf("\nIngrese una ecuacion valida: ");
    } while (i < 1 || i > cecu);

    p_ecu += (i - 1);
    printf("\nUsted selecciono: %s\n", p_ecu->ecu);

    variable = pedir_opcion("XY", "¿Sobre que variable desea ingresar valores? (X/Y): ");

    printf("Ingrese los valores para %c (uno por linea, puede usar fracciones, termine con X):\n", variable);
    while (p_val < valores + 100) {
        printf("%c[%ld]: ", variable, (long)(p_val - valores) + 1);
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        if (*p_buffer == 'X' || *p_buffer == 'x') break;
        buffer[strcspn(buffer, "\n")] = 0;
        if (strlen(buffer) == 0) continue;
        char* barra = strchr(buffer, '/');
        double valor;
        if (barra) {
            *barra = '\0';
            double num = atof(buffer);
            double den = atof(barra + 1);
            if (den == 0) {
                printf("Denominador no puede ser cero.\n");
                continue;
            }
            valor = num / den;
        } else {
            valor = atof(buffer);
        }
        *p_val = valor;
        p_val++;
    }

    limpiarPantalla();
    printf("\n--- TABLA DE VALORES ---\n");
    if (variable == 'X') {
        printf("\nTabla de valores para X de la funcion %s (f(x,y)=0):\n", p_ecu->ecu);
        printf("X\tY (tal que f(x,y)=0)\n");
        double *p_iter = valores;
        while (p_iter < p_val) {
            double x = *p_iter;
            double soluciones[10];
            double *p_sol = soluciones;
            int n_sol = buscar_raiz_multi(p_ecu->arbol_ecu, x, -100, 100, 1e-6, 100, soluciones, 10, 1);
            printf("%.6f\t", x);
            int printed = 0;
            double *p_fin_sol = soluciones + n_sol;
            while (p_sol < p_fin_sol) {
                double y = *p_sol;
                double fxy = evaluarArbol(p_ecu->arbol_ecu, x, y);
                if (!isnan(fxy) && !isinf(fxy) && fabs(fxy) < 1e-4) {
                    if (printed)
                        printf(" | ");
                    printf("%.6f", y);
                    printed = 1;
                }
                p_sol++;
            }
            if (!printed)
                printf("No hay solucion");
            printf("\n");
            p_iter++;
        }
    } else {
        printf("\nTabla de valores para Y de la funcion %s (f(x,y)=0):\n", p_ecu->ecu);
        printf("Y\tX (tal que f(x,y)=0)\n");
        double *p_iter = valores;
        while (p_iter < p_val) {
            double y = *p_iter;
            double soluciones[10];
            double *p_sol = soluciones;
            int n_sol = buscar_raiz_multi(p_ecu->arbol_ecu, y, -100, 100, 1e-6, 100, soluciones, 10, 0);
            printf("%.6f\t", y);
            int printed = 0;
            double *p_fin_sol = soluciones + n_sol;
            while (p_sol < p_fin_sol) {
                double x = *p_sol;
                double fxy = evaluarArbol(p_ecu->arbol_ecu, x, y);
                if (!isnan(fxy) && !isinf(fxy) && fabs(fxy) < 1e-4) {
                    if (printed)
                        printf(" | ");
                    printf("%.6f", x);
                    printed = 1;
                }
                p_sol++;
            }
            if (!printed)
                printf("No hay solucion");
            printf("\n");
            p_iter++;
        }
    }
    EnterParaSalir();
}

void ayuda_ecuacion(){
    puts("  Limitaciones y reglas para las ecuaciones:");
    puts("    - Solo se aceptan las variables X e Y (mayusculas o minusculas).");
    puts("    - Operadores permitidos: +, -, *, /, ^, =.");
    puts("    - Debe haber al menos una X y una Y.");
    puts("    - No puede haber division por cero.");
    puts("    - Las potencias deben escribirse con '^' y parentesis, por ejemplo: x^(2).");
    puts("    - Las raices se escriben como potencias fraccionarias, por ejemplo: y^(1/2).");
    puts("");
    puts("Ejemplo valido: x^2 + y^2 = 1");
}

void menu_ayuda_interactivo() {
    char op;
    do {
        limpiarPantalla();
        printf("\n--- MENU DE AYUDA ---\n");
        printf("[A] - Escribir ecuacion\n");
        printf("[B] - Ver ecuaciones\n");
        printf("[C] - Guardar y reiniciar\n");
        printf("[D] - Leer ecuaciones\n");
        printf("[E] - Borrar ecuaciones guardadas\n");
        printf("[F] - Resolver ecuacion\n");
        printf("[G] - Graficar ecuacion\n");
        printf("[X] - Volver al menu principal\n");
        printf("Seleccione una opcion para ver la ayuda: ");

        op = getchar();
        limpiarBuffer();
        op = Amayus(op);

        limpiarPantalla();
        switch(op) {
            case 'A':
                puts("Escribir ecuacion:");
                puts("  Permite ingresar una nueva ecuacion o modificar una existente.");
                ayuda_ecuacion();
                break;
            case 'B':
                puts("Ver ecuaciones:");
                puts("  Muestra todas las ecuaciones actualmente cargadas en memoria.");
                break;
            case 'C':
                puts("Guardar y reiniciar:");
                puts("  Guarda las ecuaciones actuales en un archivo y limpia la memoria.");
                puts("  Limite: solo se pueden guardar hasta 10 archivos diferentes.");
                break;
            case 'D':
                puts("Leer ecuaciones:");
                puts("  Permite cargar ecuaciones previamente guardadas desde un archivo.");
                break;
            case 'E':
                puts("Borrar ecuaciones guardadas:");
                puts("  Elimina todos los archivos y ecuaciones guardadas.");
                break;
            case 'F': {
                char subop;
                do {
                    limpiarPantalla();
                    puts("Resolver ecuacion:");
                    puts("  Permite trabajar con una ecuacion seleccionada de las cargadas en memoria.");
                    puts("  Las funciones estan acotadas de [-100;100] por lo que podria ser indefinido el resultado mayor a ese intervalo.");
                    puts("  Opciones disponibles:");
                    puts("    [A] Evaluar la ecuacion para valores dados de X e Y");
                    puts("    [B] Generar una tabla de valores");
                    puts("    [C] Buscar raices respecto a una variable");
                    puts("    [X] Volver al menu de ayuda");
                    printf("Seleccione una opcion para ver mas detalles: ");
                    subop = getchar();
                    limpiarBuffer();
                    subop = Amayus(subop);

                    limpiarPantalla();
                    switch(subop) {
                        case 'A':
                            puts("Evaluar la ecuacion para valores dados de X e Y:");
                            puts("  - El programa le pedira que ingrese valores numericos para X e Y.");
                            puts("  - Luego calculara el resultado de la ecuacion con esos valores.");
                            puts("  - Si la ecuacion es del tipo 'y = ...', puede dejar Y en blanco para que el programa la calcule.");
                            puts("  - Si la ecuacion es del tipo 'x = ...', puede dejar X en blanco para que el programa la calcule.");
                            puts("  - Si la ecuacion no es explicita para una variable, debe ingresar ambos valores.");
                            break;
                        case 'B':
                            puts("Generar una tabla de valores:");
                            puts("  - El programa le pedira que elija la variable independiente (X o Y).");
                            puts("  - Luego debe ingresar el rango (minimo y maximo) y el paso para esa variable.");
                            puts("  - El programa calculara y mostrara una tabla con los valores correspondientes.");
                            puts("  - util para ver como varia la ecuacion en un intervalo.");
                            break;
                        case 'C':
                            puts("Buscar raices respecto a una variable:");
                            puts("  - El programa le pedira que elija la variable respecto a la cual buscar la raiz (X o Y).");
                            puts("  - Debe ingresar el rango de busqueda.");
                            puts("  - El programa intentara encontrar un valor de la variable seleccionada para el cual la ecuacion se cumple (raiz).");
                            puts("  - Se utiliza un metodo numerico (biseccion o similar).");
                            break;
                        case 'X':
                            puts("Volviendo al menu de ayuda...");
                            break;
                        default:
                            puts("Opcion no valida. Intente nuevamente.");
                    }
                    if(subop != 'X') {
                        EnterParaSalir();
                    }
                } while(subop != 'X');
                break;
            }

            case 'G':
                puts("Graficar ecuacion:");
                puts("  Permite graficar la ecuacion seleccionada en un rango dado para X e Y.");
                puts("  El grafico se guarda como 'grafico.bmp' y tambien se muestra en consola.");
                break;
            case 'X':
                puts("Volviendo al menu principal...");
                break;
            default:
                puts("Opcion no valida. Intente nuevamente.");
        }
        if(op != 'X') {
            EnterParaSalir();
        }
    } while(op != 'X');
}
