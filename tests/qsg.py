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

class qsg(pymqiut.pymqiut):

#   def test_inq_qsgive(self):
#       x = self.pcf.inq_qsg()
#       self.check(x, [10,10,100,2] ,hh) # 10 for each queue manager
#

#   def test_start_smdsconn(self):
#       kw = {"cfname":"CSQSYSAPPL","smdsconn":"*","cmdscope":"*"}
#       self.qmgr.set_debug(1+  pymqi.xPCF)
#       x = self.pcf.inq_qsg(**kw )
#       self.check(x, [10,10,10,2],kw ,hh) # 10 for each queue manager


    def test_inq_cfstatus_summary(self):
        kw = {"cfname":"CSQSYSAPPL"}
        x = self.pcf.inq_cfstatus_summary(**kw               )
        hh= self.pcf.headers()
        self.check(x, [13,2],kw ,hh) # 10 for each queue manager

    def test_inq_cfstatus_connect(self):
        kw = {"cfname":"CSQSYSAPPL"}
        x = self.pcf.inq_cfstatus_connect(**kw )
        hh= self.pcf.headers()
        self.check(x, [9,9,9,2],kw ,hh) # 10 for each queue manager

    def test_inq_cfstatus_backup(self):
        kw = {"cfname":"CSQSYSAPPL"}
        x = self.pcf.inq_cfstatus_backup(**kw)
        hh= self.pcf.headers()
        self.check(x, [14,2],kw ,hh) # 10 for each queue manager


    def test_inq_cfstatus_smds(self):
        kw = {"cfname":"CSQSYSAPPL"}
        x = self.pcf.inq_cfstatus_smds(**kw)
        hh= self.pcf.headers()
        self.check(x, [10,10,10,2],kw ,hh) # 10 for each queue manager

if __name__ == '__main__':
     unittest.main()
