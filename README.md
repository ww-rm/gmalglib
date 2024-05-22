# gmalglib

[![Unittest](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml)
[![PyPI](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml)
[![docs](https://readthedocs.org/projects/gmalglib/badge/?version=latest)](https://gmalglib.readthedocs.io/zh-cn/latest/?badge=latest)

国密算法的 Python 扩展库, 提供一些原始基础算法.

使用 C 语言实现, 基于原生 CPython 接口进行封装, 不依赖其他任何第三方库.

## 安装

```bash
pip install gmalglib
```

## 已实现的核心算法

- SM2 椭圆曲线公钥密码算法
    - 签名验签
    - 密钥交换
    - 加密解密
- SM3 密码杂凑算法
    - 消息摘要
    - 密钥派生
    - 消息认证
- SM4 分组密码算法
- ZUC 序列密码算法

## 用法

对于 `gmalglib` 下的子模块, 分别导出了不同算法封装后结构, 按面向对象方式使用.

```python
from gmalglib.sm3 import SM3

obj = SM3()
obj.update(b"message")
obj.update(b"digest")
print(obj.digest().hex())
```

`gmalglib.wrapped` 下包装了所有算法对象的成员方法, 提供面向过程的调用方式, 且已导入 `gmalglib` 命名空间, 可以直接使用.

```python
import gmalglib

print(gmalglib.sm3_digest(b"messagedigest").hex())
```

## 关于随机数发生器

所有涉及随机数发生器的部分, 均提供了自定义随机数发生器的参数, 以回调函数方式实现, 函数类型为 `Callable[[int], bytes]`, 即生成指定长度的随机字节串.

```python
def rnd_fn(n: int) -> bytes: ...
```

如不传入随机数发生器, 则使用默认的系统相关随机数发生器, 在 Windows 下使用 `BCryptGenRandom`, 其余系统使用 `/dev/urandom` 实现, 其实现类似于 Python 标准库函数 [`os.urandom`](https://docs.python.org/3/library/os.html#os.urandom).

具体实现见 [random.c](https://github.com/ww-rm/gmalglib/blob/main/src/gmalglib/core/random.c) 内 `OsRandomProc`.

## 性能测试

性能测试代码见 [benchmark.py](https://github.com/ww-rm/gmalglib/blob/main/benchmark.py), `13th Gen Intel(R) Core(TM) i7-13700H` 上测试结果如下:

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
