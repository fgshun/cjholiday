from setuptools import setup, Extension

extensions = [Extension('cjholiday', sources=['cjholiday.c'])]

setup(
    headers=['cjholiday.h'],
    ext_modules=extensions,
)
