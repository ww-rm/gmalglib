#include <stdint.h>
#include <stdlib.h>
#include <gmalglib/random.h>


#ifdef _WIN32

#pragma comment(lib, "Bcrypt.lib")
#include <Windows.h>
#include <bcrypt.h>

#else

#endif // _WIN32


static int SimpleRandomProc(void* rand_obj, uint64_t bytes_len, uint8_t* bytes)
{
    uint64_t i;
    for (i = 0; i < bytes_len; i++)
    {
        bytes[i] = (uint8_t)rand();
    }

    return 1;
}

#ifdef _WIN32

static int _WinRandomProc(void* rand_obj, uint64_t bytes_len, uint8_t* bytes)
{
    ULONG chunk = 0;
    while (bytes_len > 0)
    {
        chunk = (ULONG)(bytes_len < ULONG_MAX ? bytes_len : ULONG_MAX);
        if (!BCRYPT_SUCCESS(BCryptGenRandom(NULL, bytes, chunk, BCRYPT_USE_SYSTEM_PREFERRED_RNG)))
            return 0;
        bytes += chunk;
        bytes_len -= chunk;
    }
    return 1;
}

#else

static int _LinuxRandomProc(void* rand_obj, uint64_t bytes_len, uint8_t* bytes)
{
    int fd = 0;
    fd = _Py_open_noraise("/dev/urandom", O_RDONLY);
    if (fd < 0)
        return 0;

    while (bytes_len > 0)
    {
        do {
            n = read(fd, buffer, (size_t)size);
        } while (n < 0 && errno == EINTR);

        if (n <= 0) {
            /* stop on error or if read(size) returned 0 */
            close(fd);
            return -1;
        }

        buffer += n;
        size -= n;
    }
    close(fd);
}

#endif // _WIN32


static int OsRandomProc(void* rand_obj, uint64_t bytes_len, uint8_t* bytes)
{
#ifdef _WIN32
    return _WinRandomProc(rand_obj, bytes_len, bytes);
#else
    return _LinuxRandomProc(rand_obj, bytes_len, bytes);
#endif // _WIN32
}

const RandomAlg _DEFAULT_RANDOM_ALGORITHM = { SimpleRandomProc, NULL };
const RandomAlg* const DEFAULT_RANDOM_ALGORITHM = &_DEFAULT_RANDOM_ALGORITHM;

int RandomBytes(RandomAlg* self, uint64_t bytes_len, uint8_t* bytes)
{
    return self->rand_proc(self->rand_obj, bytes_len, bytes);
}

int RandomUInt256(RandomAlg* self, const UInt256* max, UInt256* num)
{
    UInt256 num_tmp = { 0 };
    do
    {
        if (!RandomBytes(self, 32, num_tmp.u8))
            return 0;
    } while (UInt256_IsZero(&num_tmp) || UInt256_Cmp(&num_tmp, max) > 0);

    *num = num_tmp;
    return 1;
}
