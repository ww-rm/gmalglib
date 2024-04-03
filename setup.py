from setuptools import Extension, setup

sm3_module = Extension(
    name="gmalglib.sm3",
    sources=[
        "src/gmalglib/sm3module.c",
        "src/gmalglib/sm3.c"
    ],
    include_dirs=[
        "include",
        "include/python3"
    ],
    depends=[
        "include/gmalglib/sm3.h"
    ]
)

sm4_module = Extension(
    name="gmalglib.sm4",
    sources=[
        "src/gmalglib/sm4module.c",
        "src/gmalglib/sm4.c"
    ],
    include_dirs=[
        "include",
        "include/python3"
    ],
    depends=[
        "include/gmalglib/sm4.h"
    ]
)

setup(
    ext_modules=[
        sm3_module,
        sm4_module,
    ]
)
