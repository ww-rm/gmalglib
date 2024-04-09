#ifndef GMALGLIB_RANDOM_H
#define GMALGLIB_RANDOM_H

#include <stdint.h>

typedef int (*RandomBytesFunction)(void* rand_obj, uint64_t bytes_len, uint8_t* bytes);


#ifdef __cplusplus
extern "C" {
#endif

int DefaultRandomBytes(void* rand_obj, uint64_t bytes_len, uint8_t* bytes);

#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_RANDOM_H
