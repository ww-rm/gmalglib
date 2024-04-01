from setuptools import Extension, setup

setup(
    ext_modules=[
        Extension(
            "gmalglib.sm3",
            ["sm3/sm3module.c", "sm3/sm3.c"],
            ["sm3"],
            depends=["sm3/sm3.h"],
            py_limited_api=True
        )
    ]
)
