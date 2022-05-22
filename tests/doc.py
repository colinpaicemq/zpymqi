import sys
import platform
import pymqi
import threading
from pymqi import CMQCFC
from pymqi import CMQC
from pymqi import PCF as PCF
queue_manager = 'M801'
qmgr = pymqi.connect(queue_manager)
pcf = PCF.PCF(qmgr,
            response_wait_interval=5000,
            convert=True);
#mgr.set_debug(1+  pymqi.xPCF              )
#= pcf.inq_q_status(q='SY*',where=("curdepth",">",1))
data = pcf.inq_q(q=b'CP0000',qtype=pymqi.CMQC.MQQT_LOCAL)
# ta = pcf.inq_q(q=b'CP0000',qtype="LOCAL")
headers = pcf.headers()
for h in headers:
     if h["Reason"] != 0:
        print("PCF reason code non zero", h["Reason"] )
#    print("=====")
#    print(h)
i = 0
for row in data:
   i = i + 1
   # convert values to character strings
   pretty = pcf.prettify(row)
   print("=======Record:",i,"==========")
   print(">>>Number of entries:",len(pretty))
   for a in pretty:
       value = pretty[a] # get the value
       if not isinstance(value,list ):
            value = (value,) # make it a tuple
       for v in value:  # allow for arrays of values
           aout = str(v)
           print(a,":",aout)
