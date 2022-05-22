import sys
#mport logging
import platform
import pymqi
from pymqi import CMQC
def E2A(s): # covert Ascii to EBCDIC if on z/OS
    if isinstance(s,bytes):
        m = s.decode('cp500')
        s = m.encode("ascii")
    return s

list=( pymqi.CMQC.MQCA_TRIGGER_DATA             ,
    pymqi.CMQC.MQIA_DIST_LISTS                  ,
    pymqi.CMQC.MQIA_INHIBIT_GET                 ,
    pymqi.CMQC.MQIA_INHIBIT_PUT                 ,
    pymqi.CMQC.MQIA_TRIGGER_CONTROL             ,
    pymqi.CMQC.MQIA_TRIGGER_DEPTH               ,
    pymqi.CMQC.MQIA_TRIGGER_MSG_PRIORITY        ,
    pymqi.CMQC.MQIA_TRIGGER_TYPE                )

queue_manager = 'CSQ9'
qmgr = pymqi.connect(queue_manager)
queue = pymqi.Queue(qmgr, 'CP0000',pymqi.CMQC.MQOO_INQUIRE+pymqi.CMQC.MQOO_SET)
x = queue.inquire(pymqi.CMQC.MQIA_SCOPE)
for v in list:
   x = queue.inquire(v)
   print(queue.inq_fieldName,x)
   prop = queue.inq_fieldName
   if x is not None:
      print("Set ",queue.inq_fieldName, x, v)
      y = queue.set(v,x)
      print("reply from set",y,queue.inq_fieldName)
