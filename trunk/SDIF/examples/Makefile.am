INCLUDES  = -I../include -I$(top_srcdir)/include -I$(top_srcdir)/sdif
CFLAGS   = @CFLAGS@ '-DVERSION="$(VERSION)"'

all:		list-types

list-types: 	list-types.c
		gcc -I../include -g  -o list-types list-types.c -lsdif
