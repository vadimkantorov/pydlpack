This is an example of:
1. Create and allocate a [DLPack](https://github.com/dmlc/dlpack/) tensor in plain C library. Memory is supposed to also be freed in C with a custom deallocator.
2. Wrap DLPack structures in Python with `ctypes`
3. (experimental) Utility of consuming DLPack tensor in NumPy. The main difficulty is absence of memory leaks and segfaults related to freeing the e
4. Demonstration of importing DLPack tensor into [PyTorch](https://pytorch.org/docs/stable/dlpack.html?highlight=from_dlpack#torch.utils.dlpack.from_dlpack)

**IMPORTANT** Currently the sort-of-supported semantics of consuming DLPack tensors is move semantics. So consume the DLPack tensor only once. Also please monitor memory, I do not guarantee absence of leaks.

A more complete example of reading an audio file with ffmpeg library in C and consuming it via DLPack is available at https://github.com/vadimkantorov/readaudio

### Usage
```shell
make dlpack dlpack.so

# dump golden.bin
./dlpack golden.bin

# dump numpy.bin and torch.bin
python3 dlpack.py numpy.bin
python3 dlpack.py torch.bin

diff golden.bin numpy.bin
diff golden.bin torch.bin
```
