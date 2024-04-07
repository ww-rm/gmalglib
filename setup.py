from setuptools import Extension, setup

core_module = Extension(
    name="gmalglib.core",
    sources=[
        "src/gmalglib/coremodule.c",
        "src/gmalglib/core/sm3.c",
        "src/gmalglib/core/sm4.c",
        "src/gmalglib/core/zuc.c",
    ],
    include_dirs=[
        "include",
        "include/python3",
    ],
    depends=[
        "include/gmalglib/sm3.h",
        "include/gmalglib/sm4.h",
        "include/gmalglib/zuc.h",
    ]
)

setup(
    ext_modules=[
        core_module
    ]
)
