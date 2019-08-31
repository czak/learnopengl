CFLAGS = -I../glad/include
LDLIBS = -lglfw -ldl

uniforms: main.cpp ../glad/src/glad.o
	$(CXX) $(CFLAGS) $(LDLIBS) -o $@ $^
