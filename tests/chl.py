
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
class inqchannel(pymqiut.pymqiut):

#   def test_inq_channel_names(self,**kwargs):
#       x = self.pcf.inq_channel_names(channel="SYSTEM.*")
#
#       self.check(x,[5,2])
#

    def zzzz_inq_channel_status(self,**kwargs):
        x =[]
        try:
            x = self.pcf.inq_channel_status(channel="SYSTEM.*")
    #   except pymqi.MQMIError as e:
    #       e2  = sys.exc_info()
    #       v = e2[1].errorAsString()
    #       if v == "FAILED: MQRC_SELECTOR_ERROR":
    #          print("It gave the correct return code")
    #       else:
    #          print("It gave the WRONG return code",e2)
            print(x);
        except:
            e = sys.exc_info()
            print("75 exception is ",e)

        self.check(x,[5,2])

    def test_inq_channel_status(self,**kwargs):
#       self.qmgr.set_debug(1+  pymqi.xPCF)
#       kw = {"channel":"SYSTEM.DEF.SVRCONN" }
#       kw = {"channel":"M801.TO.M701" }
        kw = {"channel":"*"            }
        hh= self.pcf.headers()
        x = self.pcf.inq_channel_status(**kw)
        self.check(x,[5,2],kw,hh)


if __name__ == '__main__':
     unittest.main()
