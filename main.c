/**
 * Idris Raïs-Ali
 * version 1
 * 15 mai 2020
 */


#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include"option.h"
#include"aes.h"
#define CHUNK 16


//AES Param for 256-bits Key
int Nb = 4;
int Nk = 8;
int Nr = 14;

//Others extern param
int VerboseMode = 0;
//int createAAD = 0;
//int AADsize;


int main(int argc, char* argv[]){
    //int VerboseMode = 0;
    int mode = 0;
    int decrypt = 0;
    int taille = 1;
    uchar *key = NULL;

    //options
    char path[256];
    strcpy(path, argv[argc-1]);
    key = options(argc,argv,&taille,&decrypt,&mode,&VerboseMode,key);
    info("La clé de chiffrement est:");
    for(int i = 0; i < taille; i++){
        info("%4x", key[i]);
    }
    info("\n");
    info("KEY SIZE = %i bits \n", 8*taille);

    //File to encrypt
    FILE *file = fopen(path, "r");


    //AES param depending of key size
    if(taille == 16){
        Nk = 4;
        Nr = 10;
        info("Number of rounds: %i\n", Nr);
    }
    else if(taille == 24){
        Nk = 6;
        Nr = 12;
        info("Number of rounds: %i\n", Nr);
    }

    //Runtime
    float ecart;
    clock_t  begin;
    if (VerboseMode == 1){
        begin = clock();
    }

    //Key Round 
    uchar **word = calloc(Nb*(Nr+1),sizeof(uchar*));
    for (int i=0;i<Nb*(Nr+1);i++){
        word[i] = calloc(Nb,sizeof(uchar));
    }
    KeySchedule(key,word);

    //Create an AAD
    /*
    if(createAAD == 1){
        FILE* AAD_file = fopen("AAD","w+");
        uchar buffer[CHUNK];
        for(int i=0; i<AADsize; i++){
            vector_gen2(buffer,CHUNK);
            write_block(buffer,AAD_file);
        }
        fclose(AAD_file);
    }
    */

    // mode ECB 
    if(mode == 0){
        info("Mode: ECB. ");
        if(decrypt == 0){
            info("Encryption. \n");
            FILE *fileout = fopen("cipher","w+");
            Encrypt_ECB(file,word,fileout);
            fclose(fileout);
        }
        else{
            info("Decryption. \n");
            FILE *fileout = fopen("decipher","w+");
            Decrypt_ECB(file,word,fileout);
            fclose(fileout);
        }
    }

    //mode CBC
    else if(mode == 1){
        uchar InitVector[16];
        info("Mode: CBC. ");
        if(decrypt == 0){
            info("Encryption. \n");
            vector_gen2(InitVector,16);
            info("Initialisation Vector:");
            for(int i = 0; i < 16; i++){
                info("%4x", InitVector[i]);
            }
            info("\n");
            FILE *initfile = fopen("iv","w+");
            if(initfile){
                for(int i = 0; i<16; i++)
                {
                    if(InitVector[i]<16){
                        fprintf(initfile,"0");
                    }
                    fprintf(initfile,"%x",InitVector[i]);
                }
                fclose(initfile);
            }        
            FILE *fileout = fopen("cipher","w+");
            Encrypt_CBC(file,word,fileout,InitVector);
            fclose(fileout);
        }
        else{
            info("Decryption. \n");
            FILE *fileout = fopen("decipher","w+");
            FILE *initfile = fopen("iv","r");
            char temp[3];
            temp[2] = '\0';
            for (int i = 0; i < 16; i++)
            {
                temp[0] = getc(initfile);
                temp[1] = getc(initfile);
                InitVector[i] = strtol(temp,NULL,16);
            }
            Decrypt_CBC(file,word,fileout,InitVector);
            fclose(fileout);
            fclose(initfile);
        }
    }

    //Mode GCM
    /*else if(mode == 2){
        info("Mode: GCM. ");
        if(decrypt == 0){
            info("Encryption.\n");
            uchar IV[CHUNK-4]; //IV size = 96 bits
            vector_gen2(IV,12);
            info("Initialisation Vector:");
            for(int i = 0; i < CHUNK-4; i++){
                info("%4x", IV[i]);
            }
            info("\n");
            FILE* iv_file = fopen("iv","w+");
            write_block2(IV,iv_file,12);
            fclose(iv_file);
            FILE* AuthData_file = fopen("AAD","r");
            Encrypt_GCM(IV,file,AuthData_file,word);
            fclose(AuthData_file);
        }
        else
        {
            info("Decryption.\n");
            uchar IV[CHUNK-4];
            FILE* iv_file = fopen("iv","r");
            FILE* cipher_file = fopen("cipher","r");
            Decrypt_GCM(cipher_file,IV,word);
            fclose(iv_file);
        }
    }
    */
    if(VerboseMode == 1){
        clock_t  end;
        end = clock();
        ecart = (float)(end-begin)/CLOCKS_PER_SEC;
        info("Temps de calcul = %f sec.\n", ecart);
    }
    
    //Free
    free(key);
    for (int i = 0; i<Nb*(Nr+1); i++){
        free(word[i]);
    }
    free(word);
    fclose(file);
    
    return 0;
}