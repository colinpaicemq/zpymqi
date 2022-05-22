import setuptools
from setuptools import setup, Extension
import sysconfig
import os
bindings_mode = 1
version = '1.12.0'
setup(name = 'pymqi',
    version = version,
    description = 'Python IBM MQI Extension for IBM MQ (formerly WebSphere MQ and MQSeries).',
    long_description= 'PyMQI is a Python library ',
    author='Dariusz Suchojad',
    author_email='pymqi@m.zato.io',
    url='https://dsuch.github.io/pymqi/',
    download_url='https://pypi.python.org/pypi/pymqi',
    platforms='z/OS',
    package_dir = {'': 'code'},
    packages = ['pymqi'],
    license='Python Software Foundation License',
    keywords=('pymqi IBM MQ WebSphere WMQ MQSeries IBM middleware'),
    python_requires='>=3',
    classifiers = [
        'Development Status :: 5 - Production/Stable',
        'License :: OSI Approved :: Python Software Foundation License',
        'Intended Audience :: Developers',
        'Natural Language :: English',
        'Operating System :: OS Independent',
        'Programming Language :: C',
        'Programming Language :: Python',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Software Development :: Object Brokering',
        ],
    py_modules = ['pymqi.CMQC', 'pymqi.CMQCFC', 'pymqi.CMQXC', 'pymqi.CMQZC'],
    )
#       extra_link_args=["-Wl,INFO,LIST,MAP","//'COLIN.MQ924.SCSQDEFS.OBJ(CSQBMQ2X)'"],
#       extra_link_args=["//'COLIN.MQ924.SCSQDEFS.OBJ(CSQBMQ2X)'"],
#       extra_compile_args=["-Wc,LIST(c.lst),XREF"]
#   ext_modules = [Extension('pymqi.pymqe',['code/pymqi/pymqe.c'], define_macros=[('PYQMI_BINDINGS_MODE_BUILD',
#   ext_modules = [Extension('pymqi.pymqe',['code/pymqi/cpmqe.c'], define_macros=[('PYQMI_BINDINGS_MODE_BUILD',
#   bindings_mode)],
#       include_dirs=["//'COLIN.MQ924.SCSQC370'"],
#       extra_compile_args=["-Wc,LIST(/u/tmp/py/c.lst),XREF"],
#       extra_link_args=["-Wl,LIST,MAP,DLL","//'COLIN.MQ924.SCSQDEFS.OBJ(CSQBMQ2X)'"],
#       )]
#   ext_modules = [Extension('pymqi.pymqe',['code/pymqi/pymqe.c'], define_macros=[('PYQMI_BINDINGS_MODE_BUILD',
#       extra_compile_args=["-Wc,LIST(c.lst),XREF,SO"],
#       extra_compile_args=["-Wc,LIST(c.lst),XREF,SO"],
#       extra_compile_args=["-Wc,LIST(c.lst),XREF"],
#       extra_link_args=["-Wl,LIST=ALL,MAP,DLL,XREF,DYNAM=DLL","//'COLIN.MQ924.SCSQDEFS.OBJ(CSQBMQ2X)'"],
