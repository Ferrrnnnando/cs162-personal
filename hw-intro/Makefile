CC?=gcc

ifneq (,$(shell which i386-gcc))
CC=i386-gcc
endif

CFLAGS?=-g
BINARIES=map limits

default:
	make map
	make limits

map: map.c
	$(CC) $(CFLAGS) map.c recurse.c -o map -D CS162

limits: limits.c
	$(CC) $(CFLAGS) limits.c -o limits

clean:
	rm -f $(BINARIES)
