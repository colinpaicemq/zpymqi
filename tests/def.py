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

class def0(pymqiut.pymqiut):

    def test_def_ql(self):
        qn    = utoptions.tempql
        x = self.pcf.def_q(qname="TEMPQL",acctq="Q_MGR",qtype="LOCAL",qdesc="PythonQ",replace="YES",usage="NORMAL")
        self.check(x,[2])

        x = self.pcf.del_q(qname="TEMPQL",qtype="LOCAL")
        self.check(x,[2])

    def test_def_channel(self):
        x = self.pcf.def_channel(channel="PYSVRCONN",chltype="SVRCONN",replace="YES")
        self.check(x,[2])

        x = self.pcf.del_channel(channel="PYSVRCONN")
        self.check(x,[4,2])
if __name__ == '__main__':
     unittest.main()
