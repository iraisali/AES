#include "../aes.h"

uchar in[16] = { 0x00 , 0x11 , 0x22 , 0x33 , 0x44 , 0x55 , 0x66 , 0x77 , 0x88 , 0x99 , 0xaa, 0xbb , 0xcc , 0xdd , 0xee , 0xff };

uchar key16[16] = { 0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 0x0a, 0x0b , 0x0c , 0x0d , 0x0e , 0x0f };

uchar out16[16] = { 0x69 , 0xc4 , 0xe0 , 0xd8 , 0x6a , 0x7b , 0x04 , 0x30 , 0xd8 , 0xcd , 
                    0xb7, 0x80 , 0x70 , 0xb4 , 0xc5 , 0x5a };

uchar key24[24] = { 0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 
                    0x0a, 0x0b , 0x0c , 0x0d , 0x0e , 0x0f , 0x10 , 0x11 , 0x12 , 0x13 , 0x14, 
                    0x15 , 0x16 , 0x17 };

uchar out24[16] = { 0xdd , 0xa9 , 0x7c , 0xa4 , 0x86 , 0x4c , 0xdf , 0xe0 , 0x6e , 0xaf , 
                    0x70, 0xa0 , 0xec , 0x0d , 0x71 , 0x91 };

uchar key32[32] = { 0x00 , 0x01 , 0x02 , 0x03 , 0x04 , 0x05 , 0x06 , 0x07 , 0x08 , 0x09 , 
                    0x0a, 0x0b , 0x0c , 0x0d , 0x0e , 0x0f , 0x10 , 0x11 , 0x12 , 0x13 , 0x14, 
                    0x15 , 0x16 , 0x17 , 0x18 , 0x19 , 0x1a , 0x1b , 0x1c , 0x1d , 0x1e , 0x1f
                  };

uchar out32[16] = { 0x8e , 0xa2 , 0xb7 , 0xca , 0x51 , 0x67 , 0x45 , 0xbf , 0xea , 0xfc ,
                    0x49, 0x90 , 0x4b , 0x49 , 0x60 , 0x89 };

int Nb = 4;
int Nk = 4;
int Nr = 10;

int main(int argc, char** argv)
{

  //128-bits key testing
  uchar **word = calloc(Nb*(Nr+1),sizeof(uchar*));
  for (int i=0;i<Nb*(Nr+1);i++){
    word[i] = calloc(Nb,sizeof(uchar));
  }
  KeySchedule(key16,word);
  uchar buffer[16];
  memcpy(buffer,in,16);
  AES_cipher(buffer,word);
  //cipher 128
  if (memcmp(out16, buffer, 16) == 0)
    fprintf(stderr, "128-bits key encryption test: PASSED\n");
  else
    fprintf(stderr, "128-bits key encryption test: FAILED\n");
  //decipher 128
  AES_InvCipher(buffer,word);
  if (memcmp(in, buffer, 16) == 0)
    fprintf(stderr, "128-bits key decryption test: PASSED\n");
  else
    fprintf(stderr, "128-bits key decryption test: FAILED\n");
  
  for (int i = 0; i<Nb*(Nr+1); i++){
    free(word[i]);
  }
  free(word);

  // 192-bits key testing
  Nk = 6;
  Nr = 12;

  uchar **word2 = calloc(Nb*(Nr+1),sizeof(uchar*));;
  for (int i=0;i<Nb*(Nr+1);i++){
    word2[i] = calloc(Nb,sizeof(uchar));
  }
  KeySchedule(key24,word2);
  
  memcpy(buffer,in,16);
  AES_cipher(buffer,word2);
  //cipher 192
  if (memcmp(out24, buffer, 16) == 0)
    fprintf(stderr, "192-bits key encryption test: PASSED\n");
  else
    fprintf(stderr, "192-bits key encryption test: FAILED\n");
  //decipher 192
  AES_InvCipher(buffer,word2);
  if (memcmp(in, buffer, 16) == 0)
    fprintf(stderr, "192-bits key decryption test: PASSED\n");
  else
    fprintf(stderr, "192-bits key decryption test: FAILED\n");
  
  for (int i = 0; i<Nb*(Nr+1); i++){
    free(word2[i]);
  }
  free(word2);

  // 256-bits key testing
  Nk = 8;
  Nr = 14;

  uchar **word3 = calloc(Nb*(Nr+1),sizeof(uchar*));
  for (int i=0;i<Nb*(Nr+1);i++){
    word3[i] = calloc(Nb,sizeof(uchar));
  }
  KeySchedule(key32,word3);
  
  memcpy(buffer,in,16);
  AES_cipher(buffer,word3);
  //cipher 256
  if (memcmp(out32, buffer, 16) == 0)
    fprintf(stderr, "256-bits key encryption test: PASSED\n");
  else
    fprintf(stderr, "256-bits key encryption test: FAILED\n");
  //decipher 256
  AES_InvCipher(buffer,word3);
  if (memcmp(in, buffer, 16) == 0)
    fprintf(stderr, "256-bits key decryption test: PASSED\n");
  else
    fprintf(stderr, "256-bits key decryption test: FAILED\n");
  for (int i = 0; i<Nb*(Nr+1); i++){
    free(word3[i]);
  }
  free(word3);
}