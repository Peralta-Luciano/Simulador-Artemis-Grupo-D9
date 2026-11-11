# Makefile - Simulador Artemis (Situacion 4)
# Uso:  make        -> compila y genera "simulador"
#       make run    -> compila y ejecuta
#       make clean  -> borra los archivos generados
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lm
TARGET  = simulador
SRCS    = main.c simulador.c validaciones.c
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c simulador.h validaciones.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all run clean
