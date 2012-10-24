#!/usr/bin/env python

NAME = 'pywdns'
VERSION = '0.4'

from distutils.core import setup
from distutils.extension import Extension
import os

print 'executing ./gen_pywdns_constants'
os.system('./gen_pywdns_constants')

try:
    from Cython.Distutils import build_ext
    setup(
        name = NAME,
        version = VERSION,
        ext_modules = [ Extension('wdns', ['wdns.pyx'], libraries = ['wdns']) ],
        cmdclass = {'build_ext': build_ext},
        py_modules = ['wdns_constants'],

    )
except ImportError:
    setup(
        name = NAME,
        version = VERSION,
        ext_modules = [ Extension('wdns', ['wdns.c'], libraries = ['wdns']) ],
        py_modules = ['wdns_constants'],
    )