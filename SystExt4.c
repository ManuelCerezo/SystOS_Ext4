//              PROYECTO OS II
//Simulacion de sistema de ficheros en la particion.bin
//1. Desfragmentacion del comando utilizado.
//2. Ejecucion de las funciones correspondientes para llevar a cabo el comando.
//3. Volver a preguntar algun comando.


#include <stdio.h>
#include "cabeceras.h"
#include <string.h>
#include <stdio_ext.h> 
#define LONGITUD_COMANDO 100
#define LONGITUD_ORDEN 10       
#define LONGITUD_ARGUMENTOS 20 //No es necesario +20 caracteres para definir un argumento.

//Funciones.
void actualizarParticion(EXT_SIMPLE_SUPERBLOCK *Psuperbloque, 
EXT_BYTE_MAPS *PByte_maps, EXT_BLQ_INODOS *Pext_blq_inodos,
EXT_ENTRADA_DIR *Pdirectorio, EXT_DATOS *Pmemdatos,EXT_DATOS *Pparticion);

void Copiar(EXT_ENTRADA_DIR *Pdirectorio, EXT_BLQ_INODOS *Pext_blq_inodos,
           EXT_BYTE_MAPS *PByte_maps, EXT_SIMPLE_SUPERBLOCK *Psuper_bloque,
           EXT_DATOS *Pparticion, char *Argumento1, char *Argumento2);

void Borrar(EXT_ENTRADA_DIR *Pdirectorio, EXT_BLQ_INODOS *Pbloq_inodos,
           EXT_BYTE_MAPS *Pbyte_map, EXT_SIMPLE_SUPERBLOCK *Psuper_bloque,
           char *Argumento1);

void imprimiFich(EXT_ENTRADA_DIR *Pdirectorio,EXT_BLQ_INODOS *Pext_blq_inodos,EXT_DATOS *Pmemdatos,char* Argumento1);
void frename(EXT_ENTRADA_DIR *Pdirectorio,char *Argumento1, char *Argumento2);
void imprimirDir(EXT_ENTRADA_DIR *Pdirectorio,EXT_BLQ_INODOS *Pext_blq_inodos);
void ImprimirByteMaps(EXT_BYTE_MAPS *Pbyte_map);
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
    
    int flag = 0;
    int leidos = 0;
    char comando[LONGITUD_COMANDO]; //buffer de comando.
    char Argumento1[LONGITUD_ARGUMENTOS];
    char Argumento2[LONGITUD_ARGUMENTOS];
    char orden[LONGITUD_ORDEN];



    FILE *pfile = NULL; //Puntero al fichero que contiene la particion
    if ((pfile = fopen("particion.bin", "r+b")) == NULL){ // "rb" -> Lectura solo del fichero binario
        printf ( " Error en la apertura\n ");
    }
    leidos = fread(&particion, SIZE_BLOQUE, MAX_BLOQUES_PARTICION, pfile); //Tenemos toda la particion dividida en la estructura de bloques.
    //Hacemos los cambios en la particion pero vamos guardandolo en el Pfile.
     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&particion[0], SIZE_BLOQUE);
     memcpy(&directorio,(EXT_ENTRADA_DIR *)&particion[3], SIZE_BLOQUE);
     memcpy(&ext_bytemaps,(EXT_BLQ_INODOS *)&particion[1], SIZE_BLOQUE);
     memcpy(&ext_blq_inodos,(EXT_BLQ_INODOS *)&particion[2], SIZE_BLOQUE);
     memcpy(&memdatos,(EXT_DATOS *)&particion[4],MAX_BLOQUES_DATOS*SIZE_BLOQUE);

     //actualizarParticion(&ext_superblock,&ext_bytemaps,&ext_blq_inodos,directorio,memdatos,particion) FUNCION ACTUALIZAR PARTICION
    
    // Seleccion de comandos: info, bytemaps,dir,rename,imprimir,remove,copy,salir
    //Ejecucion del programa-----------------

    printf("Sistema Archivos SysExt4\n"); 
    printf("Size of leidos: %d\n",leidos);
    do{
      //  ActualizarSuperBloque(&ext_superblock,particion);
        memset(orden, 0, LONGITUD_ORDEN);           //Limpieza de los comandos.
        memset(Argumento1, 0, LONGITUD_ARGUMENTOS);
        memset(Argumento2, 0, LONGITUD_ARGUMENTOS);
        memset(comando,0,LONGITUD_COMANDO);
        do{
            printf("\n>> ");
            scanf("%[^\n]s",comando);//[\n] indica la limitacion de lectura
            __fpurge(stdin);              //En linux utilizar -> __fpurge(stdin); para limpieza de buffer de teclado. necesario la libreria: <stdio_ext.h> 
        }while(comando[0]=='\0');
        
        desfracomando(comando,orden,Argumento1,Argumento2); //Funcion para desfragmentar el comando.

        if(strcmp(orden, "info")==0){ //Llamamos a la funcion correspondiente de cada uno.
            //printf("%s",orden); //Funcion de info:
            LeeSuperBloque(&ext_superblock);

            flag = 0;
        }else if(strcmp(orden, "bytemaps")==0){
            //printf("%s\n",orden); //Funcion de bytemaps
            //inicializarInodos(&ext_bytemaps);
            ImprimirByteMaps(&ext_bytemaps);

            flag = 0;
        
        }else if(strcmp(orden, "dir")==0){
            imprimirDir(directorio,&ext_blq_inodos);
            //printf("%s",orden);
            flag = 0;
        
        }else if(strcmp(orden, "rename")==0){
            frename(directorio,Argumento1,Argumento2);
            flag = 0;
        
        }else if(strcmp(orden, "imprimir")==0){
            //printf("%s",orden);
            imprimiFich(directorio,&ext_blq_inodos,particion,Argumento1);
            flag = 0;
        
        }else if(strcmp(orden, "remove")==0){
            Borrar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,Argumento1);
            flag = 0;
        
        }else if(strcmp(orden, "copy")==0){
            Copiar(directorio,&ext_blq_inodos,&ext_bytemaps,&ext_superblock,particion,Argumento1,Argumento2);
            flag = 0;
        
        }else if(strcmp(orden, "salir")==0){
            actualizarParticion(&ext_superblock,&ext_bytemaps,&ext_blq_inodos,directorio,memdatos,particion);
            fwrite(&particion[0],SIZE_BLOQUE,leidos,pfile);

            //memcpy(&pfile,(FILE*)&particion[0],(SIZE_BLOQUE*MAX_BLOQUES_PARTICION)); //Actualizamos el archivo -> .bin
            //Actualizar el archivo .bin
            fclose(pfile);
            flag = 1;
        }else{
            printf("ERROR: Comando ilegal [info,bytemaps,dir,rename,imprimir,remove,copy,salir]");
        }

        /*printf("\nLa orden es: %s\n",orden);
        printf("El argumento 1 es: %s\n",Argumento1);
        printf("El argumento 2 es: %s\n",Argumento2);*/

    }while(flag == 0);
    return 0;
}
void actualizarParticion(EXT_SIMPLE_SUPERBLOCK *Psuperbloque, 
EXT_BYTE_MAPS *PByte_maps, EXT_BLQ_INODOS *Pbloq_inodos,
EXT_ENTRADA_DIR *Pdirectorio, EXT_DATOS *Pmemdatos,EXT_DATOS *Pparticion){

    memcpy(&Pparticion[0],(EXT_DATOS *)&Psuperbloque, SIZE_BLOQUE);
    memcpy(&Pparticion[1],(EXT_DATOS *)&PByte_maps, SIZE_BLOQUE);
    memcpy(&Pparticion[2],(EXT_DATOS *)&Pbloq_inodos, SIZE_BLOQUE);
    memcpy(&Pparticion[3],(EXT_DATOS *)&Pdirectorio, SIZE_BLOQUE);
    //memcpy(&Pparticion[4],(EXT_DATOS *)&Pmemdatos, (SIZE_BLOQUE*MAX_BLOQUES_DATOS));
}

void Copiar(EXT_ENTRADA_DIR *Pdirectorio, EXT_BLQ_INODOS *Pext_blq_inodos,
           EXT_BYTE_MAPS *PByte_maps, EXT_SIMPLE_SUPERBLOCK *Psuper_bloque,
           EXT_DATOS *Pparticion, char *Argumento1, char *Argumento2){
               
    int i = 0;
    int n = 0;
    int j = 0;
    int cont = 0;
    int f = 0;
    unsigned short int flag = 0;
    unsigned short int inodoLibre;
    unsigned short bloqueLibre[MAX_NUMS_BLOQUE_INODO];
    unsigned int tamanio;
    unsigned short aux[MAX_NUMS_BLOQUE_INODO];

    for(i=1;i<MAX_FICHEROS;i++){
        if(Pdirectorio[i].dir_inodo != NULL_INODO){
            if((strcmp(Pdirectorio[i].dir_nfich, Argumento2)==0)){
                flag = 1;
            }
        }
    }
    if(Argumento2[0]!= '\0'){
        for(i=1;i<MAX_FICHEROS;i++){ 
            if(Pdirectorio[i].dir_inodo != NULL_INODO){
                f++;
                if(strcmp(Pdirectorio[i].dir_nfich, Argumento1)==0){
                    for(n=0;n<MAX_NUMS_BLOQUE_INODO;n++){                                                       
                        if(Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n] != NULL_BLOQUE){
                            tamanio = Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].size_fichero;
                            aux[n] = Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n];
                            j++;
                        }
                    }
                }
                else{
                    cont++;
                }
            } 
        }
        if(cont == f){
            printf("[ ERROR:  Fichero no existe]");
        }
        else if (flag == 1){
            printf("[ ERROR:  El Fichero ya existe]");
        }
        else{
            for(i=3;i<MAX_INODOS;i++){
               if(PByte_maps->bmap_inodos[i] == 0){
                    inodoLibre = i;
                    i=MAX_INODOS;
               }
            }
            n=0;
            for(i=4;i<MAX_BLOQUES_PARTICION;i++){
               if(PByte_maps->bmap_bloques[i] == 0){
                   bloqueLibre[n] = i;
                   n++;
                   if(n == j){
                    i = MAX_BLOQUES_PARTICION;
                   }
               }
            }
            for(i=1;i<MAX_FICHEROS;i++){
                if(Pdirectorio[i].dir_inodo == NULL_INODO){
                    strcpy(Pdirectorio[i].dir_nfich, Argumento2);
                    Pdirectorio[i].dir_inodo = inodoLibre;
                    Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].size_fichero = tamanio;
                    PByte_maps->bmap_inodos[Pdirectorio[i].dir_inodo] = 1;
                    for(n=0;n<MAX_NUMS_BLOQUE_INODO;n++){
                        if(n == j){
                            n = MAX_NUMS_BLOQUE_INODO;
                            i = MAX_FICHEROS;
                        }
                        else{
                            Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n] = bloqueLibre[n];
                        }
                    }
                } 
            }
            for(i=0;i<j;i++){
                PByte_maps->bmap_bloques[bloqueLibre[i]] = 1;
            }
            for(i=0;i<j;i++){
                //     memcpy(&ext_superblock,(EXT_SIMPLE_SUPERBLOCK *)&particion[0], SIZE_BLOQUE);
                memcpy(&Pparticion[bloqueLibre[i]],(EXT_DATOS *)&Pparticion[aux[i]],SIZE_BLOQUE);
            }
            Psuper_bloque->s_free_inodes_count -- ;
            Psuper_bloque->s_free_blocks_count = Psuper_bloque->s_free_blocks_count - j ;
        }
    }
    else{
        printf("\n ERROR: Escribe un nombre de fichero valido");
    }
      
}

void Borrar(EXT_ENTRADA_DIR *Pdirectorio, EXT_BLQ_INODOS *Pext_blq_inodos,
EXT_BYTE_MAPS *Pbyte_map, EXT_SIMPLE_SUPERBLOCK *Psuper_bloque,char *Argumento1){

    int i = 0;
    int n = 0;
    int j = 0;
    int cont = 0;
    int f = 0;
    int inodo;
    int Auxint[MAX_NUMS_BLOQUE_INODO];


    for(i=1;i<MAX_FICHEROS;i++){   
        if(Pdirectorio[i].dir_inodo != NULL_INODO){
            f++;
            if(strcmp(Pdirectorio[i].dir_nfich, Argumento1)==0){
                for(n=0;n<MAX_NUMS_BLOQUE_INODO;n++){                                                       
                    if(Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n] != NULL_BLOQUE){
                        Auxint[j] = Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n]; 
                        j++; 
                        Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n] = NULL_BLOQUE;
                    }
                }
                Pbyte_map->bmap_inodos[Pdirectorio[i].dir_inodo] = 0;
                Pdirectorio[i].dir_inodo = NULL_INODO;
            }
            else{
                cont++;
            }
        } 
    }
    if(cont == f){
        printf("[ERROR:] El Fichero no existe\n");
    }
    else{
        for(i=0;i<j;i++){
            Pbyte_map->bmap_bloques[Auxint[i]] = 0;
        }
        Psuper_bloque->s_free_inodes_count ++ ;
        Psuper_bloque->s_free_blocks_count = Psuper_bloque->s_free_blocks_count + j ;
    }  
}

void imprimiFich(EXT_ENTRADA_DIR *Pdirectorio,EXT_BLQ_INODOS *Pext_blq_inodos,EXT_DATOS *Pparticion,char* Argumento1){
    int i = 0;
    int n = 0;
    int j = 0;
    int cont = 0;
    int f = 0;
    char *Aux = NULL;
    int Auxint[MAX_NUMS_BLOQUE_INODO];

   for(i=1;i<MAX_FICHEROS;i++){   
        if(Pdirectorio[i].dir_inodo != NULL_INODO){
            f++;
            if(strcmp(Pdirectorio[i].dir_nfich, Argumento1)==0){
                for(n=0;n<MAX_NUMS_BLOQUE_INODO;n++){                                                       
                    if(Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n] != NULL_BLOQUE){
                        Auxint[j] = Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n];
                        //printf(" %d ",Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n]);
                        //printf("Auxint[%d] <- Bloque: %d\n",j,Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n]); 
                        j++; 
                    }
                }
            }
            else{
                cont++;
            }
        } 
    }
    if(cont == f){
        printf("[ERROR:] El Fichero no existe\n");
    }else{
        printf("\n");
        for(i=0;i<j;i++){
            printf("%s",Pparticion[Auxint[i]].dato);
        }
    }
}

void frename(EXT_ENTRADA_DIR *Pdirectorio,char *Argumento1, char *Argumento2){
    int i = 0;
    int flag = 0;
    int flag1 = 0;

    if(Argumento2[0]!= '\0'){
        for(i=1;i<MAX_FICHEROS;i++){
            if(Pdirectorio[i].dir_inodo != NULL_INODO){
                if((strcmp(Pdirectorio[i].dir_nfich, Argumento2)==0)){
                    flag = 1;
                }
                if((strcmp(Pdirectorio[i].dir_nfich, Argumento1)==0)){
                    flag1 = 1;
                }
            }
        }
        if(flag == 0){
            for(i=1;i<MAX_FICHEROS;i++){   
                if(Pdirectorio[i].dir_inodo != NULL_INODO){
                    if(strcmp(Pdirectorio[i].dir_nfich, Argumento1)==0){
                        strcpy(Pdirectorio[i].dir_nfich,Argumento2);
                    }
                }   
            }
        }
        if(flag1 == 0){
            printf("ERROR: Fichero %s no encontrado",Argumento1);
        }
        else if(flag == 1){
            printf("ERROR: El fichero: %s ya existe",Argumento2);
        }
    }
    else{
        printf("\n ERROR: Escribe un nombre de fichero valido");
    }
    
}

void imprimirDir(EXT_ENTRADA_DIR *Pdirectorio,EXT_BLQ_INODOS *Pext_blq_inodos){
    int i = 0;
    int n = 0;
    int j = 0;
    
   for(i=1;i<MAX_FICHEROS;i++){   
        if(Pdirectorio[i].dir_inodo != NULL_INODO){
            printf("%s  \t",Pdirectorio[i].dir_nfich);                                                    //nombre
            printf("Inodo: %d\t",Pdirectorio[i].dir_inodo);                                             // Inodo
            printf("Tamanio: %d\t",Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].size_fichero); //Tamanio
            printf("Bloques: ");
            for(n=0;n<MAX_NUMS_BLOQUE_INODO;n++){                                                       //MAX_NUMS_BLOQUE_INODO = 7    
                if(Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n] != NULL_BLOQUE){
                printf(" %d ",Pext_blq_inodos->blq_inodos[Pdirectorio[i].dir_inodo].i_nbloque[n]);      //bloques
                }
            }
            printf("\n");
       } 
    }

}

void ImprimirByteMaps(EXT_BYTE_MAPS *Pbyte_map){
    int n = 0;
    int i = 0;

    printf("Byte_Maps_Inodos: ");
    for(i=0;i<MAX_INODOS;i++){
        printf("%d ",Pbyte_map->bmap_inodos[i]);
    }
    printf("\nByte_Maps_Bloques [0-25]: ");
    for(i=0;i<25;i++){
        printf("%d ",Pbyte_map->bmap_bloques[i]);
    }
}

void LeeSuperBloque(EXT_SIMPLE_SUPERBLOCK *Psuper_bloque){ //posible funcion de actualizado de superbloque.

    printf("    -INFO-\n\n");
    printf("Tamanio Bloque: %d\n",Psuper_bloque->s_block_size);
    printf("Inodos Particion: %d\n",Psuper_bloque->s_inodes_count);
    printf("Inodos Libres: %d\n",Psuper_bloque->s_free_inodes_count);
    printf("Bloques Particion: %d\n",Psuper_bloque->s_blocks_count);
    printf("Bloques Libres: %d\n",Psuper_bloque->s_free_blocks_count);
    printf("Primer Bloque Datos: %d\n", PRIM_BLOQUE_DATOS);
    
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
