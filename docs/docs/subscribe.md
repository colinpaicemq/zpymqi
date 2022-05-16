##Subscribing to a topic

The first example shows how to use a managaged object to subscribe to a topic, and get from the queue.
The second example shows how to use a queue.

### Using a system managed object

- The program uses a managed queue (it is a temporary object).  
- It uses a message handle, to get message properties from the published message.
- It sets up a subscription Description (MQSD) and initilises it.
- It creates a subscription and issues a sub() to the subscription.
- It then does an sub.get from the managed option passing in an MQMD and the GMO object.
- It displays the message
- It displays any properties received.

Code::
```
    import sys 
    import platform 
    import pymqi 
    from pymqi import CMQC

    queue_manager = 'M801' 
    qmgr = pymqi.connect(queue_manager) 
    qmgr.set_debug(1+pymqi.xCC+pymqi.xPCF+pymqi.xSUB+pymqi.xGETBUFFER)
    # use message handle to get the properties 
    hMsg = pymqi.MessageHandle(qmgr) 
                                                                                         
    topic_string = '/currency/rate/EUR/USD' 
    mysub="COLINSUB" 
    #  set up subscribe 
    sub_desc = pymqi.SD() 
    sub_desc['Options'] = pymqi.CMQC.MQSO_CREATE + pymqi.CMQC.MQSO_RESUME + \ 
          pymqi.CMQC.MQSO_DURABLE + pymqi.CMQC.MQSO_MANAGED 
    sub_desc['SubLevel'] = 9 
    
    # this makes the subscription permament 
    sub_desc.set_vs('SubName', mysub    ) 
    sub_desc.set_vs('ObjectString', topic_string) 
       
    # create the subscription                                                                   
    sub = pymqi.Subscription(qmgr) 
    sub.sub(sub_desc=sub_desc)  # Issue the MQSUB request


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
    # print("SD:",sub_desc) 
    # get the message properties from the message.
    v = hMsg.get_all_properties() 
    for x in v: 
        print(" PUB",x,v[x]) 
    # print("pub: properties",v) 

```

### Using a queue

The default is to use a system managed object. If you want to specify a queue for the subscription you can pass the queue object.

code::

```
     hQueue = pymqi.Queue(qmgr, 'COLINSUBQ',pymqi.CMQC.MQOO_INPUT_SHARED) 
     sub = pymqi.Subscription(qmgr)
     sub.sub(sub_desc=sub_desc, sub_queue = hQueue) 
```