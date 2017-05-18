# -*- coding: utf-8 -*-
#
#	Autori: Tosatto Davide, Riccardo Grespan
#
#	Script per compilare ed installare il modulo proxsensor, scritto in C
#   Comando da eseguire: sudo python setup.py install
#

from distutils.core import setup, Extension

module1 = Extension('hx711',
                    include_dirs = ['/usr/local/include' ,'/usr/local/lib'],
                    libraries = ['wiringPi'],
                    library_dirs = ['/usr/local/lib'],
                    sources = ['hx711.c', 'pythonizedHX711.c'])

setup (name = 'hx711',
       version = '0.1',
       description = 'Packet for controlling the ADC HX711',
       ext_modules = [module1])
