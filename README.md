# A command-line tool to encrypt and decrypt files using AES.

AES algorithms are implemented as a standalone C99 library **(aes.\* files)**.

Supported features: 
- key sizes: 128, 192, 256-bits
- mode: ECB, CBC

# Installation

## 1. Compile aes library and commande line interface for encryption/decryption
```bash
make
```

## 2. Compile and run tests
After compiling the tool, you can compile and run tests with:
```bash
make test
```  

These tests are performed for cipher and decipher, and for all size key (Using NIST's documentation examples).

## 3. Benchmarks
```bash
make bench
``` 
Give the encryption runtime (in seconds) and max memory footprints (in bytes) after `N=100` iterations for all modes and all sizes key. You can also give an other value to `N`, for example with `50`:

```make bench N=50 ```
# Usage and help

To see help in shell use: ```./AES -h```  
```
Usage:
./AES [-d] [-m MODE] [-k FILE|STR] [-s SIZE] [-v] [-h] FILE

Arguments:
   -d,   Enable decrypt mode (default is encrypt)
   -m,   Expects string MODE (default is ECB)
                        MODE = ECB | CBC
   -k,   Can be a hex string or path to file that contains hex string
                       - For example to set a 128-bits use
                        -k 0102030405060708090A0B0C0D0E0F10
                        or put the same string in a file and give file path to -k
   -s,   Create a random key in a file named rkey.key
   -v,   Enable verbose mode
   -h,   Show this message and exit
```

# Examples
- Encrypt the text named `alice` with the file `key256` which countain a 256-bits key with default mode (EBC):
```
./AES -k toys/key256 toys/alice
```
This command create a new file named `cipher` which contain the encrypted text.
- Decryption of previous `cipher` text:
```
./AES -k toys/key256 -d cipher
```
This command create a new file named `decipher` which contain the decrypted text.
- Using CBC mode with a random 192-bits key:
  - Encryption: `./AES -s 192 -m CBC toys/alice` (Create both an I.V. in `iv` and a random key in `rkey.key`).
  - Decryption: `./AES -k rkey.key -m CBC -d cipher` (Use the previously created `iv`)
