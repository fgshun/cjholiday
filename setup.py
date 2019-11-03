from setuptools import Extension, setup

extensions = [Extension('cjholiday._cjholiday', sources=['cjholiday.c'])]

setup(
    headers=['cjholiday.h'],
    ext_modules=extensions,
)
