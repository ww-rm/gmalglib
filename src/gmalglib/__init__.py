"""国密算法库

Raises:
    OverflowError: 数据长度超出范围限制.
    ValueError: 参数取值不正确.
    RuntimeError: 运行错误, 例如随机数生成失败.
    AttributeError: 缺少某些必需参数.
    MemoryError: 内存不足.
"""

from . import sm2
from . import sm3
from . import sm4
from . import zuc
from .wrapped import *

__version__ = "0.5.3"
