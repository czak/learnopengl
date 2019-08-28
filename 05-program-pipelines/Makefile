CFLAGS = -I../glad/include
LDLIBS = -lglfw -ldl

program-pipelines: program-pipelines.c ../glad/src/glad.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^
