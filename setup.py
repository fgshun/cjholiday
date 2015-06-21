#from distutils.core import setup, Extension
from setuptools import setup, Extension

py_modules = ['test_cjholiday']
extensions = [Extension('cjholiday', sources = ['cjholiday.c'])]

setup(
        name='cjholiday',
        version='1.0.3',
        author='fgshun',
        author_email='fgshun@lazy-moon.jp',
        url='http://www.lazy-moon.jp/',
        py_modules=py_modules,
        ext_modules=extensions)
