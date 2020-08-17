#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// exact commit is in readme, master; https://github.com/dmlc/dlpack/blob/master/include/dlpack/dlpack.h
#include "dlpack.h"

void deleter(struct DLManagedTensor* self)
{
	fprintf(stderr, "Deleter calling\n");
	if(self->dl_tensor.data)
	{
		free(self->dl_tensor.data);
		self->dl_tensor.data = NULL;
	}

	if(self->dl_tensor.shape)
	{
		free(self->dl_tensor.shape);
		self->dl_tensor.shape = NULL;
	}
	
	if(self->dl_tensor.strides)
	{
		free(self->dl_tensor.strides);
		self->dl_tensor.strides = NULL;
	}
	fprintf(stderr, "Deleter called\n");
}

struct DLManagedTensor create_and_allocate_dlpack_tensor()
{
	struct DLManagedTensor dlpack = {0};
	dlpack.deleter = deleter;
	dlpack.dl_tensor.ctx.device_type = kDLCPU;
	dlpack.dl_tensor.ndim = 2;
	DLDataType dtype = {.code = 0, .bits = 32, .lanes = 1}; 
	dlpack.dl_tensor.dtype = dtype;
	dlpack.dl_tensor.shape = malloc(dlpack.dl_tensor.ndim * sizeof(int64_t));
	dlpack.dl_tensor.shape[0] = 3;
	dlpack.dl_tensor.shape[1] = 2;
	dlpack.dl_tensor.strides = malloc(dlpack.dl_tensor.ndim * sizeof(int64_t));
	dlpack.dl_tensor.strides[0] = dlpack.dl_tensor.shape[1];
	dlpack.dl_tensor.strides[1] = 1;
	uint64_t itemsize = dlpack.dl_tensor.dtype.lanes * dlpack.dl_tensor.dtype.bits / 8;
	uint32_t* data_ptr = calloc(dlpack.dl_tensor.shape[0] * dlpack.dl_tensor.shape[1], itemsize);
	
	// 3x2 int32 tensor with some sample data
	data_ptr[0] = 0;
	data_ptr[1] = 1;
	data_ptr[2] = 2;
	data_ptr[3] = 3;
	data_ptr[4] = 4;
	data_ptr[5] = 5;
	
	dlpack.dl_tensor.data = data_ptr;
	return dlpack;
}

int main(int argc, char **argv)
{
	struct DLManagedTensor dlpack = create_and_allocate_dlpack_tensor();
	uint64_t itemsize = dlpack.dl_tensor.dtype.lanes * dlpack.dl_tensor.dtype.bits / 8;
	
	fwrite(dlpack.dl_tensor.data, itemsize, dlpack.dl_tensor.shape[0] * dlpack.dl_tensor.shape[1], fopen(argv[1], "wb"));
	return 0;
}
