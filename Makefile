CC = gcc
CFLAGS = -Wall
OBJ = coconut.c

all: coconut

coconut: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

debug: CFLAGS += -g3 -DDEBUG
debug: coconut

#%.o: %.c
#	$(CC) $(CFLAGS) $<

clean:
	rm -rf *.o
	rm -rf *.dSYM
	rm -rf coconut
