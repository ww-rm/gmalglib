#include <stdint.h>
#include <stdlib.h>
#include <gmalglib/random.h>


static int RandomBytesSimple(void* rand_obj, uint64_t bytes_len, uint8_t* bytes)
{
    uint64_t i;
    srand((unsigned int)rand_obj);
    for (i = 0; i < bytes_len; i++)
    {
        bytes[i] = (uint8_t)rand();
    }

    return 1;
}

int DefaultRandomBytes(void* rand_obj, uint64_t bytes_len, uint8_t* bytes)
{
    return RandomBytesSimple(rand_obj, bytes_len, bytes);
}
