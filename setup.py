from setuptools import Extension, setup

extensions = [Extension('cjholiday.cjholiday', sources=['src/cjholiday.c'])]

setup(
    headers=['src/cjholiday.h'],
    ext_modules=extensions,
)
