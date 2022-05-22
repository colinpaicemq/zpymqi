import sys
import platform
import pymqi
from pymqi import CMQC

queue_manager = 'CSQ9'
qmgr = pymqi.connect(queue_manager)
qmgr.set_debug(1+pymqi.xCC+pymqi.xPCF+pymqi.xSUB+pymqi.xGETBUFFER)
hMsg = pymqi.MessageHandle(qmgr)

topic_string = '/currency/rate/EUR/USD'
mysub="MYSUB"
# set up subscribe
sub_desc = pymqi.SD()
#ub_desc['Options'] = pymqi.CMQC.MQSO_CREATE + pymqi.CMQC.MQSO_RESUME + \
#                             pymqi.CMQC.MQSO_MANAGED
sub_desc['Options'] = pymqi.CMQC.MQSO_CREATE + pymqi.CMQC.MQSO_RESUME + \
      pymqi.CMQC.MQSO_DURABLE + pymqi.CMQC.MQSO_MANAGED
sub_desc['SubLevel'] = 9
# this makes the subscription permament
sub_desc.set_vs('SubName', mysub    )
sub_desc.set_vs('ObjectString', topic_string)
print("SubLevel",sub_desc["SubLevel"])

sub = pymqi.Subscription(qmgr)
sub.sub(sub_desc=sub_desc)
# set up publish
msg = '1.3961'
md  = pymqi.md()
md["Persistence"] = pymqi.CMQC.MQPER_NOT_PERSISTENT
pmo = pymqi.PMO(Version=pymqi.CMQC.MQPMO_VERSION_3) # PMO v3 is required properties
pmo["PubLevel"] = 9
#mo["Version"] = 4
pmo["Options"] = pymqi.CMQC.MQPMO_NO_SYNCPOINT + \
                 pymqi.CMQC.MQPMO_FAIL_IF_QUIESCING  + pymqi.CMQC.MQPMO_RETAIN
print("PMO PUBLevel", pmo["PubLevel"])
print("PMO Options ", pmo["Options"])
topic = pymqi.Topic(qmgr, topic_string=topic_string)
topic.open(open_opts=pymqi.CMQC.MQOO_OUTPUT+pymqi.CMQC.MQOO_PASS_ALL_CONTEXT)
topic.pub(msg,md,pmo)
#opic.pub(msg)
topic.close()
qmgr.commit()

#Msg.msg_handle
# get the message
get_opts = pymqi.GMO(
    MsgHandle=hMsg.msg_handle,
    Options=pymqi.CMQC.MQGMO_NO_SYNCPOINT +
            pymqi.CMQC.MQGMO_CONVERT      +
            pymqi.CMQC.MQGMO_PROPERTIES_IN_HANDLE +
            pymqi.CMQC.MQGMO_FAIL_IF_QUIESCING +
            pymqi.CMQC.MQGMO_WAIT)
get_opts['WaitInterval'] = 1000
data = sub.get(None, pymqi.md(), get_opts)
# qmgr.commit() not needed as NOSYNCPOINT
print('Received data: [%s]' % data)
s  = sub.getSD()
v = hMsg.get_all_properties()
for x in v:
    print(" PUB",x,v[x])
print("pub: properties",v)
#rint("ZZZZ",v['MQTopicString'])
