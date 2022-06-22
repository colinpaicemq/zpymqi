import setuptools
from setuptools import setup, Extension
import sysconfig
import os
os.environ['_C89_CCMODE'] = '1'
# The xlc compiler expects the oject library to have .obj at the end of the DSN.
# I created a library MQM.V920.SCSQDEFS.OBJ and copied MQM.V920.SCSQDEFS(*) 
# into it
#
# This script needs    export _C89_CCMODE=1
# Otherwise you get FSUM3008  messages
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
    package_dir = {'': '.'},
    packages = ['pymqi'],
    license='Python Software Foundation License',
    keywords=('pymqi IBM MQ WebSphere WMQ MQSeries IBM middleware'),
    python_requires='>=3',
    classifiers = [
        'Development Status :: 4 - Beta',
        'License :: OSI Approved :: Python Software Foundation License',
        'Intended Audience :: Developers',
        'Natural Language :: English',
        'Operating System :: OS Independent',
        'Programming Language :: C',
        'Programming Language :: Python',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: Software Development :: Object Brokering',
        ],
    ext_modules = [Extension('pymqi.zpymqe',['pymqe.c'], define_macros=[('PYQMI_BINDINGS_MODE_BUILD',
    bindings_mode)],
          include_dirs=["//'MQM.V920.SCSQC370'",".//"], 
          extra_compile_args=["-Wc,LIST(/u/colin/c.lst),SOURCE,NOWARN64,XREF"], 
          extra_link_args=["-Wl,LIST,MAP,DLL","//'MQM.V920.SCSQDEFS.OBJ(CSQBMQ2X)'"], 
        )]
    )

