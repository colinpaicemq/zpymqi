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

class inqcf(pymqiut.pymqiut):

    def test_inq_qsg(self,**kwargs) :
        print("====================",__name__,"================================++++++")
        kw={}
        x = self.pcf.inq_qsg()
        hh= self.pcf.headers()
        self.check(x, [10,10,10,2], kw, hh)

    def test_inq_cf_struct(self,**kwargs) :
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        kw={}
        x = self.pcf.inq_cf_struct()
        hh= self.pcf.headers()
        self.check(x, [21,2], kw, hh)

    def test_inq_smds(self,**kwargs) :
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        kw={}
        x = self.pcf.inq_smds()
        hh= self.pcf.headers()
        self.check(x, [6,6,6,2], kw, hh)

    def test_inq_smdsconn(self,**kwargs) :
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        kw={}
        x = self.pcf.inq_smdsconn()
        hh= self.pcf.headers()
        self.check(x, [8,2], kw, hh)

    def test_inq_cf_struct_names(self,**kwargs) :
        kw={}
        x = self.pcf.inq_cf_names()
        hh= self.pcf.headers()
        self.check(x, [3,2], kw, hh)

    def test_inq_cf_struct_status(self,**kwargs) :
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        kw={}
        x = self.pcf.inq_cf_struct_status()
        hh= self.pcf.headers()
        self.check(x, [13,13,2], kw, hh)

    def test_inq_usage_smds(self,**kwargs) :
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        kw={}
        x = self.pcf.inq_usage_smds()
        hh= self.pcf.headers()
        self.check(x,  [11,13,2], kw, hh)


if __name__ == '__main__':
     unittest.main()
