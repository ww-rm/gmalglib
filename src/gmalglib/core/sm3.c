#include <stdint.h>
#include <gmalglib/sm3.h>

static const
uint32_t ROL_T_TABLE[64] = {
    0x79cc4519, 0xf3988a32, 0xe7311465, 0xce6228cb, 0x9cc45197, 0x3988a32f, 0x7311465e, 0xe6228cbc,
    0xcc451979, 0x988a32f3, 0x311465e7, 0x6228cbce, 0xc451979c, 0x88a32f39, 0x11465e73, 0x228cbce6,
    0x9d8a7a87, 0x3b14f50f, 0x7629ea1e, 0xec53d43c, 0xd8a7a879, 0xb14f50f3, 0x629ea1e7, 0xc53d43ce,
    0x8a7a879d, 0x14f50f3b, 0x29ea1e76, 0x53d43cec, 0xa7a879d8, 0x4f50f3b1, 0x9ea1e762, 0x3d43cec5,
    0x7a879d8a, 0xf50f3b14, 0xea1e7629, 0xd43cec53, 0xa879d8a7, 0x50f3b14f, 0xa1e7629e, 0x43cec53d,
    0x879d8a7a, 0x0f3b14f5, 0x1e7629ea, 0x3cec53d4, 0x79d8a7a8, 0xf3b14f50, 0xe7629ea1, 0xcec53d43,
    0x9d8a7a87, 0x3b14f50f, 0x7629ea1e, 0xec53d43c, 0xd8a7a879, 0xb14f50f3, 0x629ea1e7, 0xc53d43ce,
    0x8a7a879d, 0x14f50f3b, 0x29ea1e76, 0x53d43cec, 0xa7a879d8, 0x4f50f3b1, 0x9ea1e762, 0x3d43cec5,
};

static const
uint32_t INIT_TABLE[8] = {
    0x7380166f, 0x4914b2b9, 0x172442d7, 0xda8a0600, 0xa96f30bc, 0x163138aa, 0xe38dee4d, 0xb0fb0e4e
};

static inline
uint32_t ROL(uint32_t X, uint32_t count)
{
    return (X << count) | (X >> (32 - count));
}

static inline
uint32_t FROM_BE(const uint8_t* bytes)
{
    return ((uint32_t)bytes[0] << 24) | 
           ((uint32_t)bytes[1] << 16) | 
           ((uint32_t)bytes[2] << 8)  |
           ((uint32_t)bytes[3]);
}

static inline
void TO_BE(uint32_t word, uint8_t* bytes)
{
    bytes[0] = (uint8_t)(word >> 24);
    bytes[1] = (uint8_t)(word >> 16);
    bytes[2] = (uint8_t)(word >> 8);
    bytes[3] = (uint8_t)(word);
}

static inline
uint32_t T(uint32_t i)
{
    return i <= 15 ? 0x79cc4519 : 0x7a879d8a;
}

static inline
uint32_t FF(uint32_t i, uint32_t X, uint32_t Y, uint32_t Z)
{
    return i <= 15 ? (X ^ Y ^ Z) : ((X & Y) | (X & Z) | (Y & Z));
}

static inline
uint32_t GG(uint32_t i, uint32_t X, uint32_t Y, uint32_t Z)
{
    return i <= 15 ? (X ^ Y ^ Z) : ((X & Y) | (~X & Z));
}

static inline
uint32_t P0(uint32_t X)
{
    return X ^ ROL(X, 9) ^ ROL(X, 17);
}

static inline
uint32_t P1(uint32_t X)
{
    return X ^ ROL(X, 15) ^ ROL(X, 23);
}

static inline
void Expand(const uint8_t* B, uint32_t* W1, uint32_t* W2)
{
    uint32_t i;
    for (i = 0; i < 16; i++)
    {
        W1[i] = FROM_BE(B + i * 4);
    }
    for (i = 16; i < 68; i++)
    {
        W1[i] = P1(W1[i - 16] ^ W1[i - 9] ^ ROL(W1[i - 3], 15)) ^ ROL(W1[i - 13], 7) ^ W1[i - 6];
    }
    for (i = 0; i < 64; i++)
    {
        W2[i] = W1[i] ^ W1[i + 4];
    }
}

static inline
void Compress(const uint32_t* W1, const uint32_t* W2, uint32_t* V)
{
    uint32_t i;
    uint32_t A = V[0];
    uint32_t B = V[1];
    uint32_t C = V[2];
    uint32_t D = V[3];
    uint32_t E = V[4];
    uint32_t F = V[5];
    uint32_t G = V[6];
    uint32_t H = V[7];
    uint32_t SS1, SS2, TT1, TT2;

    for (i = 0; i < 64; i++)
    {
        SS1 = ROL((ROL(A, 12) + E + ROL_T_TABLE[i]), 7);
        SS2 = SS1 ^ ROL(A, 12);
        TT1 = (FF(i, A, B, C) + D + SS2 + W2[i]);
        TT2 = (GG(i, E, F, G) + H + SS1 + W1[i]);
        D = C;
        C = ROL(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROL(F, 19);
        F = E;
        E = P0(TT2);
    }

    V[0] ^= A;
    V[1] ^= B;
    V[2] ^= C;
    V[3] ^= D;
    V[4] ^= E;
    V[5] ^= F;
    V[6] ^= G;
    V[7] ^= H;
}

static inline
void ExpandAndCompress(const uint8_t* B, uint32_t* V)
{
    uint32_t W1[68];
    uint32_t W2[64];
    Expand(B, W1, W2);
    Compress(W1, W2, V);
}

void SM3_Init(SM3* self)
{
    uint32_t i;
    for (i = 0; i < 8; i++)
    {
        self->value[i] = INIT_TABLE[i];
    }
    self->msg_buffer_length = 0;
    self->msg_bitlen = 0;
}

int SM3_Update(SM3* self, const uint8_t* data, uint64_t data_len)
{
    uint64_t data_idx;
    if ((data_len >> 61) || self->msg_bitlen + (data_len << 3) < self->msg_bitlen)
    {
        return SM3_ERR_OVERFLOW;
    }

    data_idx = 0;
    if (self->msg_buffer_length > 0)
    {
        while (data_idx < data_len && self->msg_buffer_length < 64)
        {
            self->msg_buffer[self->msg_buffer_length++] = data[data_idx++];
        }

        if (self->msg_buffer_length >= 64)
        {
            ExpandAndCompress(self->msg_buffer, self->value);
            self->msg_buffer_length = 0;
        }
    }

    while (data_idx + 63 < data_len)
    {
        ExpandAndCompress(data + data_idx, self->value);
        data_idx += 64;
    }

    while (data_idx < data_len)
    {
        self->msg_buffer[self->msg_buffer_length++] = data[data_idx++];
    }

    self->msg_bitlen += (data_len << 3);

    return 0;
}

void SM3_Digest(SM3* self, uint8_t* digest)
{
    uint32_t i;
    uint32_t value[8];
    uint8_t msg_buffer[64];
    uint64_t msg_buffer_length = self->msg_buffer_length;

    for (i = 0; i < 8; i++)
    {
        value[i] = self->value[i];
    }
    for (i = 0; i < msg_buffer_length; i++)
    {
        msg_buffer[i] = self->msg_buffer[i];
    }

    msg_buffer[msg_buffer_length++] = 0x80;

    if (msg_buffer_length > 56)
    {
        while (msg_buffer_length < 64)
        {
            msg_buffer[msg_buffer_length++] = 0x00;
        }
        ExpandAndCompress(msg_buffer, value);
        msg_buffer_length = 0;
    }

    while (msg_buffer_length < 56)
    {
        msg_buffer[msg_buffer_length++] = 0x00;
    }

    msg_buffer[56] = (uint8_t)(self->msg_bitlen >> 56);
    msg_buffer[57] = (uint8_t)(self->msg_bitlen >> 48);
    msg_buffer[58] = (uint8_t)(self->msg_bitlen >> 40);
    msg_buffer[59] = (uint8_t)(self->msg_bitlen >> 32);
    msg_buffer[60] = (uint8_t)(self->msg_bitlen >> 24);
    msg_buffer[61] = (uint8_t)(self->msg_bitlen >> 16);
    msg_buffer[62] = (uint8_t)(self->msg_bitlen >> 8);
    msg_buffer[63] = (uint8_t)(self->msg_bitlen);
    ExpandAndCompress(msg_buffer, value);

    for (i = 0; i < 8; i++)
    {
        TO_BE(value[i], digest + i * 4);
    }
}
