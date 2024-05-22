# gmalglib

[![Unittest](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml)
[![PyPI](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml)
[![docs](https://readthedocs.org/projects/gmalglib/badge/?version=latest)](https://gmalglib.readthedocs.io/zh-cn/latest/?badge=latest)

Python extension library for GM (GuoMi) cryptographic algorithms, providing a set of fundamental cryptographic algorithms.

Implemented in C language, encapsulated based on the native CPython interface, without dependencies on any third-party libraries.

## Installation

```bash
pip install gmalglib
```

## Core Algorithms Implemented

- SM2 Public Key Cryptograhpic Algorithm Based on Elliptic Curves
  - Sign/Verify
  - Key exchange 
  - Encrypt/Decrypt
- SM3 Cryptogrpahic Hash Algorithm
- SM4 Block Cipher Algorithm
- ZUC Stream Cipher Algorithm

## Usage

For submodules under gmalglib, different algorithm encapsulations are respectively exported, and can be utilized in an object-oriented manner.

```python
from gmalglib.sm3 import SM3

obj = SM3()
obj.update(b"message")
obj.update(b"digest")
print(obj.digest().hex())
```

Under gmalglib.wrapped, member methods of all algorithm objects are encapsulated, providing a procedural call method. Furthermore, the gmalglib namespace is imported, enabling direct usage.

```python
import gmalglib

print(gmalglib.sm3_digest(b"messagedigest").hex())
```

## About Random Number Generators

For all sections involving random number generators, custom parameters for random number generation are provided, implemented in the form of callback functions. The function type is `Callable[[int], bytes]`, meaning it generates a byte string of a specified length.

```python
def rnd_fn(n: int) -> bytes: ...
```

If no random number generator is passed, the default system-related random number generator is used. On Windows, it utilizes `BCryptGenRandom`, while other systems use `/dev/urandom` for implementation, which is similar to the Python standard library function [`os.urandom`](https://docs.python.org/3/library/os.html#os.urandom).

For specific implementation details, refer to [random.c](https://github.com/ww-rm/gmalglib/blob/main/src/gmalglib/core/random.c) under the `OsRandomProc` function.

## Benchmark Test

The benchmark test code can be found in `benchmark.py`. The test results on the `13th Gen Intel(R) Core(TM) i7-13700H` are as follows:

```plain
==================== SM2 Benchmark Test (1000 times, 32 bytes data) ====================
SM2.encrypt             : 2.391558s
SM2.decrypt             : 1.092445s
SM2.sign_digest         : 1.062552s
SM2.verify_digest       : 2.096187s
SM2.sign                : 1.067850s
SM2.verify              : 2.055190s
SM2.begin_key_exchange  : 1.159822s
SM2.end_key_exchange    : 1.633471s
==================== SM3 Benchmark Test (1,000,000,000 bytes data) ====================
SM3.update & SM3.digest : 5.118763s
==================== SM4 Benchmark Test (1000000 times) ====================
SM4.encrypt             : 0.369991s
SM4.decrypt             : 0.297077s
==================== ZUC Benchmark Test (1000000 times) ====================
zuc.generate            : 0.050301s
```
