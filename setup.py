#!/usr/bin/env python
from distutils.core import setup
from os.path import join

with open('docs/Features.rst') as file:
    long_description = file.read()

setup(
    name='regex',
    version='2019.12.9',
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

    package_dir={'regex': 'regex'},
    py_modules=['regex.__init__', 'regex.regex', 'regex._regex_core',
     'regex.test_regex'],
    data_files=['regex._regex.pyd'],
)
