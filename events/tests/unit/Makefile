TARGET = libequeue.a

CC = gcc
AR = ar
SIZE = size

SRC += $(wildcard ../../source/*.c)
OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)
ASM := $(SRC:.c=.s)

ifdef DEBUG
CFLAGS += -O0 -g3
else
CFLAGS += -O2
endif
ifdef WORD
CFLAGS += -m$(WORD)
endif
CFLAGS += -I. -I../../include
CFLAGS += -std=c99
CFLAGS += -Wall
CFLAGS += -D_XOPEN_SOURCE=600

LFLAGS += -pthread


all: $(TARGET)

prof: prof.o $(OBJ)
	$(CC) $(CFLAGS) $^ $(LFLAGS) -o prof
	./prof

asm: $(ASM)

size: $(OBJ)
	$(SIZE) -t $^

-include $(DEP)

%.a: $(OBJ)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) -c -MMD $(CFLAGS) $< -o $@

%.s: %.c
	$(CC) -S $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET)
	rm -f prof prof.o prof.d
	rm -f $(OBJ)
	rm -f $(DEP)
	rm -f $(ASM)
