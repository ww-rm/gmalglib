# gmalglib

[![Unittest](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml)
[![PyPI](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml)
[![docs](https://readthedocs.org/projects/gmalglib/badge/?version=latest)](https://gmalglib.readthedocs.io/zh-cn/latest/?badge=latest)

Python extension library for GM (GuoMi) cryptographic algorithms, providing a set of fundamental cryptographic algorithms.

Implemented in C language, encapsulated based on the native CPython interface, without dependencies on any third-party libraries.

## Installation

### Windows

For `python3.8` and higher versions, you can directly install using `pip`.

```bash
pip install gmalglib
```

Alternatively, refer to the source code installation for other platforms.

### Other Platforms

Visit the PyPI project file list [Download files](https://pypi.org/project/gmalglib/#files) page to download the source distribution package `gmalglib-x.y.z.tar.gz`, then proceed with the source code installation.

```bash
pip install gmalglib-x.y.z.tar.gz
```

## Core Algorithms Implemented

- SM2 Public Key Cryptograhpic Algorithm Based on Elliptic Curves
  - Sign/Verify
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
