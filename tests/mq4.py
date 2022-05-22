import sys
import platform
sys.path.append('./')
import pymqi
queue_manager = 'CSQ9'
qmgr = pymqi.connect(queue_manager)
queue = pymqi.Queue(qmgr, 'CP0000')
message = queue.put("COLINSMESSAGE")
queue2= pymqi.Queue(qmgr, 'CP0000', pymqi.CMQC.MQOO_INPUT_SHARED +pymqi.CMQC.MQOO_INQUIRE+pymqi.CMQC.MQOO_SET)
mxd = queue2.inquire(pymqi.CMQC.MQIA_INHIBIT_GET)
print("mxd",mxd)
print("=======")
mx2 = queue2.set(pymqi.CMQC.MQIA_INHIBIT_GET,0 )
print("m2",mx2)
print("=======")
print("inh get   ",mxd,mx2)
message = queue2.get()
print("Message", message)
od = queue.getOD()
print("=OD==")
for o in od:
   print(o,":",od[o])
#rint("OD",od)
#q = od["DynamicQName"]
#rint("type rq",type(rq))
#rint("RQ",rq)
#rint("RQ2",rq2)
#rint("----------__")
#rint (queue.getOD())
md  =   (queue.getMD())
print("==MD===")
for m in md:
   print(m,":",md[m])
queue.close()
queue2.close()
queuei= pymqi.Queue(qmgr, 'CP0000')
print("Queuei",queuei)
d       = queuei.inquire(pymqi.CMQC.MQIA_CURRENT_Q_DEPTH)
print("depth",d)
try:
    d       = queuei.inquire(pymqi.CMQC.MQIA_INHIBIT_EVENT  )
#xcept MQMIError as e:
#   print("30 exception is ",e)
except pymqi.MQMIError as e:
    e2  = sys.exc_info()
    v = e2[1].errorAsString()
    if v == "FAILED: MQRC_SELECTOR_ERROR":
       print("It gave the correct return code")
    else:
       print("It gave the WRONG return code",e2)
except:
    e = sys.exc_info()
    print("35 exception is ",e)

queuei.close()
qmgr.disconnect()
