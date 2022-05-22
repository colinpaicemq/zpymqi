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

class syncpoint(pymqiut.pymqiut):

    def test_syncpoint(self):
        open_options = pymqi.CMQC.MQOO_INPUT_EXCLUSIVE + pymqi.CMQC.MQOO_OUTPUT
        # Get Message Options
        queue = pymqi.Queue(self.qmgr,"PYMQI_TEST",open_options)
        message = "MessageContent"
        pmo = pymqi.PMO()
        put_mqmd = pymqi.MD()
        pmo.Options = pymqi.CMQC.MQPMO_SYNCPOINT
        queue.put(message, put_mqmd, pmo)
        self.qmgr.commit()#  needed as   SYNCPOINT
        gmo = pymqi.GMO(Options=pymqi.CMQC.MQGMO_SYNCPOINT)
        gmo.WaitInterval = 0000 # 0 seconds
        data = queue.get(None, pymqi.md(), gmo)
        # first time backout
        print('Received data1: [%s]' % data)
        self.qmgr.backout()#  needed as   SYNCPOINT

        data = queue.get(None, pymqi.md(), gmo)
        # second time commit
        self.qmgr.commit()#  needed as   SYNCPOINT

        print('Received data2: [%s]' % data)
        self.assertEqual(data, b'MessageContent' )


if __name__ == '__main__':
     unittest.main()
