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

class set(pymqiut.pymqiut):

    def test_set_qmgr(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        kw={"certlabl":"ibmWebSphereMQM801"}
        x = self.pcf.alter_qmgr(**kw                 )
        hh= self.pcf.headers()
        self.check(x,[2],kw ,hh)
        self.checkHeader(self.pcf.headers())

    @unittest.skipIf(platform.system() == 'OS/390',"Not on z/OS")
    def test_set_qmgr_maxmsgl(self):
        kw={"maxmsglength":104857599}
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        x = self.pcf.alter_qmgr(**kw                 )
        hh= self.pcf.headers()
        self.check(x, [2], kw ,hh)
        self.checkHeader(self.pcf.headers())
#       self.qmgr.set_debug(0)


    def test_set_qmgr_tcpchl(self):
        kw = {"tcpchl":201,"actchl":201,"maxchl":201}
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        x = self.pcf.alter_qmgr(**kw                 )
        hh= self.pcf.headers()
        self.check(x, [2], kw ,hh)
        self.checkHeader(self.pcf.headers())
#       self.qmgr.set_debug(0)

    def test_set_qmgr_lu62chl(self):
        kw = {"lu62chl":201,"actchl":201,"maxchl":201}
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        x = self.pcf.alter_qmgr(**kw                  )
        hh= self.pcf.headers()
        self.check(x, [2], kw, hh)
        self.checkHeader(self.pcf.headers())
#       self.qmgr.set_debug(0)


    def test_set_qmgr_maxmsgl(self):
        kw = {"trigint":9999998 }
        x = self.pcf.alter_qmgr(**kw                )
        hh= self.pcf.headers()
        self.check(x, [2] ,kw, hh)
        self.checkHeader(self.pcf.headers())


    def test_set_ql(self):
        kw = {"q":"CP0000","qtype":pymqi.CMQC.MQQT_LOCAL,"qdesc":"COLINS Comment"}
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        x = self.pcf.alter_q(**kw)
        hh= self.pcf.headers()
        self.check(x, [2], kw, hh)
        self.checkHeader(self.pcf.headers())

if __name__ == '__main__':
     unittest.main()
