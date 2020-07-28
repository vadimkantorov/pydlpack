#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* BEGIN https://github.com/dmlc/dlpack/blob/master/include/dlpack/dlpack.h */

#include <stdint.h>
#include <stddef.h>

typedef enum {
  kDLCPU = 1,
  kDLGPU = 2,
  kDLCPUPinned = 3,
  kDLOpenCL = 4,
  kDLVulkan = 7,
  kDLMetal = 8,
  kDLVPI = 9,
  kDLROCM = 10,
  kDLExtDev = 12,
} DLDeviceType;

typedef struct {
  DLDeviceType device_type;
  int device_id;
} DLContext;

typedef enum {
  kDLInt = 0U,
  kDLUInt = 1U,
  kDLFloat = 2U,
  kDLBfloat = 4U,
} DLDataTypeCode;

typedef struct {
  uint8_t code;
  uint8_t bits;
  uint16_t lanes;
} DLDataType;

typedef struct {
  void* data;
  DLContext ctx;
  int ndim;
  DLDataType dtype;
  int64_t* shape;
  int64_t* strides;
  uint64_t byte_offset;
} DLTensor;

typedef struct DLManagedTensor {
  DLTensor dl_tensor;
  void * manager_ctx;
  void (*deleter)(struct DLManagedTensor * self);
} DLManagedTensor;

/* END https://github.com/dmlc/dlpack/blob/master/include/dlpack/dlpack.h */

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
	
	FILE *out = fopen(argv[1], "wb");
	uint64_t itemsize = dlpack.dl_tensor.dtype.lanes * dlpack.dl_tensor.dtype.bits / 8;
	fwrite(dlpack.dl_tensor.data, itemsize, dlpack.dl_tensor.shape[0] * dlpack.dl_tensor.shape[1], out);
	fclose(out);
	return 0;
}
