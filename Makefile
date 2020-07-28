CC = gcc

SHAREDFLAGS = -shared -fPIC

dlpack: dlpack.c dlpack.h
	$(CC) -o $@ $< $(CFLAGS)

dlpack.so: dlpack.c dlpack.h
	$(CC) -o $@ $(SHAREDFLAGS) $< $(CFLAGS)
