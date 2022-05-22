import sys
#mport logging
import time
import platform
import pymqi
import threading
from pymqi import MQOTHER
from pymqi import CMQCFC
from pymqi import CMQC
from pymqi import PCF as PCF

import myConnect # this isolates the MQ Connect from the main programs
qmgr = myConnect.connect()

# qmgr.set_debug(1+pymqi.xCC+pymqi.xPCF)
def check(data,what):
      l = []
      for d in data:
#         d  = pcf.prettify(d   )
#         print(d)
          l.append(len(d))
      print(what)
      print("self.check(x,",l,")")

#cf = pymqi.PCFExecute(qmgr,
#cf = pymqi.PCF(qmgr,
pcf = PCF.PCF(qmgr,
            response_wait_interval=5000,
            convert=True);
#heck(pcf.inq_log(),"inq_log()")
#heck(pcf.inq_cf_names(cfname="*"),'inq_cf_names(cfname="*")')
#heck(pcf.inq_conn(gconid=pymqi.ByteString(b'')),"inq_conn(gconid=pymqi.ByteString(b''))")
#heck(pcf.inq_thread(),"pcf.inq_thread()")
#heck(pcf.inq_appl_status(),"inq_appl_status()")
#  = pcf.inq_log(cmdscope="*")
#  = pcf.inq_comm_info()
#  = pcf.inq_sub(            subid=pymqi.ByteString(b'0000'))
#  = pcf.inq_q_status(q='SY*',where=("curdepth",">",1))
#  = pcf.inq_q_status(q='CP00*' )
#  = pcf.zzz_q_status(q='SYSTEM.*',
# = pcf.inq_archive()
# = pcf.www_usage(usage=CMQCFC.MQIACF_USAGE_PAGESET,ps=1,where=("ps","eq",1))
# = pcf.zzz_usage(usage=CMQCFC.MQIACF_USAGE_PAGESET,where=("ps","eq",1))
# = pcf.inq_usage(ps=1)
# = pcf.inq_usage_log()
# = pcf.inq_usage(usage=CMQCFC.MQIACF_USAGE_DATA_SET)
#x = pcf.inq_stgclass()
###########done ####
#heck(pcf.inq_stgclass(stgclass='*'),"inq_stgclass(stgclass='*')")
#heck(pcf.inq_security(),"inq_security() ")
x =   pcf.inq_chinit()
#heck(pcf.inq_topic_names(tn="*"),'inq_topic_names(tn="*")')
#heck(pcf.inq_topic(tn="SYSTEM.BASE.TOPIC"),'inq_topic(tn="SYSTEM.BASE.TOPIC")')
#=    pcf.inq_qmgr()
#=    pcf.zzz_qmgr()
#heck(pcf.inq_pageset(psid=1),"inq_pageset(psid=1)")
#heck(pcf.inq_sub_status(subname="MySub3"),'inq_sub_status(subname="MySub3")')
#heck( pcf.inq_topic_status(ts="#"),'inq_topic_status(ts="#")')
#heck(pcf.inq_chl_auth_records(channel="*"),'inq_chl_auth_records(channel="*")')
#heck(pcf.inq_trace(),'inq_trace()')
# = pcf.inq_qmgr()
#rint("Debug",qmgr.get_debug())
# = pcf.inq_q(q='CP0000',qtype="LOCAL")
#  = pcf.inq_system()
# time.sleep(60) # Sleep for n seconds
#  = pcf.inq_sub(subname="MySub3")
headers = pcf.headers()
i = 0
for h in headers:
    i = i + 1
    print("== Header",i,"==")
    print(h.get())
    print(pcf.prettifyPCFHeader(h))
'''
'''
print(" ")
i = 0
for xx in x:
    i = i + 1
    print("==Response",i,"====================")
    z = pcf.prettify(xx  )
#   z = xx
#   print(">>>",len(z))
    for a in z :
        print(a,":", z[a])
qmgr.disconnect()
