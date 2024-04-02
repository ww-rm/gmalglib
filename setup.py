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

setup(
    ext_modules=[
        sm3_module
    ]
)
