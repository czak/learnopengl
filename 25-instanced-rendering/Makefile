LDLIBS = -lglfw -lGLESv2
OBJS = main.o font/monogram.o
SHADERS = shaders/vertex_shader.h shaders/fragment_shader.h

main: $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) -o $@

main.o: main.c $(SHADERS)

shaders/vertex_shader.h: shaders/vertex.glsl
	shaders/parse_shader.sh $< > $@

shaders/fragment_shader.h: shaders/fragment.glsl
	shaders/parse_shader.sh $< > $@

.PHONY: clean
clean:
	rm -f main $(OBJS) $(SHADERS)
