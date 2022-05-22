import sys
import platform
import pymqi
import threading
from pymqi import CMQCFC

from pymqi import CMQC

# queue_manager = 'CSQ9'
# qmgr = pymqi.connect(queue_manager)
import myConnect # this isolates the MQ Connect from the main programs
qmgr = myConnect.connect()


channel = 'DEV.APP.SVRCONN'
channel_name = 'MYCHANNEL.1'
channel_name = 'SYSTEM.DEF.SENDER'
channel_name = 'SYS*'
channel_type = pymqi.CMQXC.MQCHT_SVRCONN
args = {pymqi.CMQCFC.MQCACH_CHANNEL_NAME: channel_name,
        pymqi.CMQCFC.MQIACF_REPLACE: pymqi.CMQCFC.MQRP_YES,
        pymqi.CMQCFC.MQIACH_CHANNEL_TYPE: channel_type}
pcf = pymqi.PCFExecute(qmgr,
            response_wait_interval=5000,
            convert=True);
#x=pcf.MQCMD_CREATE_CHANNEL(args)

#rgsi= {pymqi.CMQCFC.MQCACH_CHANNEL_NAME: channel_name,
#       pymqi.CMQCFC.MQIACH_CHANNEL_TYPE: channel_type}
argsi= {pymqi.CMQCFC.MQCACH_CHANNEL_NAME: channel_name}
#=pcf.PCFValue( 1352,4)
#rint("PCFValue 1392,4",z)
print("argsi",type(argsi))
print("argsi",argsi)
x=pcf.MQCMD_INQUIRE_CHANNEL(argsi)


print("size of returned " ,len(x))
print(" ")

i = 0
for xx in x:
    print(">>>>>>",i, "<<<<<<<<")
    i = i + 1
#   z = xx
#   z = pcf.prettify(xx  )
#   if 'MQCACH_CHANNEL_NAME' in z:
#       print("Channel ",z['MQCACH_CHANNEL_NAME'],"type",z['MQIACH_CHANNEL_TYPE'])
#   print("looup ",x)
#   for a in z:
#       print(a, z[a])
#   print("Typez",type(z))
#   print(z)
    for a in xx:
        for b in a:
             print(b, a[b])
#       print("????",type(a))
#       print(a)
'''
print("==x1==")
z = pcf.prettify(x[1])
for a in z:
    print(a, z[a])
'''
qmgr.disconnect()
