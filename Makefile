CC = gcc

SHAREDFLAGS = -shared -fPIC

dlpack: dlpack.c
	$(CC) -o $@ $< $(CFLAGS)

dlpack.so: dlpack.c
	$(CC) -o $@ $(SHAREDFLAGS) $< $(CFLAGS)
