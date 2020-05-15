#include"option.h"
#include"aes.h"
#include<time.h>
int VerboseMode;
int createAAD;
int AADsize;
int is_valid_key(char Key[]){
    int N;
    N = strlen(Key)/2;
    if (N != 16 && N != 24 && N != 32){
        fprintf(stderr, "Wrong size for the key !\n");
        return -1;
    }
    char c = Key[0];
    int i = 0;
    while(c != '\0'){
        c = Key[i];
        i++;
        char *end;
        strtol(Key, &end, 16);
        if(*end != 0){
            fprintf(stderr, "Wrong key format !\n");
            //printf("%d", c);
            return -1;
        }
        return N;
    }
    return 0;
}

void usage()
{
    fprintf(stderr, "AES algorithm\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "   ./AES [-d] [-m MODE] [-k FILE|STR] [-s SIZE] [-v] [-h] FILE\n\n");
    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, "   -d,   Enable decrypt mode (default is encrypt)\n");
    fprintf(stderr, "   -m,   Expects string MODE where:\n");
    fprintf(stderr, "                        MODE = ECB | CBC | GCM (default is ECB) \n");
    fprintf(stderr, "   -k,   Can be a hex string or path to file that contains hex string\n");
    fprintf(stderr, "                - For example to set a 128-bits\n");
    fprintf(stderr, "                use -k 0102030405060708090A0B0C0D0E0F10 or put the same\n");
    fprintf(stderr, "                string in a file and give file path to -k\n");
    //fprintf(stderr, "   -a,         Create a new Authentification Data.\n");
    fprintf(stderr, "   -s,   Create a random key in a file named rkey.key\n");
    fprintf(stderr, "   -v,   Enable verbose mode\n");
    fprintf(stderr, "   -h,   Show this message and exit\n");
 
    exit(EXIT_FAILURE);
}

uchar* options(int argc, char* argv[], int *N, int *decrypt, int *mode, int *verb, uchar *key){
    char key_in[256];
    int c;
    int optk = 0;
    int opts = 0;
    int random = 0;
    while ( (c = getopt(argc, argv, "k:dm:vhs:a:")) >= 0 )
    {
      switch (c)
      {
        case 'k': // SIMPLIFIER CETTE PARTIE  !
            optk = 1;
            strcpy(key_in, optarg);
            FILE *key_file = fopen(key_in,"r"); 
            if (key_file){
                fseek(key_file,0,SEEK_END);
                *N = (ftell(key_file))/2;
                rewind(key_file);
                //printf("%i\n", *N);
                if (*N != 16 && *N != 24 && *N != 32){
                    fprintf(stderr, "Provided key doesn't have the expected size, expected 16,24 or 32 bytes key\n");
                    exit(EXIT_FAILURE);
                }
                key = malloc((int)((*N))*sizeof(uchar));
                char temp[3]; //del 
                temp[2] = '\0'; 
                fseek(key_file,0,SEEK_SET);
                for (int i = 0; i < (int)((*N)) ;i++){
                    temp[0] = getc(key_file);
                    temp[1] = getc(key_file);
                    key[i] = strtol(temp,NULL,16);
                }
                fclose(key_file);
                //free(key);
            }

            else if((*N = is_valid_key(key_in)) != -1){
                key = malloc((int)*N*sizeof(uchar));
                char temp[3]; // del
                temp[2] = '\0'; // ça aussi
                for (int i = 0; i<*N*2; i+=2){ // N 
                    temp[0] = key_in[i];
                    temp[1] = key_in[i+1];
                    key[i/2] = strtol(temp,NULL,16); // unif
                }
            }
            else{
                fprintf(stderr, "Fail to recognize a key.\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'd':
            if(random == 1){
                fprintf(stderr, "Incompatible options -s and -d.\n");
                exit(EXIT_FAILURE);
            }
            random = 1;
            *decrypt = 1;
            break;
        case 'm':
            strcpy(key_in, optarg);
            if(strcmp(key_in,"ECB") == 0){
                *mode = 0;
            }
            else if(strcmp(key_in,"CBC") == 0){
                *mode = 1;
            }
            else if(strcmp(key_in,"GCM") == 0){
                *mode = 2;
            }
            break;
        case 'v':
            *verb = 1;
            info("Verbose Mode ON. \n");
            break;
        case 'h':
            usage();
            break;
        case 's':
            if(random == 1){
                fprintf(stderr, "Incompatible options -s and -d.\n");
                exit(EXIT_FAILURE);
            }
            random = 1;
            opts = 1;
            if(opts == optk){
                fprintf(stderr, "Incompatible options -k and -s. \n");
            }
            int bitsize;
            bitsize = atoi(optarg);
            *N = bitsize/8;
            //printf("%i", *N);
            if (*N == 16|| *N == 24 || *N == 32){
                key = malloc((int)((*N))*sizeof(uchar));
                vector_gen(key,*N);
                FILE *randomkey = fopen("rkey.key","w+");
                if(randomkey){
                    for(int i = 0; i<*N; i++){
                        if(key[i]<16){
                            fprintf(randomkey,"0");
                        }
                        fprintf(randomkey,"%x",key[i]);
                    }
                    fclose(randomkey);
                }
            }
            else{
                fprintf(stderr, "Wrong Key Size. \n");
                exit(EXIT_FAILURE);
            }
            break;
        case 'a':
            createAAD = 1;
            AADsize = atoi(optarg);
            break;
        }
    }
    return key;
}

int info(const char* format, ...)
{
  if (!VerboseMode)
    return 0;

  va_list args;
  va_start(args, format);
  int to_return = vprintf(format, args);
  va_end(args);
  return to_return;
}