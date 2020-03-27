GLAD = glad_45_core_noloader
CXXFLAGS = -I$(GLAD)/include
LDLIBS = -lglfw

sample: main.cpp $(GLAD)/src/glad.c
	$(CXX) $(CXXFLAGS) $(LDLIBS) -o $@ $^

.PHONY: clean
clean:
	rm -f sample
