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

class seq(unittest.TestCase):
    def setUp(self):
#        self.platform =  platform.system()
#        self.qmgr = myConnect.connect()
#        self.pcf = pymqi.PCFExecute(self.qmgr,
#        self.pcf = pymqi.PCF(self.qmgr,
#             response_wait_interval=2000,
#             convert=True);
#        self.qmgr.set_debug(0)
#        self.qmgr.set_debug(1+2+4+8      )

    def tearDown(self):
       self.qmgr.disconnect()

    def checkMQList(self,l):
        p = self.pcf.prettify(l )
        for k in p :
#           print(k)
            if not k.startswith('MQ'):
                print("Fails to start with MQ",k)
                return False
        return True

    def checkHeader(self,hh):
        if hh[0]['Reason'] != 0:
            for h in hh:
                print(k)
        self.assertEqual(hh[0]['Reason'],0)

    def check(self,x,records):
        self.assertEqual(len(x),len(records)) # data and trailer
        for (item,count) in list(zip(x, records)):
            self.assertGreaterEqual(len(item),count)
        for item in x:
           self.assertTrue(self.checkMQList(item))
    def printit(self,x):
         i = 0
         for xx in x:
             i = i + 1
             print("=============",i,"====================")
             z = self.pcf.prettify(xx  )
         #   print(">>>",len(z))
             for a in z:
                 print(a, z[a])

class conn(seq):

    def test_set_qmgr(self):
        x = self.pcf.alter_qmgr(certlabl="COLINSCERT")
        headers = self.pcf.headers()
        self.assertEqual(headers[0]['Reason'],0)

    @unittest.skipIf(platform.system() == 'OS/390',"Not on z/OS")
    def test_set_qmgr_maxmsgl(self):
        self.qmgr.set_debug(1+                       pymqi.PCF.xPCF)
        x = self.pcf.alter_qmgr(maxmsglength=222222  )
        self.checkHeader(self.pcf.headers())
        self.qmgr.set_debug(0)


    def test_set_qmgr_tcpchl(self):
        x = self.pcf.alter_qmgr(tcpchl=66            )
        self.checkHeader(self.pcf.headers())
#       self.qmgr.set_debug(0)

    def test_set_qmgr_lu62chl(self):
        x = self.pcf.alter_qmgr(lu62chl=66            )
        self.checkHeader(self.pcf.headers())
#       self.qmgr.set_debug(0)


    def test_set_qmgr_maxmsgl(self):
        x = self.pcf.alter_qmgr(trigint=88888888    )
        self.checkHeader(self.pcf.headers())


    def test_set_ql(self):
        x = self.pcf.alter_q(q="CP0000",qtype=pymqi.CMQC.MQQT_LOCAL,qdesc="COLINS Comment")
        self.checkHeader(self.pcf.headers())



if __name__ == '__main__':
     unittest.main()
