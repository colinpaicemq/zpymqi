import sys
#python3  -m unittest ut.inq.test_inq_system
#mport logging
import time
import platform
import pymqi
import threading
import utoptions as utoptions
from pymqi import CMQCFC
from pymqi import CMQC
from pymqi import PCF as PCF
import unittest
import myConnect # this isolates the MQ Connect from the main programs
import pymqiut
class inq(pymqiut.pymqiut):

    def test_inq_qmgr(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw={}
        x = self.pcf.inq_qmgr()
        hh= self.pcf.headers()
        self.check(x, [107,2], kw, hh)

    def test_inq_chinit(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw={}
        x = self.pcf.inq_chinit()
        hh= self.pcf.headers()
        self.check(x, [2,20,7,5,5,5,2], kw, hh)

    def test_inq_q(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw={"q":"CP0000"}
        x = self.pcf.inq_q(**kw      )
        hh= self.pcf.headers()
        self.check(x, [60,2],kw ,hh)

    def test_inq_system(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw={}
        x = self.pcf.inq_system()
        hh= self.pcf.headers()
        self.check(x, [37,2] ,kw,hh)

    def test_inq_sub(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw = {"subname":"SYSTEM.DEFAULT.SUB"}
        x = self.pcf.inq_sub(**kw            )
        hh= self.pcf.headers()
        self.check(x, [31,2], kw, hh)

    def test_inq_sub_status(self):
        kw = {"subname":"SYSTEM.DEFAULT.SUB"}
        x = self.pcf.inq_sub_status(**kw            )
        hh= self.pcf.headers()
        self.check(x, [14,2], kw, hh)

    def test_inq_topic_status(self):
##      self.qmgr.set_debug(1+  pymqi.xPCF)
        kw = {"ts":"SYSTEM.BROKER.ADMIN.STREAM"}
        x = self.pcf.inq_topic_status(**kw  )
        hh= self.pcf.headers()
        self.check(x, [23, 2], kw, hh)

    def test_inq_sub_status(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw = {"subname":"SYSTEM.DEFAULT.SUB"}
        x = self.pcf.inq_sub_status(**kw            )
        hh= self.pcf.headers()
        self.check(x, [14, 2], kw, hh)

    def test_inq_chl_auth_records(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw = {"channel":"*"}
        x = self.pcf.inq_chl_auth_records(**kw       )
        hh= self.pcf.headers()
        self.check(x, [10, 13, 13, 2], kw, hh)

    def test_inq_pageset(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw = {"psid":1}
        x = self.pcf.inq_pageset(**kw  )
        hh= self.pcf.headers()
        self.check(x, [13, 9, 2],kw ,hh)

    def test_inq_topic_names(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw = {"tn":"*" }
        x = self.pcf.inq_topic_names(**kw  )
        hh= self.pcf.headers()
        self.check(x, [4, 2], kw, hh)

    def test_inq_qmgr(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw={}
        x = self.pcf.inq_qmgr()
        hh= self.pcf.headers()
        self.check(x, [107, 2], kw, hh)

    def test_inq_log(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw={}
        x = self.pcf.inq_log()
        hh= self.pcf.headers()
        self.check(x, [16,8,8,11,2], kw, hh)
#       self.check(x, [16,4,8,8,11,2    ] ,hh)

    def test_inq_stgclass(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw = {"stgclass":"DEFAULT"}
        x = self.pcf.inq_stgclass(**kw )
        hh= self.pcf.headers()
        self.check(x, [11, 2], kw ,hh)

    def test_inq_security(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw={}
        x = self.pcf.inq_security()
        hh= self.pcf.headers()
        self.check(x, [3, 3, 3, 5, 2] ,kw, hh)

    def test_inq_usage_ps(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
#       self.qmgr.set_debug(1+  pymqi.xPCF)
        kw = {"ps":1   }
        x = self.pcf.inq_usage_ps(**kw)
        hh= self.pcf.headers()
        self.check(x, [13,9,2], kw, hh)

    def test_inq_usage_log(self):
#       self.qmgr.set_debug(1+  pymqi.xPCF              )
        kw={}
        x = self.pcf.inq_usage_log()
        hh= self.pcf.headers()
        self.check(x, [6,6,6,2] ,kw, hh)

if __name__ == '__main__':
     unittest.main()
