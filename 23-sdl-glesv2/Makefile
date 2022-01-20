BIN    = sample
LDLIBS = -lSDL2 -lGLESv2

$(BIN): main.c
	$(CC) $(LDLIBS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(BIN)
