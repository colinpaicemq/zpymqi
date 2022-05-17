# Publish to a topic

Code::

```

    import sys 
    import platform 
    import pymqi 
    from pymqi import CMQC 
                                                                                                
    queue_manager = 'M801' 
    qmgr = pymqi.connect(queue_manager) 
    qmgr.set_debug(1+pymqi.xCC+pymqi.xPCF+pymqi.xSUB+pymqi.xGETBUFFER) 
                                                                                           
    topic_string = '/currency/rate/EUR/USD' 
   
    # set up publish 
    msg = '1.3961' # this is the value
    md  = pymqi.md() # create the MD, so we can override it 
    md["Persistence"] = pymqi.CMQC.MQPER_NOT_PERSISTENT 
    # create the PMO
    pmo = pymqi.PMO(Version=pymqi.CMQC.MQPMO_VERSION_3) # PMO v3 is required properties 
    pmo["PubLevel"] = 9 
    pmo["Options"] = pymqi.CMQC.MQPMO_SYNCPOINT + \ 
                     pymqi.CMQC.MQPMO_FAIL_IF_QUIESCING  + pymqi.CMQC.MQPMO_RETAIN 
    print("PMO PUBLevel", pmo["PubLevel"]) 
    print("PMO Options ", pmo["Options"])
    # create the topic 
    topic = pymqi.Topic(qmgr, topic_string=topic_string) 
    topic.open(open_opts=pymqi.CMQC.MQOO_OUTPUT+pymqi.CMQC.MQOO_PASS_ALL_CONTEXT) 
    # and issue the publish
    topic.pub(msg,md,pmo) 
    #opic.pub(msg) 
    topic.close() 
    qmgr.commit() # needed because the MQPO_SYNCPOINT
```
