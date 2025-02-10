CC = gcc
CFLAGS = -O3

all: bot

bot: othello.o bot.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ 

clean:
	rm -f *.o render
