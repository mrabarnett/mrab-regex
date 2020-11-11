#!/usr/bin/env python
from setuptools import setup, Extension
from os.path import join
import sys

MAJOR, MINOR = sys.version_info[:2]

SRC_BASE = 'regex_%i' % MAJOR

with open('README.rst') as file:
    long_description = file.read()

setup(
    name='regex',
    version='2020.11.11',
    description='Alternative regular expression module, to replace re.',
    long_description=long_description,
    author='Matthew Barnett',
    author_email='regex@mrabarnett.plus.com',
    url='https://bitbucket.org/mrabarnett/mrab-regex',
    license='Apache Software License',

    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Apache Software License',
        'Operating System :: OS Independent',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Topic :: Scientific/Engineering :: Information Analysis',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Text Processing',
        'Topic :: Text Processing :: General',
    ],

    package_dir={'regex': SRC_BASE},
    py_modules=['regex.__init__', 'regex.regex', 'regex._regex_core',
     'regex.test_regex'],
    ext_modules=[Extension('regex._regex', [join(SRC_BASE, '_regex.c'),
      join(SRC_BASE, '_regex_unicode.c')])],
)
