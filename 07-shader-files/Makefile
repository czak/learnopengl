CFLAGS = -I../glad/include
LDLIBS = -lglfw -ldl

shader-files: main.cpp ../glad/src/glad.o
	$(CXX) $(CFLAGS) $(LDLIBS) -o $@ $^
