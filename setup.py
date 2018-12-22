from setuptools import Extension, setup

extensions = [Extension('cjholiday', sources=['cjholiday.c'])]

setup(
    headers=['cjholiday.h'],
    ext_modules=extensions,
)
