from setuptools import Extension, setup

extensions = [Extension('cjholiday.cjholiday', sources=['cjholiday.c'])]

setup(
    headers=['cjholiday.h'],
    ext_modules=extensions,
)
