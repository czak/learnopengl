GLAD_PATH=glad_33_core_noloader

CXXFLAGS = -I$(GLAD_PATH)/include
LDLIBS = -lglfw

sample: main.cpp $(GLAD_PATH)/src/glad.c
	$(CXX) $(CXXFLAGS) $(LDLIBS) -o $@ $^

.PHONY: clean
clean:
	rm -f sample
