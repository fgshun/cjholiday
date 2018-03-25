from setuptools import setup, Extension

py_modules = ['test_cjholiday']
extensions = [Extension('cjholiday', sources = ['cjholiday.c'])]

setup(
        name='cjholiday',
        version='1.1.3',
        author='fgshun',
        author_email='fgshun@gmail.com',
        url='http://www.lazy-moon.jp/',
        py_modules=py_modules,
        headers=['cjholiday.h'],
        ext_modules=extensions)
