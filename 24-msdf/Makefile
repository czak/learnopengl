LDLIBS = -lglfw -lGLESv2

main: main.c shaders/vertex_shader.h shaders/fragment_shader.h
	$(CC) $< $(LDLIBS) -o $@

shaders/vertex_shader.h: shaders/vertex.glsl
	shaders/parse_shader.sh $< > $@

shaders/fragment_shader.h: shaders/fragment.glsl
	shaders/parse_shader.sh $< > $@
