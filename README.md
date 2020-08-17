This is an example of:
1. Create and allocate a [DLPack](https://github.com/dmlc/dlpack/) tensor in a plain C library. Memory is supposed to also be freed in C with a custom deallocator.
2. Wrap DLPack structures in Python with `ctypes`. [dlpack.py](./dlpack.py) should in theory be reusable as is outside this example project. Hopefully, in future there [will be](https://github.com/dmlc/dlpack/issues/51#issuecomment-672757632) official `ctypes` DLPack bindings
3. (experimentally) Consume the DLPack tensor in NumPy. Hopefully, in future there [will be](https://github.com/dmlc/dlpack/issues/55) an official method for this. The main difficulty is absence of memory leaks and segfaults related to freeing the memory.
4. Demonstrate of importing DLPack tensor into [PyTorch](https://pytorch.org/docs/stable/dlpack.html?highlight=from_dlpack#torch.utils.dlpack.from_dlpack)

**IMPORTANT** Currently the sort-of-supported semantics of consuming DLPack tensors is move semantics. So consume the DLPack tensor only once. Also please monitor memory, I do not guarantee absence of leaks.

A more complete example of reading an audio file with ffmpeg library in C and consuming it via DLPack is available at https://github.com/vadimkantorov/readaudio

DLPack header is taken from https://github.com/dmlc/dlpack/blob/3efc489b55385936531a06ff83425b719387ec63/include/dlpack/dlpack.h

### Usage
```shell
# dump golden.bin with some test data
make dlpack
./dlpack golden.bin

# dump numpy.bin and torch.bin
make libdlpack.so
python3 dlpack.py numpy.bin
python3 dlpack.py torch.bin

# check that both ways of importing DLPack tensors work
diff golden.bin numpy.bin
diff golden.bin torch.bin
```
