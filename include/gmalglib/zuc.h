#ifndef GMALGLIB_ZUC_H
#define GMALGLIB_ZUC_H

#include <stdint.h>

#define ZUC_KEY_LENGTH      16
#define ZUC_IV_LENGTH       16
#define ZUC_WORD_LENGTH     4

typedef uint32_t uint31_t;

typedef struct _ZUC {
    uint31_t s[16];
    uint32_t r1;
    uint32_t r2;
} ZUC;

#ifdef __cplusplus
extern "C" {
#endif

void ZUC_Init(ZUC* self, const uint8_t* key, const uint8_t* iv);

uint32_t ZUC_Generate(ZUC* self, uint8_t* word);

#ifdef __cplusplus
}
#endif

#endif // !GMALGLIB_ZUC_H
