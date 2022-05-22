import sys
#mport logging
import platform
import pymqi
import threading
from pymqi import CMQCFC

from pymqi import CMQC

import myConnect # this isolates the MQ Connect from the main programs
qmgr = myConnect.connect()

pcf = pymqi.PCFExecute(qmgr,
            response_wait_interval=5000,
            convert=True);
# = pcf.inq_qmgr()
z = pcf.inq_q(q=b'CP0000',qtype=pymqi.CMQC.MQOT_LOCAL_Q)
# = pcf.inq_q(q=b'CP0000')
i  = 0
for xx in z:
    i = i + 1
    z = pcf.prettify(xx  )
    for a in z:
        print(a, z[a])
qmgr.disconnect()
