#!/usr/bin/env python

from setuptools import setup
from setuptools import find_packages
import re


def find_version():
    return re.search(r"^__version__ = '(.*)'$",
                     open('nala/version.py', 'r').read(),
                     re.MULTILINE).group(1)


setup(name='nala',
      version=find_version(),
      description='A test framework for C projects.',
      long_description=open('README.rst', 'r').read(),
      author='Erik Moqvist and Valentin Berlier',
      author_email='erik.moqvist@gmail.com',
      license='MIT',
      classifiers=[
          'License :: OSI Approved :: MIT License',
          'Programming Language :: Python :: 3',
      ],
      keywords=['c', 'test', 'mock'],
      url='https://github.com/eerimoq/nala',
      packages=find_packages(exclude=['tests']),
      test_suite="tests",
      install_requires=[
          'pycparser',
          'jinja2'
      ],
      include_package_data=True,
      entry_points = {
          'console_scripts': ['nala=nala.cli:main']
      })
