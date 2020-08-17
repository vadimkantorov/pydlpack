CC = gcc

SHAREDFLAGS = -shared -fPIC

dlpack: dlpack.c dlpack.h
	$(CC) -o $@ $< $(CFLAGS)

libdlpack.so: dlpack.c dlpack.h
	$(CC) -o $@ $(SHAREDFLAGS) $< $(CFLAGS)

clean:
	rm dlpack libdlpack.so

.PHONY: clean
