# gmalglib 用户指南

欢迎来到 gmalglib! 一个国密算法的 Python 扩展库, 提供一些原始基础算法, 开箱即用.

项目仍在持续更新中, 前身是纯 Python 库 [gmalg](https://gmalg.readthedocs.io/), 用于学习和测试.

## 安装

```bash
pip install gmalglib
```

## 包含的算法

- SM2 椭圆曲线公钥密码算法
    - 签名验签
    - 密钥交换
    - 加密解密
- SM3 密码杂凑算法
- SM4 分组密码算法
- ZUC 序列密码算法

## 性能测试

`13th Gen Intel(R) Core(TM) i7-13700H` 上测试结果如下:

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

## 下一步

详细接口文档请前往 [API 参考](./api.md).

查看源代码请访问 Github 仓库 [gmalglib](https://github.com/ww-rm/gmalglib).
