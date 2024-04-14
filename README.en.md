# gmalglib

[![Unittest](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml)
[![PyPI](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml)

Python extension library for GM (GuoMi) cryptographic algorithms, providing a set of fundamental cryptographic algorithms.

Implemented in C language, encapsulated based on the native CPython interface, without dependencies on any third-party libraries.

## Installation

```bat
pip install gmalglib
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
