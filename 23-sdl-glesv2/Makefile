BIN    = sample
LDLIBS = -lSDL2 -lGLESv2

$(BIN): main.c
	$(CC) $(LDLIBS) -o $@ $^

run: $(BIN)
	SDL_VIDEODRIVER=wayland ./$(BIN)

.PHONY: clean
clean:
	rm -f $(BIN)
