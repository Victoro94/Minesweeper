CC=gcc
CFLAGS= -Wall -Werror -Wextra -Wvla
LDFLAGS= -lncurses

SRCS= \
main.c \
print_board.c

OBJS=$(SRCS:.c=.o)

all: $(OBJS)
	$(CC) -o ../Minesweeper $(OBJS) $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) $(OBJS)
