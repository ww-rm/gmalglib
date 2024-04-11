from setuptools import Extension, setup

core_module = Extension(
    name="gmalglib.core",
    sources=[
        "src/gmalglib/coremodule.c",
        "src/gmalglib/core/sm3.c",
        "src/gmalglib/core/sm4.c",
        "src/gmalglib/core/zuc.c",
        "src/gmalglib/core/bignum.c",
        "src/gmalglib/core/random.c",
        "src/gmalglib/core/sm2curve.c",
        "src/gmalglib/core/sm2.c",
    ],
    include_dirs=[
        "include",
        "include/python3",
    ],
    depends=[
        "include/gmalglib/sm3.h",
        "include/gmalglib/sm4.h",
        "include/gmalglib/zuc.h",
        "include/gmalglib/bignum.h",
        "include/gmalglib/random.h",
        "include/gmalglib/sm2curve.h",
        "include/gmalglib/sm2.h",
    ]
)

setup(
    ext_modules=[
        core_module
    ]
)
