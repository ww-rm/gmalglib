from setuptools import Extension, setup

_core_module = Extension(
    name="gmalglib._core",
    sources=[
        "src/gmalglib/_coremodule.c",
        "src/gmalglib/sm3.c",
        "src/gmalglib/sm4.c",
    ],
    include_dirs=[
        "include",
        "include/python3",
    ],
    depends=[
        "include/gmalglib/sm3.h",
        "include/gmalglib/sm4.h",
    ]
)

setup(
    ext_modules=[
        _core_module
    ]
)
