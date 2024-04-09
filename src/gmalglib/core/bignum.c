#include <stdint.h>
#include <gmalglib/bignum.h>

int UInt256_Cmp(const UInt256* x, const UInt256* y)
{
    if (x->u64[3] > y->u64[3]) return  1;
    else if (x->u64[3] < y->u64[3]) return -1;
    else if (x->u64[2] > y->u64[2]) return  1;
    else if (x->u64[2] < y->u64[2]) return -1;
    else if (x->u64[1] > y->u64[1]) return  1;
    else if (x->u64[1] < y->u64[1]) return -1;
    else if (x->u64[0] > y->u64[0]) return  1;
    else if (x->u64[0] < y->u64[0]) return -1;

    return 0;
}

uint8_t UInt256_Add(const UInt256* x, const UInt256* y, UInt256* z)
{
    uint32_t i = 0;
    uint64_t carry = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 4; i++)
    {
        tmp = x->u64[i] + carry;
        carry = tmp < x->u64[i];
        z->u64[i] = tmp + y->u64[i];
        carry += z->u64[i] < tmp;
    }
    return (uint8_t)carry;
}

uint8_t UInt256_Sub(const UInt256* x, const UInt256* y, UInt256* z)
{
    uint32_t i = 0;
    uint64_t borrow = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 4; i++)
    {
        tmp = x->u64[i] - borrow;
        borrow = tmp > x->u64[i];
        z->u64[i] = tmp - y->u64[i];
        borrow += z->u64[i] > tmp;
    }
    return (uint8_t)borrow;
}

void UInt256_Mul(const UInt256* x, const UInt256* y, UInt512* z)
{
    UInt512 z_tmp = { 0 };
    uint64_t carry = 0;

    uint32_t i;
    uint32_t j;

    for (i = 0; i < 8; i++)
    {
        carry = 0;
        for (j = 0; j < 8; j++)
        {
            carry += (uint64_t)z_tmp.u32[i + j] + (uint64_t)x->u32[i] * (uint64_t)y->u32[j];
            z_tmp.u32[i + j] = (uint32_t)carry;
            carry >>= 32;
        }
        z_tmp.u32[i + 8] = (uint32_t)carry;
    }

    *z = z_tmp;
}

uint8_t UInt512_Add(const UInt512* x, const UInt512* y, UInt512* z)
{
    uint32_t i = 0;
    uint64_t carry = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 8; i++)
    {
        tmp = x->u64[i] + carry;
        carry = tmp < x->u64[i];
        z->u64[i] = tmp + y->u64[i];
        carry += z->u64[i] < tmp;
    }
    return (uint8_t)carry;
}
