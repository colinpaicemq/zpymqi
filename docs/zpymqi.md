# Running pymqi on native z/OS

PyMQI on z/OS is a port of [PyMQI](https://dsuch.github.io/pymqi/) from Zato.  The z/OS version has been extended, to make PCF easier to use, and to add tracing capabilities to make it easier to debug the MQ flows.

## Pre requisites

This code has been developed and tested with MQ V9.  It should work with MQ version 8, but has not been tested.

This code was developed and tested with Python V3.8 on z/OS( Libraries /usr/lpp/IBM/cyp/v3r8/pyz/bin/python3) .  It will not work with Python V2.

## Installation

You need to copy the zpymqi.zip file to z/OS and unzip it into a directory.  You cannot use PIP to install it because of the naming convention used by PIP.

The native z/OS support for pymqi has some *.py files and a module zpymqe.so.

### Setup


You need to set up environment variable PYTHONPATH to point to the code. You also need to set up STEPLIB.

For example, in your .profile shell script

Code ::

       #export PATH=$PATH:$HOME: #set and export PATH variable 
       # set up Python path 
       export PATH=/usr/lpp/IBM/cyp/v3r8/pyz/bin:$PATH 
       # set up MQ Libraries
       export STEPLIB=MQM.V900.SCSQANLE:MQM.V900.SCSQANLE:MQM.V900.SCSQAUTH:$STEPLIB 
       # set up PYTHONPATH to point to where zpymqi is
       export PYTHONPATH=/u/betacp/pymqi:$PYTHONPATH 
     

Without this you will get exceptions like::
  ImportError: cannot import name 'zpymqe' from partially initialized module 'pymqi'

There is python code to most of the work, and there is a load module object (zpymqi) for issuing the MQ API requests.  

The zpymqi code is written in C, and compiled as ASCII. When pymqi calls the zpymqi code, the values are passed across in ASCII.  The pymqi/\_\_init\_\_.py code translates the ASCII code to EBCDIC, where possible.  It is possible that applications may have to translate data themselves, or set the appropriate options for MQ to do the translation.  If you hit this problem, please let the author know so the code can be updated.

### Using Python on z/OS

Python on z/OS has some interesting quirks.  I've documented some of them [here](https://colinpaice.blog/2022/01/04/python-on-z-os-helpful-hints/). 

## Acknowledgements

This code was based on pymqi from Zato, and the work by Darusz Suchojad.  Without him (and others) there would be no pymqi code.

## Problems
Any problems with Pymqi on z/OS should be reported to colinpaicemq@gmail.com.


# MQ Python specific topics

[Using the API](UsingApi.md)

[API referennce](APIReference.md)

[Connect and disconnect](ConnDisk.md)

[Creating messages](Messages.md)

[Opening and closing a queue](OpenClose.md)

[Simple put](SimplePut.md)

[Getting messages from a queue](get.md)

[Message properties and Message Handle](msgprop.md)

[Commit and Backout](commit.md)

[Inquire and Set](inquire.md)

[Using PCF](zpymqi_pcf.md)