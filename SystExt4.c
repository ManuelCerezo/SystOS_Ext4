//              PROYECTO OS II
//Simulacion de sistema de ficheros en la particion.bin
//1. Desfragmentacion del comando utilizado.
//2. Ejecucion de las funciones correspondientes para llevar a cabo el comando.
//3. Volver a preguntar algun comando.


#include <stdio.h>
#include "cabeceras.h"
#include <string.h>
#define LONGITUD_COMANDO 100
#define LONGITUD_ORDEN 10       
#define LONGITUD_ARGUMENTOS 20 //No es necesario +20 caracteres para definir un argumento.

//Funciones.
void ActualizarSuperBloque(EXT_SIMPLE_SUPERBLOCK *Psuper_bloque,EXT_DATOS *particion);
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *Psuper_bloque);
void desfracomando(char*comando,char *oren,char *Argumento1,char *Argumento2);
int main(){
    //Definicion de las estructuras.
    EXT_SIMPLE_SUPERBLOCK ext_superblock;         //Definicion de superbloque
    EXT_BYTE_MAPS ext_bytemaps;                   //Definicion de la estructura de bytemaps [bloques e inodos]
    EXT_BLQ_INODOS ext_blq_inodos;                //Defincion de la estructura de inodos
    EXT_ENTRADA_DIR directorio[MAX_FICHEROS];     //Definicion de la estructura directorio
    EXT_DATOS memdatos[MAX_BLOQUES_DATOS];        //Definicion de la estructura datos libres
    EXT_DATOS particion[MAX_BLOQUES_PARTICION];   //Array de estructuras que contienen bloques de particion
    int entradadir;
    int grabardatos;
    int flag = 0;

    char comando[LONGITUD_COMANDO]; //buffer de comando.
    char Argumento1[LONGITUD_ARGUMENTOS];
    char Argumento2[LONGITUD_ARGUMENTOS];
    char orden[LONGITUD_ORDEN];

    FILE *pfile = NULL; //Puntero al fichero que contiene la particion
    if ((pfile = fopen("particion.bin", "r+b")) == NULL){ // "rb" -> Lectura solo del fichero binario
        printf ( " Error en la apertura\n ");
    }
    fread(&particion, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, pfile); //Tenemos toda la particion dividida en la estructura de bloques.
    //Hacemos los cambios en la particion pero vamos guardandolo en el Pfile.

    //instanciamos los primeros 4 bloques dentro de la particion
    memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&particion[0], SIZE_BLOQUE);
    memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&particion[1], SIZE_BLOQUE);
    memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&particion[2], SIZE_BLOQUE);
    memcpy(&directorio,(EXT_ENTRADA_DIR *)&particion[3], SIZE_BLOQUE);
    memcpy(&memdatos,(EXT_DATOS *)&particion[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);
    
    
    // Seleccion de comandos: info, bytemaps,dir,rename,imprimir,remove,copy,salir
    //Ejecucion del programa-----------------
    printf("Sistema Archivos SysExt4\n"); 
    do{
        ActualizarSuperBloque(&ext_superblock,particion);
        memset(orden, 0, LONGITUD_ORDEN);           //Limpieza de los comandos.
        memset(Argumento1, 0, LONGITUD_ARGUMENTOS);
        memset(Argumento2, 0, LONGITUD_ARGUMENTOS);
        
        printf("\n>> ");scanf("%[^\n]s",comando);fpurge(stdin);  //[\n] indica la limitacion de lectura
        desfracomando(comando,orden,Argumento1,Argumento2); //Funcion para desfragmentar el comando.

        if(strcmp(orden, "info")==0){ //Llamamos a la funcion correspondiente de cada uno.
            //printf("%s",orden); //Funcion de info:
            LeeSuperBloque(&ext_superblock);
            flag = 0;
        }else if(strcmp(orden, "bytemaps")==0){
            printf("%s",orden); //Funcion de bytemaps
            flag = 0;
        
        }else if(strcmp(orden, "dir")==0){
            printf("%s",orden);
            flag = 0;
        
        }else if(strcmp(orden, "rename")==0){
            printf("%s",orden);
            flag = 0;
        
        }else if(strcmp(orden, "imprimir")==0){
            printf("%s",orden);
            flag = 0;
        
        }else if(strcmp(orden, "remove")==0){
            printf("%s",orden);
            flag = 0;
        
        }else if(strcmp(orden, "copy")==0){
            printf("%s",orden);
            flag = 0;
        
        }else if(strcmp(orden, "salir")==0){
            flag = 1;
        }else{
            //printf("%s\n",orden);
            printf("ERROR: Comando ilegal [info,bytemaps,dir,rename,imprimir,remove,copy,salir]");
        }

        /*printf("\nLa orden es: %s\n",orden);
        printf("El argumento 1 es: %s\n",Argumento1);
        printf("El argumento 2 es: %s\n",Argumento2);*/
    }while(flag == 0);
    //---------------------------------------
    return 0;
}
void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *Psuper_bloque){ //posible funcion de actualizado de superbloque.
    printf("    -INFO-\n\n");
    printf("Tamaño Bloque: %d\n",Psuper_bloque->s_block_size);
    printf("Inodos Particion: %d\n",Psuper_bloque->s_inodes_count);
    printf("Inodos Libres: %d\n",Psuper_bloque->s_free_inodes_count);
    printf("Bloques Particion: %d\n",Psuper_bloque->s_blocks_count);
    printf("Bloques Libres: %d\n",Psuper_bloque->s_free_blocks_count);
    printf("Primer Bloque Datos: %d\n",Psuper_bloque->s_relleno);
}
void ActualizarSuperBloque(EXT_SIMPLE_SUPERBLOCK *Psuper_bloque,EXT_DATOS *particion){
    //Preguntar como se puede actualizar el superbloque    
    int i = 0;
    int j = 0;
    for(j=0; j<MAX_BLOQUES_PARTICION;j++){
        for(i=0; i<SIZE_BLOQUE;i++){
            particion[i].dato[j]; //Recorrer todos los bytes de la particion.
            //¿Como actualizo el superbloque?
        }
    } 
}

void desfracomando(char*comando,char *orden,char *Argumento1,char *Argumento2){ //Desfragmentacion del comando
    int i = 0;
    int n = 0;
    int longitud = 0;

    char delimitador[] = " ";
    char *token = strtok(comando,delimitador);
    if(token != NULL){
        while(token != NULL){
            i=0;
            while(token[i++]!='\0');
            longitud = i - 1;
            if(n == 0){
                for(i=0;i<longitud;i++){
                    orden[i] = token[i];
                }
            }
            if( n == 1){
                for(i=0;i<longitud;i++){
                    Argumento1[i] = token[i];
                }
            }
            if(n == 2){
                for(i=0;i<longitud;i++){
                    Argumento2[i] = token[i];
                }
            }
            n++;
            token = strtok(NULL, delimitador);
        }
    }
}
