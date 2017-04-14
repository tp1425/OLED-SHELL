CC = gcc
CFLAGS = -Wall -g -std=c99
DEPS = list.h tokenizer.h
OBJ = list.o tokenizer.o sush.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sush: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ core *~
#this is a comment
