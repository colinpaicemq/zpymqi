import sys
import platform
import pymqi
from pymqi import CMQC

queue_manager = 'CSQ9'
qmgr = pymqi.connect(queue_manager)
hMsg = pymqi.MessageHandle(qmgr)

topic_string = '/currency/rate/EUR/USD'
mysub="MYSUB"
# set up subscribe
sub_desc = pymqi.SD()
sub_desc['Options'] = pymqi.CMQC.MQSO_CREATE + pymqi.CMQC.MQSO_RESUME + \
      pymqi.CMQC.MQSO_DURABLE + pymqi.CMQC.MQSO_MANAGED
sub_desc.set_vs('SubName', mysub    )
sub_desc.set_vs('ObjectString', topic_string)
print("SubLevel",sub_desc["SubLevel"])

sub = pymqi.Subscription(qmgr)
sub.sub(sub_desc=sub_desc)
# set up publish
#Msg.msg_handle
# get the message
get_opts = pymqi.GMO(
    MsgHandle=hMsg.msg_handle,
    Options=pymqi.CMQC.MQGMO_NO_SYNCPOINT +
            pymqi.CMQC.MQGMO_CONVERT      +
            pymqi.CMQC.MQGMO_PROPERTIES_IN_HANDLE +
            pymqi.CMQC.MQGMO_FAIL_IF_QUIESCING +
            pymqi.CMQC.MQGMO_WAIT)
get_opts['WaitInterval'] = 20000
data = sub.get(None, pymqi.md(), get_opts)
# qmgr.commit() not needed as NOSYNCPOINT

prop= pymqi.MessageHandle._Properties(qmgr.get_handle,hMsg.msg_handle)
p = prop.get("MQPubLevel")

print('Received data: [%s]' % data)
s  = sub.getSD()
print("sub:42 publevel",p)
v = hMsg.get_all_properties()
for x in v:
    print(" PUB",x,v[x])
print("pub: properties",v)
#rint("ZZZZ",v['MQTopicString'])
