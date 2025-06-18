#include "Archivos.h"
#include "calculadora.h"
#include <string.h>


#define NOM_ARCH 50

void quitarenter(char*p){
    while (*p && *p != '\n')
        p++;
        
    if (*p == '\n')
        *p = '\0';
}

int contarArchivos(int *band){
    FILE* pf;
    char buffer[NOM_ARCH];
    pf=fopen("NombresArchivos.txt","rt");
    if(!pf){
        return 1;
    }
    while(fgets(buffer,NOM_ARCH,pf)){
        (*band)++;
    }
    return 0;
}

int GuardaArchivos(ecuacion* ecu, size_t ce, int* band){
    char nomArch[NOM_ARCH], nom[NOM_ARCH];
    int i=0, nombandera=1;
    FILE* pf1, *pf;

    printf("\nElija el nombre con el que se va a guardar el archivo: ");
    
    fgets(nomArch,NOM_ARCH, stdin);
    quitarenter(nomArch);
    strcpy(nomArch,strcat(nomArch,".txt"));
    
    pf = fopen(nomArch, "wt");

    if(!pf){
        printf("No se pudo crear el archivo.\n");
        return 1;
    }

    ecuacion *p_ecu = ecu;
    for(i=0 ; i <ce ; i++){
        fprintf(pf,p_ecu->ecu, p_ecu);
        fprintf(pf,"\n");
        p_ecu++;
    }
    
    pf1= fopen("NombresArchivos.txt", "a+t");
    if(!pf1){
        printf("No se pudo crear el archivo de nombres.\n");
        return 1;
    }
    rewind(pf1);
    
    while(nombandera && fgets(nom, NOM_ARCH, pf1)){
        quitarenter(nom);

        if((strcmp(nom,nomArch))==0){
            nombandera=0;
        }
    }

    if(nombandera){
        (*band)++;
        fprintf(pf1,nomArch);
        fprintf(pf1,"\n");
    }   
    
    fclose(pf);
    fclose(pf1);

    
return 0; 
}

int CargarArchivo(ecuacion* ecu,size_t *cecu){
    FILE *pf;
    int i=0, band_arch=1;
    char lineaarch[TAM_ECU], lineausuario[TAM_ECU];
    pf=fopen("NombresArchivos.txt","rt");
     if(!pf){
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    while(fgets(lineaarch,TAM_ECU,pf)){
        printf("%d) %s\n",i,lineaarch);
        i++;
    }
    rewind(pf);
    char *p_lineausuario=lineausuario;
    printf("Que archivo desea cargar en el programa? (sin la extension .txt) ($ para cancelar): ");
    do{
        fgets(lineausuario,TAM_ECU, stdin);
        quitarenter(lineausuario);
        strcpy(lineausuario,strcat(lineausuario, ".txt"));
        
        while(*p_lineausuario!='$' && band_arch && fgets(lineaarch,TAM_ECU,pf)){
            quitarenter(lineaarch);
            if(!strcmp(lineausuario,lineaarch)){ 
                band_arch=0;
            }
        }
        if(*p_lineausuario!='$' && band_arch){
            printf("No se encontro un archivo con ese nombre, intente nuevamente: ");
            rewind(pf);
        }
    }while(*p_lineausuario!='$' && band_arch);
    
    fclose(pf);
    
    if(*p_lineausuario!='$'){
        pf = fopen(lineausuario,"rt");

        borrarecuacion(ecu, cecu);
        
        while(fgets(lineaarch,TAM_ECU,pf))
            ingresarecu(ecu,cecu,0,lineaarch);
    }
    puts("Archivo cargado correctamente.");
    fclose(pf);

return 0;
}

int borrarArchivos(){
    FILE *pf_arch;
    char linea[TAM_ECU];
    pf_arch=fopen("NombresArchivos.txt","rt");
    if(!pf_arch){
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }
    while(fgets(linea,TAM_ECU,pf_arch)){
        quitarenter(linea);
        remove(linea);
    }
    fclose(pf_arch);
    remove("NombresArchivos.txt");

    return 0;
}
