#include <stdint.h>
#include <gmalglib/sm2.h>

static const uint8_t _SM2_DEFAULT_UID[SM2_DEFAULT_UID_LENGTH] = {
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
};
const uint8_t* const SM2_DEFAULT_UID = _SM2_DEFAULT_UID;

static inline
int _SM2_EntityInfo(const uint8_t* uid, uint16_t* uid_len, const SM2JacobPointMont* pk, uint8_t* entity_info)
{
    
}

int SM2_Init(
    SM2* self,
    const uint8_t* sk,
    const uint8_t* pk,
    const uint8_t* uid,
    uint16_t uid_len,
    int pc_mode,
    RandomBytesFunction rand_func,
    void* rand_obj
)
{
    uint32_t i;

    // check and parse sk
    // self->sk;

    // check and parse pk
    // self.pk

    // check and get entity_info
    // TODO: check return value
    if (!uid)
    {
        uid = SM2_DEFAULT_UID;
        uid_len = SM2_DEFAULT_UID_LENGTH;
    }
    if (uid_len > SM2_UID_MAX_LENGTH)
        return SM2_ERR_UID_OVERFLOW;

    _SM2_EntityInfo(uid, uid_len, &self->pk, self->entity_info);

    // check and parse pc_mode
    if (pc_mode != SM2_PCMODE_RAW && pc_mode != SM2_PCMODE_COMPRESS && pc_mode != SM2_PCMODE_MIX)
        return SM2_ERR_INVALID_PCMODE;
    self->pc_mode = pc_mode;

    // store rand function and rand obj
    if (!rand_func)
    {
        rand_func = DefaultRandomBytes;
        rand_obj = NULL;
    }
    self->rand_func = rand_func;
    self->rand_obj = rand_obj;

    return 0;
}

int SM2_Sign()
{

}
