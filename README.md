# zpymqi

`zpymqi` is a port of [PyMQI](https://dsuch.github.io/pymqi/) from Zato to run natively on the z/OS platform.  The z/OS version has been extended, to make PCF easier to use, and to add tracing capabilities to make it easier to debug the MQ flows.

If you have already installed PyMQI, refer to [Getting Started with PyMQI on z/OS](#getting-started-with-pymqi-on-zos)

## Pre-requisites

This code has been developed and tested in the following environment:

- [IBM MQ for z/OS Version 9](https://www.ibm.com/uk-en/products/mq/zos). *Compatibility with older versions of MQ is not guaranteed.*
- [IBM Open Enterprise SDK for Python v3.8](https://www.ibm.com/products/open-enterprise-python-zos) (Libraries /usr/lpp/IBM/cyp/v3r8/pyz/bin/python3). *This code does not work with any Python 2 release*

----------

## Installation

*Currently, you cannot use `python pip` to install zpymqi*

### Steps

1. Create the directory you want to install zpymqi into (for example `/u/colin/pymqi`)
2. Copy the latest `zpymqi.pax` into this directory using a file transfer tool (SFTP, SCP etc).
3. Extract the source code using `pax -rf /u/colin/zpymqi.pax .`

If you list this directory you should now see something like:

```text
- -rwxr-xr--   1 OMVSKERN SYS1        2302 Feb 11 15:53 CMQZC.py                                  
- -rwxr-xr--   1 OMVSKERN SYS1        6303 Feb 11 15:53 CMQXC.py                                  
- -rwxr-xr--   1 OMVSKERN SYS1       53843 Feb 11 15:53 CMQCFC.py                                 
- -rwxr-xr--   1 OMVSKERN SYS1       63317 Feb 11 15:53 CMQC.py                                   
- -rwxr-xr--   1 OMVSKERN SYS1        4022 Mar 15 17:00 MQOTHER.py                                
- -rwxr-xr--   1 OMVSKERN SYS1      150352 May 16 13:05 __init__.py                               
- -rwxr-xr--   1 OMVSKERN SYS1       14824 May 17 17:20 PCF.py                                    
- -rwxr-xr--   1 OMVSKERN SYS1     1466368 May 22 15:15 zpymqe.so                                 
```

----------

### System Configuration

You need to set up environment variable PYTHONPATH to point to the code. You also need to set up STEPLIB.

For example, in your `.profile` shell script:

```sh
# set up Python path 
export PATH=/usr/lpp/IBM/cyp/v3r8/pyz/bin:$PATH 

# set up MQ Libraries
export STEPLIB=MQM.V900.SCSQANLE:MQM.V900.SCSQANLE:MQM.V900.SCSQAUTH:$STEPLIB 

# set up PYTHONPATH to point to where the PyMQI directory is 
export PYTHONPATH=/u/colin/:$PYTHONPATH 
```

Without this you will get exceptions like:

```text
ImportError: cannot import name 'zpymqe' from partially initialized module 'pymqi'
```

There is Python code to most of the work, and there is a load module object (zpymqi) for issuing the MQ API requests.

----------

## Validating your PyMQI Installation

Create file `ivp.py`

```py
import pymqi 
from pymqi import CMQC, CMQCFC 

# replace with your Queue Manager name
qmgr = pymqi.connect('CSQ9')

qmgr.disconnect()
```

and execute it

```sh
python3 ivp.py
```

If the script raises no exceptions, you have succesfully connected to your queue manager using PyMQI and verified your installation.

----------

## Background

The zpymqi code is written in C, and compiled as ASCII. When PyMQI calls the zpymqi code, the values are passed across in ASCII.  
The `pymqi/__init__.py` code translates the ASCII code to EBCDIC, where possible.  It is possible that applications may have to translate data themselves, or set the appropriate options for MQ to do the translation.  
If you hit this problem, please let the author know so the code can be updated.

## Using Python on z/OS

Python on z/OS has some interesting quirks.  I've documented some of them [here](https://colinpaice.blog/2022/01/04/python-on-z-os-helpful-hints/).

## Acknowledgements

This code was based on [PyMQI from Zato](https://dsuch.github.io/pymqi/), and the work by Darusz Suchojad.  Without him (and others) there would be no PyMQI code.

## Problems

- Any problems with PyMQI on z/OS should be raised as GitHub issues.
- Any questions about PyMQI on z/OS can be sent to [colinpaicemq@gmail.com](mailto:colinpaicemq@gmail.com).

----------

## Getting started with PyMQI on z/OS

[Using the API](docs/UsingApi.md)

[API referennce](docs/APIReference.md)

[Connect and disconnect](docs/ConnDisk.md)

[Creating messages](docs/Messages.md)

[Opening and closing a queue](docs/OpenClose.md)

[Simple put](docs/SimplePut.md)

[Getting messages from a queue](docs/get.md)

[Message properties and Message Handle](docs/msgprop.md)

[Commit and Backout](docs/commit.md)

[Inquire and Set](docs/inquire.md)

[Subscribe](docs/subscribe.md) and [Publish](docs/publish.md)

[Using PCF](docs/zpymqi_pcf.md)