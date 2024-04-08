#include <stdint.h>
#include <gmalglib/primefield.h>

uint8_t U256_Add(const uint256_t* x, const uint256_t* y, uint256_t* z)
{
    uint64_t* x_parts = (uint64_t*)x;
    uint64_t* y_parts = (uint64_t*)y;
    uint64_t* z_parts = (uint64_t*)z;
    uint32_t i = 0;
    uint64_t carry = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 4; i++)
    {
        tmp = x_parts[i] + carry;
        carry = tmp < x_parts[i];
        z_parts[i] = tmp + y_parts[i];
        carry += z_parts[i] < tmp;
    }
    return (uint8_t)carry;
}

uint8_t U256_Sub(const uint256_t* x, const uint256_t* y, uint256_t* z)
{
    uint64_t* x_parts = (uint64_t*)x;
    uint64_t* y_parts = (uint64_t*)y;
    uint64_t* z_parts = (uint64_t*)z;
    uint32_t i = 0;
    uint64_t borrow = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 4; i++)
    {
        tmp = x_parts[i] - borrow;
        borrow = tmp > x_parts[i];
        z_parts[i] = tmp - y_parts[i];
        borrow += z_parts[i] > tmp;
    }
    return (uint8_t)borrow;
}

void U256_Mul(const uint256_t* x, const uint256_t* y, uint512_t* z)
{
    uint32_t* x_parts = (uint32_t*)x;
    uint32_t* y_parts = (uint32_t*)y;
    uint32_t* z_parts = (uint32_t*)z;
    uint64_t carry = 0;
    
    uint32_t i;
    uint32_t j;

    for (i = 0; i < 16; i++)
    {
        z_parts[i] = 0;
    }

    for (i = 0; i < 8; i++)
    {
        carry = 0;
        for (j = 0; j < 8; j++)
        {
            carry += (uint64_t)z_parts[i + j] + (uint64_t)x_parts[i] * (uint64_t)y_parts[j];
            z_parts[i + j] = (uint32_t)carry;
            carry >>= 32;
        }
        z_parts[i + 8] = (uint32_t)carry;
    }
}

uint8_t U512_Add(const uint512_t* x, const uint512_t* y, uint512_t* z)
{
    uint64_t* x_parts = (uint64_t*)x;
    uint64_t* y_parts = (uint64_t*)y;
    uint64_t* z_parts = (uint64_t*)z;
    uint32_t i = 0;
    uint64_t carry = 0;
    uint64_t tmp = 0;
    for (i = 0; i < 8; i++)
    {
        tmp = x_parts[i] + carry;
        carry = tmp < x_parts[i];
        z_parts[i] = tmp + y_parts[i];
        carry += z_parts[i] < tmp;
    }
    return (uint8_t)carry;
}
