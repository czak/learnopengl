CFLAGS = -I../glad/include
LDLIBS = -lglfw -ldl

indexed-drawing: main.c ../glad/src/glad.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^
