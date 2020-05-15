CC=gcc

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)

N=100

#CFLAGS = -I./src -I./aes -Wall 

all: clean AES cleanobj

AES: $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $^ -Wall

#.PHONY: clean

clean:
	rm -f $(OBJ)
	rm -f AES
	rm -f AES_TEST

cleanobj:
	rm -f $(OBJ)

test:
	$(CC) aes_tests/aes_test.c aes.c -o AES_TEST
	./AES_TEST

bench: benchECB benchCBC #benchGCM

benchECB:
	@echo "AES benchmarks for 128-bits key and ECB mode, N = $(N)"
	/usr/bin/time -f %E,%M bash benchmark/aes_benchmark.sh ECB ./toys/key128 ./toys/alice $(N)
	@echo "AES benchmarks for 192-bits key and ECB mode, N = $(N)"
	/usr/bin/time -f %E,%M bash benchmark/aes_benchmark.sh ECB ./toys/key192 ./toys/alice $(N)
	@echo "AES benchmarks for 256-bits key and ECB mode, N = $(N)"
	/usr/bin/time -f %E,%M bash benchmark/aes_benchmark.sh ECB ./toys/key256 ./toys/alice $(N)

benchCBC:
	@echo "AES benchmarks for 128-bits key and CBC mode, N = $(N)"
	/usr/bin/time -f %E,%M bash benchmark/aes_benchmark.sh CBC ./toys/key128 ./toys/alice $(N)
	@echo "AES benchmarks for 192-bits key and CBC mode, N = $(N)"
	/usr/bin/time -f %E,%M bash benchmark/aes_benchmark.sh CBC ./toys/key192 ./toys/alice $(N)
	@echo "AES benchmarks for 256-bits key and CBC mode, N = $(N)"
	/usr/bin/time -f %E,%M bash benchmark/aes_benchmark.sh CBC ./toys/key256 ./toys/alice $(N)


