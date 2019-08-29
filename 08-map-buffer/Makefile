CFLAGS = -I../glad/include
LDLIBS = -lglfw -ldl

map-buffer: main.cpp ../glad/src/glad.o
	$(CXX) $(CFLAGS) $(LDLIBS) -o $@ $^
