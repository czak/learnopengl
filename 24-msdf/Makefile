LDLIBS = -lglfw -lGLESv2
OBJS = main.o font/chakra.o
SHADERS = shaders/vertex_shader.h shaders/fragment_shader.h

main: $(OBJS)
	$(CC) $(OBJS) $(LDLIBS) -o $@

main.o: main.c $(SHADERS)

font/%.c: font/%.png
	magick $< rgb:- | xxd -i -c 16 -n font > $@

shaders/vertex_shader.h: shaders/vertex.glsl
	shaders/parse_shader.sh $< > $@

shaders/fragment_shader.h: shaders/fragment.glsl
	shaders/parse_shader.sh $< > $@

.PHONY: clean
clean:
	rm -f main $(OBJS) $(SHADERS)
