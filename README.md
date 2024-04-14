# gmalglib

[![Unittest](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-test.yml)
[![PyPI](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml/badge.svg)](https://github.com/ww-rm/gmalglib/actions/workflows/python-publish.yml)

国密算法的 Python 扩展库, 提供一些原始基础算法.

使用 C 语言实现, 基于原生 CPython 接口进行封装, 不依赖其他任何第三方库.

## 安装

```bat
pip install gmalglib
```

## 已实现的核心算法

- SM3 密码杂凑算法
    - 消息摘要
    - 密钥派生
    - 消息认证
- SM4 分组密码算法
- ZUC 序列密码算法
- SM2 椭圆曲线公钥密码算法
    - 签名验签
    - 加密解密

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
