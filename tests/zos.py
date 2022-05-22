import sys
#python3  -m unittest ut.inq.test_inq_system
#mport logging
import time
import platform
import pymqi
import threading
from pymqi import CMQCFC
from pymqi import CMQC
import unittest
import myConnect # this isolates the MQ Connect from the main programs
from pymqi import PCF as PCF
import pymqiut
import utoptions

class zos(pymqiut.pymqiut):

    def zest_inq_archive(self):
        kw={}
        x = self.pcf.inq_archive()
        hh= self.pcf.headers()
        self.check(x, [19,2],  kw, hh)


    def test_inq_archive_all(self):
        kw = {"cmdscope":"*"}
#       print("XMQMD", pymqi.xMQMD)
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
#       self.qmgr.set_debug(1             )
        x = self.pcf.inq_archive(**kw        )
        hh= self.pcf.headers()
        self.check(x, [5,19,2,19,2,19,3], kw, hh)

if __name__ == '__main__':
     unittest.main()
