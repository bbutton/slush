CFLAGS=-g

run: slush
	./slush

slush: main.c
	gcc $(CFLAGS) $^ -o slush
