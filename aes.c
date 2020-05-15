#include"aes.h"
#include <sys/types.h>
#include<unistd.h> //getpid

/**
 * SBox  
 * Size 16x16
 * NIST
 */
static const uchar SBox[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
 };

 /**
 * Inverse SBox
 * Size: 16x16
 */
static const uchar InvSBox[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
/**
 * Rcon1  
 * Rijndael table (NIST)
 */
static const uchar Rcon1[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

/**
 *\fn Rotword 
 *\param[in] minword Mot de 4 octets 
 *\brief le premier octet de minword est déplacé à la fin.
 */ 
void Rotword(uchar minword[]){
    int i = 0;
    uchar temp;
    temp = minword[0];
    for (i=0;i<Nb-1;i++){
        minword[i] = minword[i+1];        
    }
    minword[Nb-1] = temp;
}

/**
 *\fn Subword
 *\param[in] minword Mot de 4 octets.
 *\brief Transforme chaque octet du mot avec la Table de Substitution SBox.
 */ 
void Subword(uchar minword[]) {
    int a,b;
    for (int i = 0; i<Nb; i++){
        a = minword[i]/16;
        b = minword[i]%16;
        minword[i] = SBox[a*16+b];
    }
    //return Colword;
}

/**
 *\fn KeySchedule  
 *\param[in] Key clé de chiffrement sous forme d'un tableau (allocation dynamique de mémoire) 
 *\param[in] **word Tableau sous la forme d'un tableau 2D de taille Nb(Nb+1)x4 octets 
 *\brief A partir de la clé de chiffrement, détermine et stocke l'ensemble des clés de ronde dans la table Ronde
 */ 
void KeySchedule(uchar Key[], uchar **word){
    int i = 0;
    int j = 0;

    for(i=0;i<Nk;i++){
        for (j=0;j<Nb;j++){
            word[i][j] = Key[4*i+j];
        }
    }
    for(i=Nk;i<Nb*(Nr+1);i++){
        uchar *temp = calloc(Nb,sizeof(uchar));
        for(j=0;j<Nb;j++){
            temp[j] = word[i-1][j];
        }
        if (i%Nk == 0){
            Rotword(temp);
            Subword(temp);
            /*
            for (j=0;j<Nb;j++){
                temp[j] = temp[j] ^ Rcon[i/Nk][j];
            }*/
            temp[0] = temp[0] ^ Rcon1[(i/Nk)-1];
        }
        //Condition jamais remplie pour l'aes 128, 192.
        else if (Nk > 6 && i%Nk == 4){
            Subword(temp);
        }
        for(j=0;j<Nb;j++){
            word[i][j] = word[i-Nk][j] ^ temp[j];
        }
        free(temp);
    }
}

/**
 *\fn KeyRound
 *\param[in] **word Tableau contenant les différentes clés de rondes.
 *\param[in] KeyR[] Tableau de taille Nk*Nb
 *\param[in] NTour Numéro du tour en cours. 
 *\brief Extraire la clé de chiffrement de la ronde 'NTour' contenue dans 'word' et la stocker dans KeyR.
 */ 
void KeyRound(uchar **word, uchar KeyR[], int NTour){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j<Nb; j++){
            KeyR[i+4*j] = word[4*NTour+j][i];
            //KeyR[j+4*i] = word[4*NTour+j][i];
        }
    }
}

/**
*\fn padding_right 
*\param[in] *block 
*\param[in] byte 
*\param[in] value
*\brief Complète un bloc pour obtenir un bloc de taille 16 (octets).
**/
void padding_right(uchar* block, int byte, int value)
{
  size_t size = CHUNK-byte;
  memset(&block[byte], value, size);
}

/**
*\fn get_block
*\param[in] *file
*\param[in] buffer[]
**/
int get_block(FILE* file, uchar buffer[])
{
  if (feof(file)){
    return -1;
  }
  int i;
  int c;
  for (i=0; i<CHUNK; i++)
  {
    c = fgetc(file);
    if ((c == EOF) && (i==0))
    {
      return -1;
    }
    else if (c == EOF)
    {
      padding_right(buffer, i, 0); // padding avec '0' en ascii
      break;
    }
    buffer[i] = (uchar)c; 
  }
  return 1;
}

int write_block(uchar buffer[], FILE* fileout){
    if(feof(fileout)){
        return -1;
    }
    int i = 0;
    for (i=0; i<CHUNK; i++){
        fputc(buffer[i],fileout); 
    }
    return 1;
}
int write_block2(uchar buffer[], FILE* fileout, int Size){
    if(feof(fileout)){
        return -1;
    }
    int i = 0;
    for (i=0; i<Size; i++){
        fputc(buffer[i],fileout); 
    }
    return 1;
}
void vector_gen(uchar *Key, int KeySize){
    static FILE *fp;
    fp = fopen("/dev/random","rb");
    if(!fp){
        fprintf(stderr,"erreur d'acces au fichier /dev/random. \n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < KeySize ;i++){
        Key[i] = fgetc(fp);
    }
    fclose(fp);
}
void vector_gen2(uchar *Key, int KeySize){
    srand(time(NULL)+getpid());
    for (int i = 0; i < KeySize ;i++){
        Key[i] = (uchar) rand();
    }
}
/**
*\fn SubBytes
*\param[in] buffer 
*\brief Chaque octet de 'buffer' est remplacé par un autre grâce à la SBox
**/
void SubBytes(uchar buffer[]){
    int a,b;
    for(int i = 0; i < 16; i++){
        a = buffer[i]/16;
        b = buffer[i]%16;
        buffer[i] = SBox[a*16+b];
    }
}

void InvSubBytes(uchar buffer[]){
    int a,b;
    for(int i = 0; i < 16; i++){
        a = buffer[i]/16;
        b = buffer[i]%16;
        buffer[i] = InvSBox[a*16+b];
    }
}
/**
 *\fn ShiftRows
 *\param[in] buffer 
 *\brief Vu comme un tableau de taille 4*4, on permutte les éléments (octets) de chaque ligne. 
 **/
void ShiftRows(uchar buffer[]){
    uchar temp[4];
    for(int i = 1; i < 4; i++){
        for(int j = 0; j < 4; j++){
            temp[j] = buffer[4*j+i];
        }
        for(int j = 0; j < 4; j++){
            buffer[4*j+i] = temp[(j+i)%4];
        }
    }    
}

void InvShiftRows(uchar buffer[]){
    uchar temp[4];
    for(int i = 1; i < 4; i++){
        for(int j = 0; j < 4; j++){
            temp[j] = buffer[4*j+i];
        }
        for(int j = 0; j < 4; j++){
            buffer[4*j+i] = temp[(j+3*i)%4];
        }
    }    
}
/**
 *\fn xtime
 *\param[in] bytes 
 *\brief multiplication par x ....
 **/
uchar xtime(uchar Bytes){
    if(Bytes/128 == 1){
        return (uchar) (Bytes<<1) ^ 0x1b; //0x91 ?
    }
    else{
        return (uchar) (Bytes<<1);
    }
}

/**
 *\fn MixColumns 
 *\param[in] buffer
 *\brief 
 **/
void MixColumns(uchar buffer[]){
    uchar temp[4];
    for(int j = 0; j < 4; j++){
        for (int i = 0; i < 4; i++){
            temp[i] = buffer[4*j + i];
        }
        for (int i = 0; i < 4; i++){
            buffer[4*j+i] = xtime(temp[(i)%4]) ^ (xtime(temp[(1+i)%4]) ^ temp[(1+i)%4]) ^ temp[(2+i)%4] ^ temp[(3+i)%4];
        }
    }
}

void InvMixColumns(uchar buffer[]){
    uchar temp[4];
    for(int j = 0; j < 4; j++){
        for (int i = 0; i < 4; i++){
            temp[i] = buffer[4*j + i];
        }
        for (int i = 0; i < 4; i++){
            buffer[4*j+i] = xtime(xtime(xtime(temp[i%4])^temp[i%4])^temp[i%4]) ^ temp[(i+1)%4] ^ xtime(temp[(i+1)%4]^xtime(xtime(temp[(i+1)%4]))) ^ temp[(i+2)%4] ^ xtime(xtime(temp[(i+2)%4] ^ xtime(temp[(i+2)%4]))) ^ temp[(i+3)%4] ^ xtime(xtime(xtime(temp[(i+3)%4])));
        }
    }
}
/**
 *\fn AddRoundKey 
 * \param[in] buffer sous la forme d'un tableau de 16 octets.
 * \param[in] KeyR Clé de ronde sous la forme d'un tableau de 16 octets.
 * \brief 
 **/

void AddRoundKey(uchar buffer[], uchar KeyR[]){
    for (int i = 0; i < 16; i++){
        buffer[i] = buffer[i] ^ KeyR[i];
    }
}

/**
 *\fn AES_cipher
 *\param[in] *file
 *\param[in] **word
 *\brief 
 */
void AES_cipher(uchar buffer[], uchar **word){
    uchar KeyR[16];
    KeyRound(word,KeyR,0);
    AddRoundKey(buffer,KeyR);
    for(int i = 0; i < Nr-1; i++){
        SubBytes(buffer);
        ShiftRows(buffer);
        MixColumns(buffer);
        KeyRound(word,KeyR,i+1);
        AddRoundKey(buffer,KeyR);
    }
    SubBytes(buffer);
    ShiftRows(buffer);
    KeyRound(word,KeyR,Nr);
    AddRoundKey(buffer,KeyR);
}

/**
 *\fn AES_decipher 
 *\param[in] *file
 *\param[in] **word
 *\brief 
 */
void AES_InvCipher(uchar buffer[], uchar **word){
    uchar KeyR[16];
    KeyRound(word,KeyR,Nr);
    AddRoundKey(buffer,KeyR);

    for(int i = 0; i < Nr-1; i++){
        InvShiftRows(buffer);

        InvSubBytes(buffer);

        KeyRound(word,KeyR,Nr-i-1);
        AddRoundKey(buffer,KeyR);

        InvMixColumns(buffer);
        /*for (int i = 0; i<16; i++){
            printf("%4x", buffer[i]);
        }
        printf("\n");*/
    }
    InvShiftRows(buffer);
    InvSubBytes(buffer);
    KeyRound(word,KeyR,0);
    AddRoundKey(buffer,KeyR);
}

/**
 *\fn Encrypt_ECB
 *\param[in] *file
 *\param[in] Key[]
 *\param[in] *fileout
 *\brief 
 */
void Encrypt_ECB(FILE *file, uchar **word, FILE *fileout){

    uchar* buffer = calloc(1,CHUNK); //blocs de 16*8 octets = 128 bits 

    while (get_block(file, buffer) != -1)
    { 
        AES_cipher(buffer,word);
        write_block(buffer,fileout);
    }
    /*for (int i = 0; i<16; i++){
        printf("%4x", buffer[i]);
    }
    printf("\n");*/
    //Free
    free(buffer);
}
/**
 *\fn Decrypt_ECB
 *\param[in] *file
 *\param[in] Key[]
 *\param[in] *fileout
 *\brief 
 */
void Decrypt_ECB(FILE *file, uchar **word, FILE *fileout){
    uchar* buffer = calloc(1,CHUNK); //blocs de 16*8 octets = 128 bits 

    while (get_block(file, buffer) != -1)
    { 
        AES_InvCipher(buffer,word);
        write_block(buffer,fileout);
    }
    /*for (int i = 0; i<16; i++){
        printf("%4x", buffer[i]);
    }
    printf("\n");*/
    //Free
    free(buffer);
}

/**
 *\fn Encrypt_CBC
 *\param[in] *file
 *\param[in] Key[]
 *\param[in] *fileout
 *\brief 
 */
void Encrypt_CBC(FILE *file, uchar **word, FILE *fileout, uchar InitVector[]){
    uchar* buffer = calloc(CHUNK,sizeof(uchar));
    uchar* prec = calloc(CHUNK,sizeof(uchar));
    for (int i = 0; i<CHUNK; i++){
        prec[i] = InitVector[i];
    }
    while (get_block(file, buffer) != -1){
        for(int i = 0; i<CHUNK; i++){
            buffer[i] = buffer[i] ^ prec[i];
        }
        AES_cipher(buffer,word);
        for(int i = 0; i < CHUNK; i++){
            prec[i] = buffer[i];
        }
        write_block(buffer,fileout);
    }
    free(buffer);
    free(prec);
}

/**
 *\fn Decrypt_CBC
 *\param[in] *file
 *\param[in] Key[]
 *\param[in] *fileout
 *\brief 
 */
void Decrypt_CBC(FILE *file, uchar **word, FILE *fileout, uchar InitVector[]){
    uchar* buffer = calloc(1, sizeof(uchar)*CHUNK);
    uchar* actu = calloc(1,sizeof(uchar)*CHUNK);
    uchar* prec = calloc(1,sizeof(uchar)*CHUNK);
    for (int i = 0; i<CHUNK; i++){
        prec[i] = InitVector[i];
    }
    while (get_block(file, buffer) != -1){
        for(int i = 0; i < CHUNK; i++){
            actu[i] = buffer[i];
        }
        AES_InvCipher(buffer,word);
        for(int i = 0; i<CHUNK; i++){
            buffer[i] = buffer[i] ^ prec[i];
        }
        for(int i = 0; i < CHUNK; i++){
            prec[i] = actu[i];
        }
        write_block(buffer,fileout);
    }
    free(buffer);
    free(actu);
    free(prec);
}
