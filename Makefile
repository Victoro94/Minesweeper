CC=gcc
CFLAGS= -Wall -Werror -Wextra -Wvla
LDFLAGS= -lncurses

SRCS= src/main.c
OBJS=$(SRCS:.c=.o)

all: $(OBJS)
	$(CC) -o Minesweep $(OBJS) $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) Minesweep $(OBJS)
