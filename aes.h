#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<time.h>

#define CHUNK 16

typedef unsigned char uchar;

extern int Nb;
extern int Nk;
extern int Nr;

/*
struct aes_t
{
    uchar*  key; //Clé 
    uchar*  keyr; // Clés de rondes = word ?
    uchar*  state; //
    uchar** word;
};
  
typedef struct aes_t aes_t;
  
aes_t*    aes_Init();
void      aes_Alloc(aes_t* aes);
void      aes_Destroy(aes_t* aes);
int       aes_SetKey(aes_t* aes, uchar* raw, int size);
void      aes_KeySchedule(aes_t* aes);
void      aes_enc_ECB(aes_t* aes);
void      aes_dec_ECB(aes_t* aes);
int       aes_enc_ECB_f(aes_t* aes, FILE* in, FILE* out);
int       aes_dec_ECB_f(aes_t* aes, FILE* in, FILE* out); */

// Remplacer ce qu'il y a en dessous par ce qu'il y a au dessus:
void vector_gen(uchar *Key, int KeySize);
void vector_gen2(uchar *Key, int KeySize);
int write_block(uchar buffer[], FILE* fileout);
int write_block2(uchar buffer[], FILE* fileout, int Size);
int get_block(FILE* file, uchar buffer[]);

void AES_cipher(uchar buffer[], uchar **word);
void AES_InvCipher(uchar buffer[], uchar **word);
void KeySchedule(uchar Key[], uchar **word);
void Encrypt_GCM(uchar* IV, FILE* plaintext_file, FILE* AuthData_file, uchar** word);
void Decrypt_GCM(FILE* cipher_file, uchar* IV, uchar** word);
void Encrypt_ECB(FILE *file, uchar **word, FILE *fileout);
void Decrypt_ECB(FILE *file, uchar **word, FILE *fileout);
void Encrypt_CBC(FILE *file, uchar **word, FILE *fileout, uchar InitVector[]);
void Decrypt_CBC(FILE *file, uchar **word, FILE *fileout, uchar InitVector[]);

//uchar* Mult_GCM(uchar*, uchar*);