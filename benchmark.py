import secrets
from timeit import timeit

import gmalglib


def benchmark_sm2(times: int = 1000, data_len: int = 32):
    print(f"==================== SM2 Benchmark Test ({times} times, {data_len} bytes data) ====================")

    skA, pkA = gmalglib.sm2.SM2().generate_keypair()
    skB, pkB = gmalglib.sm2.SM2().generate_keypair()

    sm2A = gmalglib.sm2.SM2(skA)
    sm2B = gmalglib.sm2.SM2(skB)

    data = secrets.token_bytes(data_len)

    t = timeit("sm2A.encrypt(data)", number=times, globals=locals())
    print(f"SM2.encrypt\t\t: {t:.6f}s")

    cipher = sm2A.encrypt(data)

    t = timeit("sm2A.decrypt(cipher)", number=times, globals=locals())
    print(f"SM2.decrypt\t\t: {t:.6f}s")

    t = timeit("sm2A.sign_digest(data)", number=times, globals=locals())
    print(f"SM2.sign_digest\t\t: {t:.6f}s")

    signature = sm2A.sign_digest(data)
    t = timeit("sm2A.verify_digest(data, signature)", number=times, globals=locals())
    print(f"SM2.verify_digest\t: {t:.6f}s")

    t = timeit("sm2A.sign(data)", number=times, globals=locals())
    print(f"SM2.sign\t\t: {t:.6f}s")

    signature = sm2A.sign(data)
    t = timeit("sm2A.verify(data, signature)", number=times, globals=locals())
    print(f"SM2.verify\t\t: {t:.6f}s")

    t = timeit("sm2A.begin_key_exchange()", number=times, globals=locals())
    print(f"SM2.begin_key_exchange\t: {t:.6f}s")

    tA, RA = sm2A.begin_key_exchange()
    tB, RB = sm2B.begin_key_exchange()

    t = timeit("sm2B.end_key_exchange(tB, RA, pkA, True, 16)", number=times, globals=locals())
    print(f"SM2.end_key_exchange\t: {t:.6f}s")


def benchmark_sm3(data_len: int = 1000000000):
    print(f"==================== SM3 Benchmark Test ({data_len:,} bytes data) ====================")

    sm3 = gmalglib.sm3.SM3()
    data = secrets.token_bytes(data_len)
    t = timeit("sm3.update(data); sm3.digest()", number=1, globals=locals())
    print(f"SM3.update & SM3.digest\t: {t:.6f}s")


def benchmark_sm4(times: int = 1000000):
    print(f"==================== SM4 Benchmark Test ({times} times) ====================")

    key = secrets.token_bytes(16)
    plain = secrets.token_bytes(16)

    sm4 = gmalglib.sm4.SM4(key)

    t = timeit("sm4.encrypt(plain)", number=times, globals=locals())
    print(f"SM4.encrypt\t\t: {t:.6f}s")

    cipher = sm4.encrypt(plain)
    t = timeit("sm4.decrypt(cipher)", number=times, globals=locals())
    print(f"SM4.decrypt\t\t: {t:.6f}s")


def benchmark_zuc(times: int = 1000000):
    print(f"==================== ZUC Benchmark Test ({times} times) ====================")

    key = secrets.token_bytes(16)
    iv = secrets.token_bytes(16)

    zuc = gmalglib.zuc.ZUC(key, iv)

    t = timeit("zuc.generate()", number=times, globals=locals())
    print(f"zuc.generate\t\t: {t:.6f}s")


if __name__ == "__main__":
    functions = dict(globals())
    for k, v in functions.items():
        if k.startswith("benchmark"):
            v()
