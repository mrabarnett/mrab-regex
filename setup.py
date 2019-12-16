#!/usr/bin/env python
from setuptools import setup, Extension
from os.path import join

with open('docs/Features.rst') as file:
    long_description = file.read()

src_dir = 'regex_3'

setup(
    name='regex',
    version='2019.11.1',
    description='Alternative regular expression module, to replace re.',
    long_description=long_description,
    author='Matthew Barnett',
    author_email='regex@mrabarnett.plus.com',
    url='https://bitbucket.org/mrabarnett/mrab-regex',
    license='Python Software Foundation License',

    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Python Software Foundation License',
        'Operating System :: OS Independent',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Topic :: Scientific/Engineering :: Information Analysis',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Text Processing',
        'Topic :: Text Processing :: General',
    ],
    setup_requires=['wheel'],

    package_dir={'': src_dir},
    py_modules=['__init__', 'regex', '_regex_core',
     'test_regex'],
    ext_modules=[Extension('_regex', [
        "{}/{}".format(src_dir, s) for s in ['_regex.c', '_regex_unicode.c']
    ])],
)
